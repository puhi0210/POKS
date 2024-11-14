#include <stdio.h>
#include <unistd.h>

main(){
    printf("Proces ima: PID = %d in PPID = %d\n", getpid(), getppid());

    return 0;
}