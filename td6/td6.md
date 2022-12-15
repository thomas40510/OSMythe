# OS TD n°6

```c
typedef struct processus{
    char *pt_mot_etat;
    int pid;
    int taille;
    int etat_uc;
    int etat_mem;
    char *pt_mem;
    char *pt_disque;
    time d_swap;
    int priorite;
    int proc_suiv;
} PROCESSUS, *PT_PROCESSUS;
```

## 1. SWAP
### Exercice 1

**Conditions logiques _swapin_** :
- CPU :
    - le processus n'est pas en cours d'exécution
    - le processus prêt ou bloqué
- Mémoire : 
    - le processus est non résident (sinon il est déjà en mémoire)

**Conditions _swapout_** :
- CPU : 
  - le processus n'est pas en cours d'exécution
  - le processus est bloqué ou prêt
- Mémoire : 
  - le processus est résident en mémoire


### Exercice 2
**Critères de priorisation du processus à _swapin_** :
1. état prêt (on est sûrs qu'il va s'exécuter)
   - priorité la plus élevée
     1) valeur de `d_swap` la plus faible (équité)
     2) taille du processus la plus faible (efficacité)
   
2. état bloqué
   1) taille la plus faible
   2) `d_swap` la plus faible
   3) priorité la plus élevée

**Critères de priorisation du processus à _swapout_** :
1. état bloqué
   1) taille la plus grande
   2) priorité la plus faible 

2. état prêt
   1) taille la plus grande
   2) priorité la plus faible

### Exercice 3
```c
main() {
    int noProcIn, noProcOut;
    sema reveil_swap = 0;
    int place_en_memoire = 1;
    while (1){
        /* la mémoire comporte une place libre */
        while (place_en_memoire){
            /* on cherche le processus à swapin (priorisation) */
            noProcIn = rechercherProcessusAEntrer();
            if (noProcIn < 0){
                /* pas de processus à swapin, on endort le swapper */
                etat_swap = INACTIF;
                P(reveil_swap);
            } else {
                /* on swapin le processus, on prend la place mémoire */
                if (swapin(noProcIn) != 0){
                    place_en_memoire = 0;
                }
            }
        }
        /* la mémoire est pleine */
        while (!place_en_memoire){
            /* on cherche le processus à swapout (priorisation) */
            noProcOut = rechercherProcessusARetirer();
            if (noProcOut < 0){
                /* pas de processus à swapout, on endort le swapper */
                etat_swap = INACTIF;
                P(reveil_swap);
            } else {
                /* on swapout le processus, on libère la place mémoire */
                swapout(noProcOut);
                place_en_memoire = 1;
                }
            }
        }
    }
}
```

```c
/* réveil périodique du swapper */
Interruption_horloge_swap:
    if (etat_swap == INACTIF){
        /* on réveille le swapper */
        etat_swap = ACTIF;
        V(reveil_swap);
    }
```

_NB_ : en réalité, on utilise des seuils pour déterminer le swapin et le swapout.

### Exercice 4
#### Fonctions à dispo:
```c
char *allouerZone(int taille);
int libererZone(int taille, char *pt_mem):

int lireDisque(char *pt_disque, char *pt_mem, int nb_octets);
int ecrireDisque(char *pt_disque, char *pt_mem, int nb_octets);

int RechercherProcessusAEntrer();
int RechercherProcessusARetirer();
```

#### Fonction `swapin`:
```c
int swapin(int noProc){
    taille = PROCESSUS[noProc].taille;
    /* allocation de la zone mémoire */
    int *zone;
    zone = allouerZone(taille);
    PROCESSUS[noProc].pt_mem = zone;
    if (PROCESSUS[noProc].pt_mem == NULL){
        /* pas de place en mémoire */
        return -1;
    }
    /* lecture espace de swap */
    if (lireDisque(PROCESSUS[noProc].pt_disque, 
                   PROCESSUS[noProc].pt_mem, taille) != 0){
        /* erreur de lecture */
        libererZone(taille, pt_mem);
        return -1;
    }
    /* mise à jour des champs du processus */
    PROCESSUS[noProc].etat_mem = RESIDENT;
    PROCESSUS[noProc].pt_mem = pt_mem;
    PROCESSUS[noProc].d_swap = now();
    return 0;
}
```

#### Fonction `swapout`:
```c
int swapout(int noProc){
    int taille = PROCESSUS[noProc].taille;
    /* écriture sur disque */
    if (ecrireDisque(PROCESSUS[noProc].pt_disque, 
                     PROCESSUS[noProc].pt_mem, taille) != 0){
        /* erreur d'écriture */
        return -1;
    }
    /* libération de la zone mémoire */
    libererZone(taille, PROCESSUS[noProc].pt_mem);
    /* mise à jour des champs du processus */
    PROCESSUS[noProc].etat_mem = NON_RESIDENT;
    PROCESSUS[noProc].pt_mem = NULL;
    PROCESSUS[noProc].d_swap = now();
    return 0;
}
```

## 2. Pagination
> - Taille des pages : 512 octets
> - Nombre de pages réelles : 16

Processus P1 : 

|  N° page virtuelle  | N° page réelle | Présent en mémoire  |
|:-------------------:|:--------------:|:-------------------:|
|          0          |       2        |          1          |
|          1          |       3        |          1          |
|          2          |       0        |          1          |
|          3          |       2        |          0          |
|          4          |       1        |          1          |
|          5          |       3        |          0          |


### Exercice 5
1) on peut imaginer la structure d'adressage suivante :
    > - 4 bits pour le numéro de page
    > - 9 bits pour l'offset
    
    On a donc une adresse physique de 13 bits.
2) Espace mémoire virtuel occupé par P1 : 
   > 6 pages * 512 octets = 3072 octets
3) Espace mémoire réel occupé par P1 :
    > 4 pages * 512 octets = 2048 octets
   
### Exercice 6
    Adresse virtuelle de P1 : 2058
1. Adresse virtuelle (no page, déplacement) :
    > 2058 = 2048 + 10 = 4 * 512 + 10, donc @P1 = (4, 10)
   
2. Adresse réelle :
    > @V = (4, 10) —> @P = (1, 10)
   
3. Valeur de l'adresse réelle :
    > @P(1, 10) = 1 * 512 + 10 = 522, donc @R = 522
   

## 3. Segmentation
### Table de segments

| Segment | Base | Longueur |
|:-------:|:----:|:--------:|
|    0    | 540  |   234    |
|    1    | 1254 |   128    |
|    2    |  54  |   328    |
|    3    | 2048 |   1024   |
|    4    | 976  |   200    |

### Exercice
* (0, 128) = 540 + 128 = 668
* (1, 99) = 1254 + 99 = 1353
* (1, 100) = 1254 + 100 = 1354
* (2, 465) = 54 + 465 = 519
* (3, 888) = 2048 + 888 = 2936
* (4, 100) = 976 + 100 = 1076
* (4, 344) = 976 + 344 = 1320
