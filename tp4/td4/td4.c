/* ===================================================
 * TD4 : Threads / POSIX
 * ================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NB_THR 5

/* Exercice 1 */

void *thread_function(void *arg) {
    int val = *(int*) arg;
    while(1){
        fprintf(stderr, "New thread with arg %d\n", val);
        sleep(1);
    }
}

void *thread_function_exit(void *arg) {
    int val = *(int*) arg;
    int i = 1;
    while(i != 5){
        fprintf(stderr, "New thread with arg %d\n", val);
        sleep(1);
        i++;
    }
    exit(0);
}

void *thread_function_texit(void *arg) {
    int val = *(int*) arg;
    while(1){
        fprintf(stderr, "New thread with arg %d\n", val);
        pthread_exit(NULL);
    }
}


int main_init(void){ //2.1
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

int main_with_loop(void){ // 2.2
    /* Create 5 threads and send them their number */
    pthread_t thr;

    int vals[NB_THR]; // allows to send different values to each thread
    for (int i = 0; i < NB_THR; i++) {
        vals[i] = i;
    }

    for(int i = 0; i < NB_THR; i++){
        if(pthread_create(&thr, NULL, thread_function, (void*) &vals[i]) != 0){
            perror("pthread_create");
            exit(1);
        }
    }

    while(1){
        fprintf(stderr, "Main thread\n");
        sleep(1);
    }
}

int main_with_thread_exit(void){ // 2.3.1/2
    /*
     * Create 5 threads and send them their number
     * Each thread will exit after 5 iterations
     * Case 1: exit(0) in thread_function_exit, main thread will exit too
     * Case 2: pthread_exit(NULL) in thread_function_texit, main thread will continue
     */
    pthread_t thr;

    int vals[NB_THR]; // allows to send different values to each thread
    for (int i = 0; i < NB_THR; i++) {
        vals[i] = i;
    }

    for(int i = 0; i < NB_THR; i++){
        if(pthread_create(&thr, NULL, thread_function_texit, (void*) &vals[i]) != 0){
        //if(pthread_create(&thr, NULL, thread_function_exit, (void*) &vals[i]) != 0){
            perror("pthread_create");
            exit(1);
        }
    }

    while(1){
        fprintf(stderr, "Main thread\n");
        sleep(1);
    }
}

int main_with_exit(void){ // 2.3.3
    pthread_t thr;

    int vals[NB_THR]; // allows to send different values to each thread
    for (int i = 0; i < NB_THR; i++) {
        vals[i] = i;
    }

    for(int i = 0; i < NB_THR; i++){
        if(pthread_create(&thr, NULL, thread_function, (void*) &vals[i]) != 0){
            perror("pthread_create");
            exit(1);
        }
    }

    while(1){
        fprintf(stderr, "Main thread\n");
        exit(0);
    }

}


int main(void){
    main_with_thread_exit();
    return 0;
}
