#include <stdio.h>
#include <unistd.h>

int parentOnlyFork(int iChild);


int main(){
    printf("Začetek. Proces ima: PID = %d in PPID = %d\n", getpid(), getppid());
    int otrok = getpid();
    otrok = parentOnlyFork(otrok);
    otrok = parentOnlyFork(otrok);
    otrok = parentOnlyFork(otrok);
    
    printf("Konec. Proces ima: PID = %d, PPID = %d, PID otroka = %d\n", getpid(), getppid(), otrok);
    sleep(1000);
    return 0;
}


int parentOnlyFork(int iChild){
    int oChild = 0;
    if(iChild != 0){
        oChild = fork();
    }
    return oChild;
}