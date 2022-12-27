#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "product.h"

/* Data structure */
Product prod;


void initPendingMult(Product *prod) {
    size_t i;
    for(i = 0; i < prod->size; i++){
        prod->pendingMult[i] = 1;
    }
}

int nbPendingMult(Product *prod) {
    size_t i;
    int nb = 0;
    for(i = 0; i < prod->size; i++){
        nb += prod->pendingMult[i];
    }
    return nb;
}

void wasteTime(unsigned long ms) {
    unsigned long t, t0;
    struct timeval tv;
    gettimeofday(&tv, (struct timezone *)0);
    t0 = tv.tv_sec*1000LU + tv.tv_usec/1000LU;
    do {
        gettimeofday(&tv, (struct timezone *)0);
        t = tv.tv_sec*1000LU + tv.tv_usec/1000LU;
    } while(t - t0 < ms);
}

void *mult(void *data) {
    size_t index = 0;
    size_t iter;

    /* Get the index of the thread, i.e. index of the first not pending mult */
    for(index = 0; index < prod.size; index++){
        if(prod.pendingMult[index] == 0){
            prod.pendingMult[index] = 1;
            break;
        }
    }

    fprintf(stderr, "[LOG/I]: mult currently running on CPU %d\n", sched_getcpu());
    fprintf(stderr, "Begin mult %zu\n", index);

    for(iter = 0; iter < prod.nbIter; iter++){
        /* Await mult clearance for new iteration */
        pthread_mutex_lock(&prod.mutex);
        while(prod.state != STATE_MULT || prod.pendingMult[index] == 0){
            pthread_cond_wait(&prod.cond, &prod.mutex);
        }
        pthread_mutex_unlock(&prod.mutex);

        fprintf(stderr, "--> Mult %zu\n", index);

        /* exec mult, simulate long operation and print result */
        prod.v3[index] = prod.v1[index] * prod.v2[index];

        wasteTime(200+rand()%200);
        fprintf(stderr, "<-- End mult %zu : %.3g*%.3g = %.3g \n",
                index, prod.v1[index], prod.v2[index], prod.v3[index]);

        /* mark current mult as done */
        pthread_mutex_lock(&prod.mutex);
        prod.pendingMult[index] = 0;


        /* Check if all the multiplications are done */
        if(nbPendingMult(&prod) == 0){
            fprintf(stderr, "All mult done\n");
            prod.state = STATE_ADD;  // If yes, we go to the addition state
            pthread_cond_broadcast(&prod.cond);
        }
        pthread_mutex_unlock(&prod.mutex);
    }
    fprintf(stderr, "Quitting mult %zu\n", index);
    return(data);
}

void *add(void *data) {
    size_t iter;
    fprintf(stderr, "Begin add()\n");
    for(iter = 0; iter < prod.nbIter; iter++){
        size_t index;
        /* Wait for add clearance */
        pthread_mutex_lock(&prod.mutex);
        while(prod.state != STATE_ADD){
            pthread_cond_wait(&prod.cond, &prod.mutex);
        }
        pthread_mutex_unlock(&prod.mutex);
        fprintf(stderr, "--> add\n");

        /* exec add, simulate long operation and print result */
        prod.result = 0.0;
        for(index = 0; index < prod.size; index++){
            prod.result += prod.v3[index];
        }
        wasteTime(100+rand()%100);

        fprintf(stderr, "<-- End add : %.3g\n", prod.result);

        /* give the print clearance */
        prod.state = STATE_PRINT;
        pthread_cond_broadcast(&prod.cond);
    }
    fprintf(stderr, "Quitting add()\n");
    return(data);
}

double *genVectors(double *v1, double *v2, int size) {
    size_t i;
    for(i = 0; i < size; i++){
        v1[i] = 10.0 * (.5- ((double)rand() / (double)RAND_MAX));
        v2[i] = 10.0 * (.5- ((double)rand() / (double)RAND_MAX));
    }
}

