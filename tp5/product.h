/*
 * File product.c
 * --------------
 * Calculates the product of two n-size vectors with multithreading.
 */

/* Data types */
typedef enum{
    STATE_WAIT,
    STATE_MULT,
    STATE_ADD,
    STATE_PRINT
} State;

typedef struct{
    State state;
    int *pendingMult;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    size_t nbIter;
    size_t size;
    double *v1;
    double *v2;
    double *v3;
    double result;
} Product;

/* Functions */
void initPendingMult(Product *prod);

int nbPendingMult(Product *prod);

void wasteTime(unsigned long ms);

/********** Multiplication **********/
void *mult(void *data);

/********** Addition **********/
void *add(void *data);
