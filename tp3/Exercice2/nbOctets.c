#include <stdio.h>             /* nbOctets.c */
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "partage.h"

/* Decrire le handler de signal pour SIGUSR1 */
/* ========================================= */

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

/* action.sa_handler = ... */

  /* Appel a l'appel systeme sigaction */

/* ... */

/* Creation de la zone de memoire partagee */
/* ======================================= */

/* ... */

 ptDeb=(int*)z.debut;    /* *ptDeb <=> *((int*)z.debut) */

/* Creation du tube */
/* ================ */

/* ... */

/* Creation du processus qui fera le exec ...   */
/* ============================================ */

/* pidWC=... */

   /* Dans le processus cree :
      - Rediriger la sortie standard vers le tube
      - Fermer le(s) descripteur(s) inutile(s) a cet enfant
      - Recouvrir par la commande ``wc'' 
   */

/* Creation du processus qui fera la lecture ...*/
/* ============================================ */

/* pidREAD=... */

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

  /* Fermer les descripteurs de tube inutiles au pere */

/* ... */

  /* Attente d'un signal */

/* ... */

  /* Recuperer le resultat dans la memoire partagee */

/* ... */

  /* Attendre le 1er enfant  */

/* ... */

  /* Attendre le 2eme enfant */

/* ... */

  /* Supprimer la memoire partagee */

/* ... */

 return 0;
}
