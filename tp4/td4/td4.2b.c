/* ===================================================
 * TD4 : Threads / POSIX
 * Exercice 2.1 -- factorielle avec variable globale
 * TODO: make it work
 * ================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

int res;

sem_t mutex;

void *calculeFactorielle(void *ptrVoidVal){
    int val;
    if(ptrVoidVal == NULL){
        pthread_exit(NULL);
    } else{
        val = *(int*) ptrVoidVal;
    }

    // take semaphore
    sem_wait(&mutex);
    res = 1;
    while(val > 0){
        res *= val;
        val--;
    }
    // release semaphore
    sem_post(&mutex);
    pthread_exit(NULL);
}


int main(int argc, char *argv[]){
    int x = 7;
    int y = 10;

    // create semaphore
    pthread_t threadX;
    pthread_t threadY;

    int *ptrResX;
    int *ptrResY;

    if(pthread_create(&threadX, NULL, calculeFactorielle, &x) != 0){
        perror("pthread_create");
        exit(1);
    }

    if(pthread_create(&threadY, NULL, calculeFactorielle, &y) != 0){
        perror("pthread_create");
        exit(1);
    }

    pthread_join(threadX, (void**)&ptrResX);
    pthread_join(threadY, (void**)&ptrResY);

    printf("Factorielle de %d = %d\n", x, *ptrResX);
    printf("Factorielle de %d = %d\n", y, *ptrResY);

    // del semaphore
    sem_destroy(&mutex);
    return 0;
}