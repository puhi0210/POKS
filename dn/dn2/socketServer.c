#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

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

int main(int argc, char *argv[]) {
    int sock, n;
    struct sockaddr_in server, from;
    socklen_t fromlen;
    char buf[1024];
    int counter = 1;
    int uuid[8];
    char strUUID[46];
    unsigned int last_sent_crc = 0;

    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(0);
    }

    srand(time(NULL));
    init_crc32_table();

    sock = socket(AF_INET, SOCK_DGRAM, 0); /* create new socket */
    if (sock < 0) error("Opening socket");

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET; /* Internet */
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(argv[1])); /* port */

    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0) /* bind socket with port */
        error("binding");

    fromlen = sizeof(struct sockaddr_in);

    while (1) {
        bzero(buf, 1024);
        n = recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&from, &fromlen);
        if (n < 0) error("recvfrom");

        printf("Received a datagram: %s\n", buf);

        if (strcmp(buf, "GET") == 0) {
            generateUUID(uuid);
            uuidStr(uuid, strUUID, counter);
            last_sent_crc = compute_crc32(strUUID);
            char response[64];
            snprintf(response, sizeof(response), "%s %08X", strUUID, last_sent_crc);
            n = sendto(sock, response, strlen(response), 0, (struct sockaddr *)&from, fromlen);
            if (n < 0) error("sendto");
            printf("Sent: %s\n", response);
        } else if (strncmp(buf, "PREJETO", 7) == 0) {
            char received_crc[9];
            sscanf(buf + 8, "%8s", received_crc);
            unsigned int received_crc_int = strtoul(received_crc, NULL, 16);

            if (received_crc_int == last_sent_crc) {
                counter++;
                printf("CRC32 valid. Counter incremented.\n");
            } else {
                char error_msg[] = "NAPAKA 4900B4DB";
                n = sendto(sock, error_msg, strlen(error_msg), 0, (struct sockaddr *)&from, fromlen);
                if (n < 0) error("sendto");
                printf("Sent: %s\n", error_msg);
            }
        } else {
            char error_msg[] = "NEPREPOZNAVEN UKAZ";
            n = sendto(sock, error_msg, strlen(error_msg), 0, (struct sockaddr *)&from, fromlen);
            if (n < 0) error("sendto");
            printf("Sent: %s\n", error_msg);
        }
    }

    close(sock);
    return 0;
}
