#include <sys/types.h>     /* key_t  */
#include <sys/ipc.h>       /* ftok   */
#include <sys/sem.h>       /* semget, semctl, semop */
#include <sys/stat.h>      /* stat, struct stat  */
#include <stdlib.h>        /* malloc */
#include <stdio.h>         /* perror, printf */
#include <errno.h>         /* errno */
#include <unistd.h> 	   /*sleep*/

// Ce programme doit être dupliqué pour chaque processus voulant établir un RDV
// vous pouvez nommer le programme du premier processus RDV1.C et celui du 2nd RDV2.C par exemple
// Les 2 programmes doivent être compilés séparément et exécutés séparément sur le shell


/* retourne -1 en cas d'erreur */
int P(int semid, int noSem){
	struct sembuf Ops[1];
	int ok;
	
	// Q- donner les 3 éléments de la structure Ops pour
    // réaliser l'opération (voir le cours)
	// Ops[0].sem_num = ...; ...
    Ops[0].sem_num = noSem;
    Ops[0].sem_op = -1;
    Ops[0].sem_flg = 0;
	
	// Q- faire appel à la fonction semop pour réaliser l'opération P, la variable OK récupère la valeur de retour
    ok = semop(semid, Ops, 1);
    return ok;
}

/* retourne -1 en cas d'erreur */
int V(int semid, int noSem){
	struct sembuf Ops[1];
	int ok;
	
	// Q- donner les 3 éléments de la structure Ops pour
    // réaliser l'opération (voir le cours)
	// Ops[0].sem_num = ...; ...
    Ops[0].sem_num = noSem;
    Ops[0].sem_op = 1;
    Ops[0].sem_flg = 0;
	
	// Q- faire appel à la fonction semop pour réaliser l'opération V, la variable OK récupère la valeur de retour
    ok = semop(semid, Ops, 1);
    return ok;
}


int main (void){ // à compléter sans oublier de supprimer l'ensemble des sémaphores

	// Q- Il faut d'abord recréer la clé (voir sema.c)
    int k;
    k = execl("./sema", "sema", "/tmp", "2", "1", NULL);
	
	// Q- il faut ensuite ouvrir le semaphore avec semget, à part la clé,
    // les autres arguments doivent être à zéro
	// car il ne s'agit pas d'une création, mais d'une ouverture
    int semid;
    semid = semget(k, 0, 0);
    printf("semid = %d\n", semid);

    printf("I am RDV2, I'm going to sleep\n");

    // Q- faire l'appel à sleep() afin d'avoir des attentes de différentes durées pour les 2 processus
    sleep(4);


	// Q- faire appel à P et à V (voir le TD)
    int v = V(semid, 2);
    printf("V = %d\n", v);
    P(semid, 1);

	
	// appeler la fonction de RDV, un printf est suffisant.

    printf("RDV2\n");
    semctl(semid, 0, IPC_RMID);
    return 0;

}