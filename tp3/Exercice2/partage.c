#include <stdio.h>             /* partage.c */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "partage.h"

/*--- 
  Pour memoire :
 ==============

  Structure zone representant une zone partagee entre processus ayant
   un lien de parente. Exemple: pere-fils
   => Utilisation d'une cle privee d'IPC Sys V

typedef struct 
{
 int    id;
 void * debut;
 int    taille;
} Zone;
---*/

/* ---------------------------------------------------
   creerZonePartagee(int taille, Zone *zp)

   Action   : creer une zone de memoire partagee grace
              aux IPC SysV, accesible un lecture/ecriture
              par le createur. La taille est en octets.

   Remarque : cette zone est heritee par un processus fils

   Retourne : -1 en cas d'erreur, 0 sinon 
              remplit la structure zone passee en argument
 --------------------------------------------------- */

int creerZonePartagee(int taille, Zone * zp) 
{
 if (!zp) { fprintf(stderr,"Erreur: zp==NULL dans creerZonePartagee\n");
            return -1;
 }

 if ((zp->id=shmget(IPC_PRIVATE, taille, 0600|IPC_CREAT))==-1)
      { perror("shmget (creerZonePartagee)"); return -1; }

 if ((zp->debut=shmat(zp->id, 0, 0))==(void*)-1)
      { perror("shmat (creerZonePartagee)"); 
        shmctl(zp->id, IPC_RMID, NULL);
        return -1; }

 zp->taille=taille;

 return 0;
}

/* ------------------------------------------------------
   int supprimerZonePartagee(Zone *zp)

   Libere la zone partagee

   Retourne -1 en cas d'erreur, 0 sinon 
  ---------------------------------------------------- */

int supprimerZonePartagee(Zone * zp)
{
 if (!zp) { fprintf(stderr,"Erreur: zp==NULL dans supprimerZonePartagee\n");
            return -1;
 }

 if (shmdt(zp->debut)==-1) 
     { perror("shmdt (supprimerZonePartagee)"); return -1; }

 if (shmctl(zp->id, IPC_RMID, NULL)==-1) 
     { perror("shmctl (supprimerZonePartagee)"); return -1; }

 return 0;
}
