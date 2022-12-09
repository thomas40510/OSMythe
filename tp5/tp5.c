#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <sched.h>

/* --- Exo 1 ---
 * Endless loop that prints the number of the CPU it is running on.
 */
void exo1(void){
    int i = 0;
    while(1){
        printf("CPU %d\n", sched_getcpu());
        i++;
    }
}





int main(int argc, char *argv[])
{
    exo1();
    return 0;
}
