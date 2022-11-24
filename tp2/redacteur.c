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

    //recreate key
    key_t k;
    k = ftok("./tmp", 1);

    //open semaphore
    int semid;
    semid = semget(k, 0, 0);
    printf("semid = %d\n", semid);

    // P and V
    P(semid, donnee);
    //write in file file.txt
    FILE *f = fopen("file.txt", "w");
    fprintf(f, "Hello World!");
    fclose(f);
    sleep(5);
    V(semid, donnee);


    semctl(semid, donnee, IPC_RMID);

    return 0;

}