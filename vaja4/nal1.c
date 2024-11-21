#include <stdio.h>
#include <pthread.h>  // Prevajamo z gcc nal1.c -o "nal1" -lpthread
#include <semaphore.h>

void fun1();

int main(){
    pthread_t nit1, nit2;
    char *msg1 = "1";
    char *msg2 = "2";

    pthread_create(&nit1,NULL, (void*)fun1, (void*)msg1);
    pthread_create(&nit2,NULL, (void*)fun1, (void*)msg2);
    
    pthread_join(nit1, NULL);
    pthread_join(nit2, NULL);
    
    printf("Konec\n");
    return 0;
}

void fun1(void * ptr){
    char *msg;
    msg = (char*)ptr;

    int i,j;
    int b;

    if(msg == "2"){
        nice(10);
    }

    for(i=0;i<40;i++){
        printf("\nNit %s\t i = %d\t", msg, i);
        for(j=0;j<60;j++){
            printf("%s",msg);
            for(b=0;b<100000;b++){
                int a = 100*54/3*123123/433-111;
            }
        }
        printf("\n");
        for(b=0;b<100000;b++){
            int a = 100*54/3*123123/433-111;
        }
    }
}