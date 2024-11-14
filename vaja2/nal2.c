#include <stdio.h>
#include <unistd.h>

int main(){
    printf("Začetek. Proces ima: PID = %d in PPID = %d\n", getpid(), getppid());
    int otrok = fork();
    if(otrok){
        printf("Jaz sem starš.\n");
    }else{
        printf("Jaz sem otrok.\n");
    }
    printf("Konec. Proces ima: PID = %d, PPID = %d, PID otroka = %d\n", getpid(), getppid(), otrok);
    sleep(1);
    return 0;
}