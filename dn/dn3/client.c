#include <stdio.h>    //printf
#include <string.h>   //strlen
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr
#include <unistd.h>
#include <time.h>

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

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in server;
    char buf[1024];
    char exitMSG[1024] = "EXIT";

    // Inicializacija CRC32 tabele
    init_crc32_table();

    // Ustvari socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(5095);

    // Poveži se s strežnikom
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");

    while (1) {
        printf("Please enter the message: ");
        bzero(buf, 1024);
        fgets(buf, 1024, stdin);
        buf[strcspn(buf, "\n")] = 0; // Odstrani newline znak
        printf("Send: %s\n", buf);

        if (send(sock, buf, 1024, 0) < 0) {
            perror("Sendto");
        }
        if (strncmp(buf, exitMSG, 4) == 0) {
            break;
        }

        if (recv(sock, buf, 1024, 0) < 0) {
            perror("recvfrom");
        }
        printf("Received: %s\n", buf);

        if (strncmp(buf, "EXIT", 4) == 0) {
            break;
        }

        // Če je odgovor UUID in CRC
        if (strchr(buf, ' ') != NULL) {
            char uuid_part[64], received_crc[9];
            sscanf(buf, "%63s %8s", uuid_part, received_crc);

            // Izračunaj CRC za UUID brez CRC kode
            unsigned int calculated_crc = compute_crc32(uuid_part);
            sleep(3);

            // Pošlji potrditev
            char confirmation[64];
            snprintf(confirmation, sizeof(confirmation), "PREJETO %08X", calculated_crc);
            if (send(sock, confirmation, strlen(confirmation), 0) < 0) {
                perror("Send confirmation");
            }
            printf("Sent confirmation: %s\n", confirmation);
        }
    }

    close(sock);
    return 0;
}
