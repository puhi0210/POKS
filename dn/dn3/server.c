#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h> //write
#include <pthread.h> //for threading , link with lpthread
#include <time.h>
// #include <uuid/uuid.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>

unsigned int crc32_table[256];

void init_crc32_table() {
    unsigned int polynomial = 0xEDB88320;
    for (int i = 0; i < 256; i++) {
        unsigned int crc = i;
        for (int j = 0; j < 8; j++) {
            if (crc & 1)
                crc = (crc >> 1) ^ polynomial;
            else
                crc >>= 1;
        }
        crc32_table[i] = crc;
    }
}

unsigned int compute_crc32(const char *data) {
    unsigned int crc = 0xFFFFFFFF;
    while (*data) {
        unsigned char byte = (unsigned char)(*data++);
        crc = (crc >> 8) ^ crc32_table[(crc ^ byte) & 0xFF];
    }
    return crc ^ 0xFFFFFFFF;
}

void generateUUID(int *uuid) {
    for (int i = 0; i < 8; i++) {
        switch (i) {
        case 3:
            uuid[i] = (rand() % 0x0FFF) | 0x4000;
            break;
        case 4:
            uuid[i] = (rand() % 0x3FFF) | 0x8000;
            break;
        default:
            uuid[i] = rand() % 0xFFFF;
            break;
        }
    }
}

void uuidStr(int *uuidInt, char *uuidStr, int uuidNum) {
    snprintf(uuidStr, 46, "%08d-%04x%04x-%04x-%04x-%04x-%04x%04x%04x", uuidNum, uuidInt[0], uuidInt[1], uuidInt[2], uuidInt[3], uuidInt[4], uuidInt[5], uuidInt[6], uuidInt[7]);
}

void error(const char *msg) {
    perror(msg);
    exit(0);
}

// the thread function
void *connection_handler(void *socket_desc) {
    int sock, n;
    struct sockaddr_in from;
    socklen_t fromlen;
    char buf[1024];
    int counter = 1;
    int uuid[8];
    char strUUID[46];
    unsigned int last_sent_crc = 0;

    sock = *(int *)socket_desc;
    free(socket_desc);
    fromlen = sizeof(struct sockaddr_in);

    init_crc32_table();
    srand(time(NULL));

    while (1) {
        bzero(buf, 1024);
        n = recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&from, &fromlen);
        if (n < 0) {
            perror("recvfrom");
            break;
        }

        printf("Received a datagram: %s\n", buf);

        if (strcmp(buf, "GET") == 0) {
            generateUUID(uuid);
            uuidStr(uuid, strUUID, counter);
            last_sent_crc = compute_crc32(strUUID);
            char response[64];
            snprintf(response, sizeof(response), "%s %08X", strUUID, last_sent_crc);
            n = sendto(sock, response, strlen(response), 0, (struct sockaddr *)&from, fromlen);
            if (n < 0) {
                perror("sendto");
                break;
            }
            printf("Sent: %s\n", response);
        } else if (strncmp(buf, "PREJETO", 7) == 0) {
            char received_crc[9];
            sscanf(buf + 8, "%8s", received_crc);
            unsigned int received_crc_int = strtoul(received_crc, NULL, 16);

            if (received_crc_int == last_sent_crc) {
                counter++;
                printf("CRC32 valid. Counter incremented.\n");
            } else {
                char error_msg[64];
                snprintf(error_msg, sizeof(error_msg), "NAPAKA %08X", received_crc_int);
                n = sendto(sock, error_msg, strlen(error_msg), 0, (struct sockaddr *)&from, fromlen);
                if (n < 0) {
                    perror("sendto");
                    break;
                }
                printf("Sent: %s\n", error_msg);
            }
        } else {
            char error_msg[] = "NEPREPOZNAVEN UKAZ";
            n = sendto(sock, error_msg, strlen(error_msg), 0, (struct sockaddr *)&from, fromlen);
            if (n < 0) {
                perror("sendto");
                break;
            }
            printf("Sent: %s\n", error_msg);
        }
    }

    close(sock);
    return 0;
}

int main(int argc, char *argv[]) {
    int socket_desc, client_sock, c, *new_sock;
    struct sockaddr_in server, client;

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
    }
    puts("Socket created");

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5095);

    // Bind
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    // Listen
    listen(socket_desc, 3);

    // Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    while ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c))) {
        puts("Connection accepted");

        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;

        if (pthread_create(&sniffer_thread, NULL, connection_handler, (void *)new_sock) < 0) {
            perror("could not create thread");
            return 1;
        }
        puts("Handler assigned");
    }

    if (client_sock < 0) {
        perror("accept failed");
        return 1;
    }

    return 0;
}
