#include <sys/types.h>     /* key_t  */
#include <sys/ipc.h>       /* ftok   */
#include <sys/sem.h>       /* semget, semctl, semop */
#include <sys/stat.h>      /* stat, struct stat  */
#include <stdlib.h>        /* malloc */
#include <stdio.h>         /* perror, printf */
#include <errno.h>         /* errno */
#include <unistd.h> 	   /*sleep*/

struct sembuf Ops[2];

/* retourne -1 en cas d'erreur */
int P(int semid, int noSem){
    int ok;

    Ops[0].sem_num = noSem;
    Ops[0].sem_op = -1;
    Ops[0].sem_flg = 0;

    ok = semop(semid, Ops, 1);
    return ok;
}

/* retourne -1 en cas d'erreur */
int V(int semid, int noSem){
    int ok;

    Ops[0].sem_num = noSem;
    Ops[0].sem_op = 1;
    Ops[0].sem_flg = 0;

    ok = semop(semid, Ops, 1);
    return ok;
}


int main (void){

    int donnee = 0;
    int mutex = 1;

    //recreate key
    key_t k;
    k = ftok("./tmp", 1);

    //open semaphore
    int semid;
    semid = semget(k, 0, 0);
    printf("semid = %d\n", semid);

    // read nbLecteurs from file
    FILE *n = fopen("nbLecteur.txt", "r");
    int nbLecteurs;
    fscanf(n, "%d", &nbLecteurs);

    /*
     * New reader
     */
    P(semid, mutex);
    nbLecteurs++;
    rewind(n);
    fprintf(n, "%d", nbLecteurs);
    if(nbLecteurs == 1){ // first reader takes the right to access data
        P(semid, donnee);
    }
    V(semid, mutex);

    /* Open file and read */
    FILE *f = fopen("file.txt", "r");
    char c;
    while((c = fgetc(f)) != EOF){
        printf("%c", c);
    }
    fclose(f);

    /*
     * End of reading
     */
    P(semid, mutex);
    nbLecteurs--;
    rewind(n);
    fprintf(n, "%d", nbLecteurs);
    fclose(n);
    if(nbLecteurs == 0){ // last reader releases the access to data
        V(semid, donnee);
    }
    V(semid, mutex);

    semctl(semid, mutex, IPC_RMID); // remove mutex

    return 0;

}