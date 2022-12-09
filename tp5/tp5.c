#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <sched.h>
#include <unistd.h>


/* --- Exo 1 ---
 * Endless loop that prints the number of the CPU it is running on.
 */
void exo1(){
    int i = 0;
    while(1){
        printf("CPU %d\n", sched_getcpu());
        sleep(2);
        i++;
    }
}

/* --- Exo 2 ---
 * Execute a function on a specific CPU.
 */
void exo2(void){
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(1, &cpuset);
    sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
    exo1();
}


int main(int argc, char *argv[]){
    exo2();
    return 0;
}
