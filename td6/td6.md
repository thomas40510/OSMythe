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

## Exercice 1

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


## Exercice 2
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

## Exercice 3
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
            noProcOut = rechercherProcessusASortir();
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
Interruption_horloge_swap:
    if (etat_swap == INACTIF){
        /* on réveille le swapper */
        etat_swap = ACTIF;
        V(reveil_swap);
    }
```

_NB_ : en réalité, on utilise des seuils pour déterminer le swapin et le swapout.

## Exercice 4