int main(int argc, char **argv){
    size_t i, it;
    pthread_t *multTh;
    size_t *multData;
    pthread_t addTh;
    void *threadReturnValue;

    /* Check args */
    if((argc <= 2) ||
       (sscanf(argv[1], "%zu", &prod.nbIter) != 1) ||
       (sscanf(argv[2], "%zu", &prod.size) != 1) ||
       (int)prod.nbIter <= 0 || (int)prod.size <= 0)
    {
        fprintf(stderr, "Usage: %s nbIter size\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "nbIter = %zu, size = %zu\n", prod.nbIter, prod.size);

    printf("%ld processeurs disponibles\n", sysconf(_SC_NPROCESSORS_ONLN));

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    if (sched_setaffinity(0, sizeof(cpu_set_t), &cpuset) == -1) {
        perror("sched_setaffinity");
        exit(EXIT_FAILURE);
    }

    printf("[LOG/I]: main() running on CPU %d\n", sched_getcpu());

    prod.state = STATE_WAIT;
    prod.pendingMult = (int *) malloc(prod.size * sizeof(int));

    /* Init mutex and cond */
    pthread_mutex_init(&prod.mutex, NULL);
    pthread_cond_init(&prod.cond, NULL);

    prod.mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    prod.cond = (pthread_cond_t) PTHREAD_COND_INITIALIZER;

    /* Dynamic allocation of vectors */
    prod.v1 = (double *) malloc(prod.size * sizeof(double));
    prod.v2 = (double *) malloc(prod.size * sizeof(double));
    prod.v3 = (double *) malloc(prod.size * sizeof(double));

    /* Allocating table for threads */
    multTh = (pthread_t *) malloc(prod.size * sizeof(pthread_t));

    /* Allocating table of mulData */
    multData = (size_t *) malloc(prod.size * sizeof(size_t));

    /* Init multData table */
    for(i = 0; i < prod.size; i++){
        multData[i] = i;
    }

    /* Create mult threads */
    for(i = 0; i < prod.size; i++){
        /* Balance on available CPUs */
        int cpu = i % sysconf(_SC_NPROCESSORS_ONLN);
        cpu_set_t multSet;
        CPU_ZERO(&multSet);
        CPU_SET(cpu, &multSet);
        pthread_create(&multTh[i], NULL, mult, &multData[i]);
        if (pthread_setaffinity_np(multTh[i], sizeof(cpu_set_t), &multSet) == -1) {
            perror("pthread_setaffinity_np");
            exit(EXIT_FAILURE);
        }
    }
    /* Create add thread */
    cpu_set_t addSet;
    CPU_ZERO(&addSet);
    CPU_SET(0, &addSet);
    pthread_create(&addTh, NULL, add, NULL);
    if (pthread_setaffinity_np(addTh, sizeof(cpu_set_t), &addSet) == -1) {
        perror("pthread_setaffinity_np");
        exit(EXIT_FAILURE);
    }

    srand(time((time_t *)0)); // Init random generator

    /* Main loop (while undone iterations remain) */
    for(it=0; it < prod.nbIter; it++){
        size_t j;

        /* gen random vectors */
        for(j = 0; j < prod.size; j++){
            prod.v1[j] = 10.0 * (.5- ((double)rand() / (double)RAND_MAX));
            prod.v2[j] = 10.0 * (.5- ((double)rand() / (double)RAND_MAX));
        }

        /* Allow mult threads to start */
        prod.state = STATE_MULT;
        pthread_cond_broadcast(&prod.cond);

        /* Await print clearance */
        while(prod.state != STATE_PRINT){
            pthread_cond_wait(&prod.cond, &prod.mutex);
        }

        /* Display current result */
        fprintf(stderr, "ITER=%zu, result=%.3g\n", it, prod.result);
    }

    /* Await termination of mult threads */
    for(i = 0; i < prod.size; i++){
        pthread_join(multTh[i], &threadReturnValue);
    }

    /* Await termination of add thread */
    pthread_join(addTh, &threadReturnValue);

    /* Destroy mutex and cond */
    pthread_mutex_destroy(&prod.mutex);
    pthread_cond_destroy(&prod.cond);

    /* Free memory */
    free(prod.v1);
    free(prod.v2);
    free(prod.v3);
    free(multTh);
    free(multData);
    free(prod.pendingMult);

    return(EXIT_SUCCESS);
}
