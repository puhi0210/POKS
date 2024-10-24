#include <stdio.h>

int main(){
    char ime[50];
    char priimek[50];
    int starost;
    printf("Ime: ");
    scanf("%s", ime);
    printf("Priimek: ");
    scanf("%s", priimek);
    printf("Starost: ");
    scanf("%d",&starost); // "&" dodamo, ker nismo definirali velikosti spremenljivke.

    printf("\nZdravo %s %s!\nStar si %d let.\n", ime, priimek, starost);        
    return 0;
}
