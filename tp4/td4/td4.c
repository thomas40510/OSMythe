/* ===================================================
 * TD4 : Threads / POSIX
 * ================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/* Exercice 1 */

void *thread_function(void *arg) {
    while(1){
        fprintf(stderr, "New thread with arg %d\n", arg);
        sleep(1);
    }
}

int main_with_loop(void){
    /* Create 5 threads and send them their number */
    pthread_t thr;

    for(int i = 0; i < 5; i++){
        if(pthread_create(&thr, NULL, thread_function, i+1) != 0){
            perror("pthread_create");
            exit(1);
        }
    }

    while(1){
        fprintf(stderr, "Main thread\n");
        sleep(1);
    }
}

int main_init(void){
    /* Create a thread */
    pthread_t thr;

    if(pthread_create(&thr, NULL, thread_function, NULL) != 0){
        fprintf(stderr, "Error while creating thread\n");
        exit(EXIT_FAILURE);
    }

    while(1){
        fprintf(stderr, "Main thread\n");
        sleep(1);
    }

}

int main(void){
    main_with_loop();
    return 0;
}
