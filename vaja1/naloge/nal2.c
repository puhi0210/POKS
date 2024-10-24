#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    printf("Ugani število med 1 in 100!");
    srand(time(NULL));
    int stevilo = rand()%100+1;
    int u;
    int poizkusi = 6;
    do{
        printf("\nVnesi število: ");
        scanf("%d", &u);
        if(u<stevilo){
            printf("Iskano število je večje od tvojega poizkusa");
        }
        else if(u>stevilo){
            printf("Iskano število je manjše od tvojega poizkusa");
        }
        else{
            printf("Čestitam. Uganil si!");
            return 0;
        }
        poizkusi--;
    }while (poizkusi);
    printf("Nisi uganil števila. pravilen odgovor je %d", stevilo);
    return 0;
    

}