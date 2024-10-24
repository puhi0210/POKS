#include <stdio.h>

int main(){
    int zasluzek;
    int dohodnina;
    printf("\nIzračun dohodnine. Vnesi svoj letni zaslužek v €: ");
    scanf("%d", &zasluzek);

    if(zasluzek<=8500){
        dohodnina = zasluzek * 0.16;
    }else if(zasluzek>8500 && zasluzek<=25000){
        dohodnina = (zasluzek - 8500) * 0.26 + 1360; // par razredov se manka
    }else{
        dohodnina = (zasluzek - 72000) * 0.50 + 22480;
    }

    printf("Plačati moraš %d€ dohodnine.", dohodnina);
    return 0;

}