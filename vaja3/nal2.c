#include <stdio.h>
#include <pthread.h>  // Prevajamo z gcc nal1.c -o "nal1" -lpthread

void *fun(void *arg);

int stevec = 0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int main(){
    pthread_t nit1, nit2;

    pthread_create(&nit1, NULL, &fun, NULL);
    pthread_create(&nit2, NULL, &fun, NULL);

    pthread_join(nit1,NULL);
    pthread_join(nit2,NULL);

    printf("\nKonec. števec = %d\n", stevec);
    return 0;
}


void *fun(void *arg){
    pthread_t nit1 = pthread_self();
    int stevecint = 0;
    int i,n;

    for(i=0;i<100;i++){
        pthread_mutex_lock(&mutex1);
        stevecint = stevec;
        stevecint++;
        printf("\nNit %lu je povečala števec na %d", nit1, stevec);
        for(n=0;n<5000;n++){
            int a = 2*32/5;
        }
        stevec = stevecint;
        pthread_mutex_unlock(&mutex1);
    }
}
