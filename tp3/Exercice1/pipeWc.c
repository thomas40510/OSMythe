#include <stdio.h>             /* pipeWc.c */
#include <string.h>
#include <unistd.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define TAILLE 256

void fils(int tube[2])
{
 dup2(tube[0],STDIN_FILENO);     /* tube[0] => Entree standard         */

 close(tube[0]);  /* Il est donc maintenant possible de fermer tube[0] */

 close(tube[1]);  /* ESSENTIEL : pour la fin du tube                   */

 execlp("wc","wc","-l",NULL);

 perror("execlp");
}

void pere(int tube[2])
{
 int   ok = 0 ;
 char  ch[TAILLE];

 close(tube[0]);       /* Le pere ne lit pas dans le tube */

 printf("Entrez des lignes et terminez par fin : \n");

 fgets(ch,TAILLE,stdin);

 while (strcmp(ch,"fin\n")!=0)   /* fgets : \n a la fin ! */
 {
   ok = write(tube[1],ch,strlen(ch)+1);
   if (ok==-1) { perror("write"); exit(EXIT_FAILURE); }
   fgets(ch,TAILLE,stdin);
 }

 close(tube[1]);     /* Fermeture du tube ... FONDAMENTAL */

 wait(NULL);
}

int main(void)
{
 int   tube[2];

 pid_t pid = 0;

 if (pipe(tube) != 0) { perror("pipe"); exit(EXIT_FAILURE); }

 pid = fork();

 switch (pid)
 {
  case -1 : perror("fork");
            exit(EXIT_FAILURE);
            break;
  case  0 : fils(tube);
            break;
  default : pere(tube);
            break;
 }
 
 return EXIT_SUCCESS;
}
