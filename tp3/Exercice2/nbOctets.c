#include <stdio.h>             /* nbOctets.c */
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "partage.h"

/* Decrire le handler de signal pour SIGUSR1 */
/* ========================================= */
void handler(int signum){
    printf("[I]: Signal %d reçu.\n", signum);
    exit(0);
}

/* ... */

/* Le main */
/* ======= */

int main(int argc,char **argv)
{
    pid_t  pidWC;
    pid_t  pidREAD;
    int    status;   /* Pour les waitpid                        */

    int    tube[2];
    FILE  *fIn;      /* Pour faire un fdopen : int -> FILE *    */

    struct sigaction action;

    Zone     z;
    int *ptDeb;      /* Un pointeur (int*) sur la zone debut    */

    char  *fileName=NULL;

    if (argc!=2) { fprintf(stderr,"Usage: %s fileName\n",argv[0]); return 1; }

    fileName=argv[1];

/* A cause de warnings lorsque le code n'est pas encore la ...*/

    (void)action; (void)fIn; (void)tube; (void)status; (void)pidREAD; (void)pidWC;

/* Gestion des signaux */
/* =================== */

    /* Preparation de la structure action pour recevoir le signal SIGUSR1 */

    action.sa_handler = handler;

    if(sigaction(SIGUSR1, &action, NULL) == -1){
        perror("sigaction");
        exit(1);
    }


/* Creation de la zone de memoire partagee */
/* ======================================= */

    /* Creer la zone de memoire partagee pouvant accueillir un int*/
    if(creerZonePartagee(sizeof(int), &z) == -1){
        perror("creerZonePartagee");
        exit(1);
    }

/* ... */

    ptDeb=(int*)z.debut;    /* *ptDeb <=> *((int*)z.debut) */


/* Creation du tube */
/* ================ */

    if(pipe(tube) == -1){
        perror("pipe");
        exit(1);
    }

/* Creation du processus qui fera le exec ...   */
/* ============================================ */

    pidWC=fork();

    if(pidWC == -1){
        perror("fork");
        exit(1);
    }

    /* redirect stdout to the pipe */
    if(pidWC == 0){
        close(tube[0]);
        dup2(tube[1], STDOUT_FILENO);
        close(tube[1]);
        execlp("wc", "wc", "-c", fileName, NULL);
        perror("execlp");
        exit(1);
    }

/* Creation du processus qui fera la lecture ...*/
/* ============================================ */

    pidREAD=fork();

    if(pidREAD == -1){
        perror("fork");
        exit(1);
    }

    /* redirect stdin to the pipe, read and put to shared memory, wait and kill father */
    if(pidREAD == 0){
        close(tube[1]);
        dup2(tube[0], STDIN_FILENO);
        close(tube[0]);
        fIn=fdopen(STDIN_FILENO, "r");
        fscanf(fIn, "%d", ptDeb);
        fclose(fIn);
        sleep(4);
        kill(getppid(), SIGUSR1);
        pause();
        exit(0);
    }

    /* Dans le processus cree :
       - Fermer le(s) descripteur(s) inutile(s) a cet enfant
       - Ouvrir un flux fIn sur la sortie du tube: fIn=fdopen(tube[0],"r");
       - Lire le resultat via le flux fIn et le mettre dans la memoire partagee
       - Fermer le flux fIn et le(s) descripteur(s) encore ouvert(s)
       - Attendre un peu pour que le pere puisse faire pause avant
       - Envoyer le signal SIGUSR1 au pere
    */

/* La suite du pere */
/* ================ */

    /* close useless descriptors */
    close(tube[0]);
    close(tube[1]);

    /* Attente d'un signal */
    pause();

    /* Recuperer le resultat dans la memoire partagee */
    printf("Nombre d'octets : %d\n", *ptDeb);

    /* Attendre le 1er enfant  */
    waitpid(pidWC, &status, 0);

    /* Attendre le 2eme enfant */
    waitpid(pidREAD, &status, 0);

    /* Supprimer la memoire partagee */
    supprimerZonePartagee(&z);

    return 0;
}
