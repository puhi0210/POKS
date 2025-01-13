#include <netdb.h>
#include <netinet/in.h>
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

void error(const char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {
    int sock, n;
    struct sockaddr_in server;
    socklen_t serverlen;
    char buf[1024], user_input[1024];
    char uuid_with_crc[64], received_crc[9];

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <hostname> <port>\n", argv[0]);
        exit(0);
    }

    init_crc32_table();

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) error("socket");

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    serverlen = sizeof(server);

    struct hostent *hp = gethostbyname(argv[1]);
    if (!hp) error("gethostbyname");
    bcopy(hp->h_addr, &server.sin_addr, hp->h_length);

    while (1) {
        printf("Enter command: ");
        bzero(user_input, 1024);
        fgets(user_input, 1024, stdin);
        user_input[strcspn(user_input, "\n")] = 0;  // Remove newline character

        n = sendto(sock, user_input, strlen(user_input), 0, (struct sockaddr *)&server, serverlen);
        if (n < 0) error("sendto");
        printf("Sent: %s\n", user_input);

        bzero(buf, 1024);
        n = recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&server, &serverlen);
        if (n < 0) error("recvfrom");
        printf("Received: %s\n", buf);

        if (strncmp(user_input, "GET", 3) == 0) {
            // Razčleni prejeto sporočilo
            sscanf(buf, "%63s %8s", uuid_with_crc, received_crc);

            // Izračunaj CRC za UUID brez CRC kode
            unsigned int calculated_crc = compute_crc32(uuid_with_crc);

            // Pošlji potrditev
            char confirmation[64];
            snprintf(confirmation, sizeof(confirmation), "PREJETO %08X", calculated_crc);
            n = sendto(sock, confirmation, strlen(confirmation), 0, (struct sockaddr *)&server, serverlen);
            if (n < 0) error("sendto");
            printf("Sent confirmation: %s\n", confirmation);
        } else if (strcmp(user_input, "X") == 0) {
            printf("Exiting...\n");
            break;
        }
    }

    close(sock);
    return 0;
}
