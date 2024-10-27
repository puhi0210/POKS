#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // For sleep

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




int main(){
    srand(time(NULL));
    int uuid[8];   // xxxxxxxx-xxxx-Mxxx-Nxxx-xxxxxxxxxxxx
    char strUUID[45];

    for(int i=0;i<=100;i++){
        generateUUID(uuid);
        uuidStr(uuid, strUUID, i);
        printf("%s\n", strUUID);
    }
    
    return 0;
}

