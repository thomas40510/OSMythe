#ifndef PARTAGE_H              /* partage.h */
#define PARTAGE_H

/* Structure zone representant une zone partagee entre processus ayant
   un lien de parente. Exemple: pere-fils
   => Utilisation d'une cle privee d'IPC Sys V                         */
typedef struct 
{
 int    id;
 void * debut;
 int    taille;
} Zone;

/* fonctions publiques */
extern int creerZonePartagee(int taille, Zone * zp);
extern int supprimerZonePartagee(Zone * zp);

#endif /* PARTAGE_H */
