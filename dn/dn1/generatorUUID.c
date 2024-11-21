#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // For sleep

unsigned int crc32_table[256];

void generateUUID(int *uuid){
    for(int i=0; i<8; i++){
        switch (i){
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

void uuidStr(int *uuidInt, char *uuidStr, int uuidNum){
    snprintf(uuidStr, 46, "%08d-%04x%04x-%04x-%04x-%04x-%04x%04x%04x", uuidNum, uuidInt[0],uuidInt[1],uuidInt[2],uuidInt[3],uuidInt[4],uuidInt[5],uuidInt[6],uuidInt[7]);
}

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




int main(){
    srand(time(NULL));
    init_crc32_table();
    int uuid[8];   // xxxxxxxx-xxxx-Mxxx-Nxxx-xxxxxxxxxxxx
    char strUUID[46];
    unsigned int crc;

    for(int i=0;i<=100;i++){
        generateUUID(uuid);
        uuidStr(uuid, strUUID, i);
        crc = compute_crc32(strUUID);
        printf("%s %08x\n", strUUID, crc);
    }
    
    return 0;
}

