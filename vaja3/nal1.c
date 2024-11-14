#include <stdio.h>
#include <pthread.h>  // Prevajamo z gcc nal1.c -o "nal1" -lpthread

void *sum1();
void *sum2();
void *sum3();

int a=1;
int b=2;
int c=3;
int d=0;
int e=4;
int f=8;

int ab=0;
int cd=0;
int ef=0;


int main(){
    pthread_t nit1, nit2, nit3;
    int R = 0;

    pthread_create(&nit1, NULL, &sum1, NULL);
    pthread_create(&nit2, NULL, &sum2, NULL);
    pthread_create(&nit3, NULL, &sum3, NULL);
    
    pthread_join(nit1, NULL);
    pthread_join(nit2, NULL);
    pthread_join(nit3, NULL);

    R = ab * cd * ef;

    printf("\nRezultat (a+b) + (c+d) + (e+f) = %d\n", R);
}


void *sum1(){
    pthread_t nit1 = pthread_self(); // Unsignd long
    ab = a + b;
    printf("\nNIT ID: %lu\nRezultat A + B = %d", nit1, ab);
}

void *sum2(){
    pthread_t nit2 = pthread_self(); // Unsignd long
    cd = c + d;
    printf("\nNIT ID: %lu\nRezultat C + D = %d", nit2, cd);
}

void *sum3(){
    pthread_t nit3 = pthread_self(); // Unsignd long
    ef = e + f;
    printf("\nNIT ID: %lu\nRezultat E + F = %d", nit3, ef);
}