/* ===================================================
 * TD4 : Threads / POSIX
 * Exercice 2
 * ================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


void *calculeFactorielle(void *ptrVoidVal){
    int val;
    if(ptrVoidVal == NULL){
        pthread_exit(NULL);
    } else{
        val = *(int*) ptrVoidVal;
    }
    int *res = malloc(sizeof(int));
    *res = 1;
    while(val > 0){
        *res *= val;
        val--;
    }
    pthread_exit(res);
}


int main(int argc, char *argv[]){
    int x = 7;
    int y = 10;

    pthread_t threadX;
    pthread_t threadY;

    if(pthread_create(&threadX, NULL, calculeFactorielle, &x) != 0){
        perror("pthread_create");
        exit(1);
    }
    if(pthread_create(&threadY, NULL, calculeFactorielle, &y) != 0){
        perror("pthread_create");
        exit(1);
    }
    int *ptrResX;
    int *ptrResY;

    pthread_join(threadX, (void**)&ptrResX);
    pthread_join(threadY, (void**)&ptrResY);

    printf("Factorielle de %d = %d\n", x, *ptrResX);
    printf("Factorielle de %d = %d\n", y, *ptrResY);

    free(ptrResX);
    free(ptrResY);

    return 0;
}