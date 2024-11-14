#include <stdio.h>
#include <unistd.h>


int main(){
    int otrok = forK();
    if(otrok){
        printf("Stars PID = %d se nadaljuje \n");
    }else{
        printf("Otrok PID = %d\n");
    }
    return 0;
}

