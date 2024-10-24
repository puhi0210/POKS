#include <stdio.h>

int main(){
    char ime[50];
    char priimek[50];
    printf("Ime: ");
    fgets(ime, sizeof(ime), stdin);
    ime[strcspn(ime,"\n")] = '\0';
    printf("Priimek: ");
    fgets(priimek, sizeof(priimek), stdin);
    priimek[strcspn(priimek,"\n")] = '\0';
    
    printf("\nZdravo %s %s!\n", ime, priimek);
}