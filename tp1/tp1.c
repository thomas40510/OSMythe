//
// Created by Thomas Prévost on 04/11/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <libc.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>


void exo1(){
    int code_retour;
    printf("début du fork()\n");
    code_retour = fork();
    switch (code_retour){
        case -1:
            printf("erreur de fork()\n");
            break;
        case 0:
            printf("je suis le fils\n");
            break;
        default:
            printf("je suis le père\n");
            printf("je viens de créer le processus fils de pid %d\n", code_retour);
            break;
    }
    printf("code retour de fork() = %d\n", code_retour);
    printf("fin du fork()\n");
}

void exo2(){
    /*
     * On crée un nouveau processus et affiche les infos du père et du fils
     */
    int processus;
    processus = fork();
    if (processus == 0){
        printf("====================================\n");
        printf("👶 je suis le fils 👶\n");
        printf("mon pid est %d\n", getpid());
        printf("mon père a pour pid %d\n", getppid());
        printf("mon répertoire de travail est %s\n", getcwd(NULL, 0));
        printf("mon propriétaire réel est %d\n", getuid());
        printf("mon propriétaire effectif est %d\n", geteuid());
        printf("mon groupe réel est %d\n", getgid());
        printf("mon groupe effectif est %d\n", getegid());
    } else {
        printf("====================================\n");
        printf("🎅 Je suis le père 🎅\n");
        printf("mon pid est %d\n", getpid());
        printf("mon père a pour pid %d\n", getppid());
        printf("mon répertoire de travail est %s\n", getcwd(NULL, 0));
        printf("mon propriétaire réel est %d\n", getuid());
        printf("mon propriétaire effectif est %d\n", geteuid());
        printf("mon groupe réel est %d\n", getgid());
        printf("mon groupe effectif est %d\n", getegid());
    }
}

void exo3(){
    /*
     * On affiche le père et le fils, en laissant le père mourir avant le fils
     * > le fils a pour père le processus de pid 1 (init).
     */
    int processus;
    processus = fork();
    if (processus == 0){
        printf("====================================\n");
        printf("👶 je suis le fils 👶\n");
        printf("mon pid est %d\n", getpid());
        printf("mon père a pour pid %d\n", getppid());
        printf("mon répertoire de travail est %s\n", getcwd(NULL, 0));
        printf("mon propriétaire réel est %d\n", getuid());
        printf("mon propriétaire effectif est %d\n", geteuid());
        printf("mon groupe réel est %d\n", getgid());
        printf("mon groupe effectif est %d\n", getegid());
        sleep(5);
    } else {
        printf("====================================\n");
        printf("🎅 Je suis le père 🎅\n");
        printf("mon pid est %d\n", getpid());
        printf("mon père a pour pid %d\n", getppid());
        printf("mon répertoire de travail est %s\n", getcwd(NULL, 0));
        printf("mon propriétaire réel est %d\n", getuid());
        printf("mon propriétaire effectif est %d\n", geteuid());
        printf("mon groupe réel est %d\n", getgid());
        printf("mon groupe effectif est %d\n", getegid());
    }
}

void exo4(){
    /* On ouvre un fichier
     * 1) le fils écrit puis s'endort
     * 2) le père dort puis lit, écrit et se termine
     * 3) le fils se réveille, lit et se termine
     */
    int fd;
    fd  = open("../toto.txt", O_RDWR | O_CREAT, 0666);
    printf("fd = %d\n", fd);
    int processus;
    char *buf = malloc(4*sizeof (char));
    printf("file descriptor = %d\n", fd);

    processus = fork();
    if (processus == 0){
        printf("====================================\n");
        printf("👶 je suis le fils 👶\n");
        write(fd, "FILS", 4);
        sleep(2);
        read(fd, buf, 4);
        printf("le fils lit : %s\n", buf);
    } else {
        sleep(1);
        printf("====================================\n");
        printf("🎅 Je suis le père 🎅\n");
        read(fd, buf, 4);
        printf("le père lit : %s\n", buf);
        write(fd, "PERE", 4);
    }
    close(fd);
    free(buf);
}

void exo5(){
    int processus;
    processus = fork();
    printf("%d", processus);
    if (processus == 0){
        printf("====================================\n");
        printf("👶 je suis le fils 👶\n");
        printf("mon pid est %d\n", getpid());
        printf("mon père a pour pid %d\n", getppid());
        printf("mon répertoire de travail est %s\n", getcwd(NULL, 0));
        printf("mon propriétaire réel est %d\n", getuid());
        printf("mon propriétaire effectif est %d\n", geteuid());
        printf("mon groupe réel est %d\n", getgid());
        printf("mon groupe effectif est %d\n", getegid());
        sleep(5);
        exit(0);
    } else {
        printf("====================================\n");
        printf("🎅 Je suis le père 🎅\n");
        printf("mon pid est %d\n", getpid());
        printf("mon père a pour pid %d\n", getppid());
        printf("mon répertoire de travail est %s\n", getcwd(NULL, 0));
        printf("mon propriétaire réel est %d\n", getuid());
        printf("mon propriétaire effectif est %d\n", geteuid());
        printf("mon groupe réel est %d\n", getgid());
        printf("mon groupe effectif est %d\n", getegid());
        int *terminaison = malloc(sizeof(int));
        pid_t res = waitpid(processus, terminaison, WCONTINUED);
        // > res = 0 si option WNOHANG (fils non terminé)
        // > res = pid du fils si WCONTINUED (attend d'avoir fils terminé)
        printf("/// waitpid a retourné %d ///\n", res);
        printf(" avec la terminaison %d\n", *terminaison);
        free(terminaison);
        exit(0);
    }
}

void exo6(){
    /*
     * On lance un processus fils, on affiche les infos du père et du fils
     * puis le père attend la fin du fils
     * avant d'afficher les infos renvoyées par waitpid
     */
    int processus;
    processus = fork();
    printf("%d", processus);
    if (processus == 0){
        printf("====================================\n");
        printf("👶 je suis le fils 👶\n");
        printf("mon pid est %d\n", getpid());
        printf("mon père a pour pid %d\n", getppid());
        printf("mon répertoire de travail est %s\n", getcwd(NULL, 0));
        printf("mon propriétaire réel est %d\n", getuid());
        printf("mon propriétaire effectif est %d\n", geteuid());
        printf("mon groupe réel est %d\n", getgid());
        printf("mon groupe effectif est %d\n", getegid());
        sleep(3);
        exit(3);
    } else {
        printf("====================================\n");
        printf("🎅 Je suis le père 🎅\n");
        printf("mon pid est %d\n", getpid());
        printf("mon père a pour pid %d\n", getppid());
        printf("mon répertoire de travail est %s\n", getcwd(NULL, 0));
        printf("mon propriétaire réel est %d\n", getuid());
        printf("mon propriétaire effectif est %d\n", geteuid());
        printf("mon groupe réel est %d\n", getgid());
        printf("mon groupe effectif est %d\n", getegid());
        int *terminaison = malloc(sizeof(int));
        pid_t res = waitpid(processus, terminaison, WCONTINUED);
        printf("/// waitpid a retourné %d ///\n", res);
        printf("avec la terminaison %d\n", *terminaison);
        printf("qui s'interprète comme exit code %d\n", WEXITSTATUS(*terminaison));
        free(terminaison);
        exit(0);
    }
}

void exo6b(){
    /*
     * Même principe, mais cette fois on laisse dormir le fils
     * et on le tue à la main dans un shell avec kill -15 pid_fils
     * et le père affiche le résultat de waitpid
     *
     * > waitpid est bien interprété comme l'exit code qu'on a donné (15)
     * > En faisant la même chose avec kill -9 pid_fils, on a bien 9 comme exit code
     */
    int processus;
    processus = fork();
    printf("%d", processus);
    if (processus == 0){
        printf("====================================\n");
        printf("👶 je suis le fils 👶\n");
        printf("mon pid est %d\n", getpid());
        printf("mon père a pour pid %d\n", getppid());
        printf("mon répertoire de travail est %s\n", getcwd(NULL, 0));
        printf("mon propriétaire réel est %d\n", getuid());
        printf("mon propriétaire effectif est %d\n", geteuid());
        printf("mon groupe réel est %d\n", getgid());
        printf("mon groupe effectif est %d\n", getegid());
        sleep(20);
        exit(3);
    } else {
        printf("====================================\n");
        printf("🎅 Je suis le père 🎅\n");
        printf("mon pid est %d\n", getpid());
        printf("mon père a pour pid %d\n", getppid());
        printf("mon répertoire de travail est %s\n", getcwd(NULL, 0));
        printf("mon propriétaire réel est %d\n", getuid());
        printf("mon propriétaire effectif est %d\n", geteuid());
        printf("mon groupe réel est %d\n", getgid());
        printf("mon groupe effectif est %d\n", getegid());
        int *terminaison = malloc(sizeof(int));
        pid_t res = waitpid(processus, terminaison, WCONTINUED);
        printf("/// waitpid a retourné %d ///\n", res);
        printf("avec la terminaison %d\n", *terminaison);
        printf("qui s'interprète comme exit code %d\n", WEXITSTATUS(*terminaison));
        free(terminaison);
        exit(0);
    }
}

void exo7(){
    /*
     * On crée 2 processus fils, on affiche les infos du père et des fils
     * puis on attend la mort du second fils
     * en affichant les infos renvoyées par waitpid
     * > waitpid renvoie bien les infos du second fils mort
     */
    int processus, processus2;
    processus = fork();
    processus2 = fork();
    if(processus == 0){
        printf("====================================\n");
        printf("je suis le fils 1\n");
        printf("mon pid est %d\n", getpid());
        printf("mon père a pour pid %d\n", getppid());
        sleep(5);
        printf("je suis le fils 1 et je meurs\n");
        exit(1);
    } else if (processus2 == 0){
        printf("====================================\n");
        printf("je suis le fils 2\n");
        printf("mon pid est %d\n", getpid());
        printf("mon père a pour pid %d\n", getppid());
        sleep(10);
        printf("je suis le fils 2 et je meurs\n");
        exit(2);
    } else {
        printf("====================================\n");
        printf("je suis le père\n");
        int *terminaison = malloc(sizeof(int));
        pid_t res = waitpid(processus2, terminaison, WCONTINUED);
        printf("/// waitpid a retourné %d ///\n", res);
        printf("avec la terminaison %d\n", *terminaison);
        printf("qui s'interprète comme exit code %d\n", WEXITSTATUS(*terminaison));
        free(terminaison);

        exit(0);
    }
}

void exo9(){
    /*
     * On étudie le comportement de exec
     * > exécute la commande donnée, puis quitte le programme.
     * > Si la commande échoue, continue l'exécution du programme.
     */
    char buf[80];

    printf("[%d]-- début du text exec()\n", getpid());
    sprintf(buf, "--pid=[%d]", getpid());
    //execl("/bin/echo", "echo",
    //      "Execution", "d'un test exec", buf, NULL);
    execl("/bin/ech", "echo",
          "Execution", "d'un test exec", buf, NULL);
    printf("[%d]-- Echec de la fonction execl() \n", getpid());
    printf("[%d]-- Fin du test exec()\n", getpid());
    exit(0);
}

void exo10(){
    /*
     * Exécution de la commande ps -l depuis un processus fils
     */
    int processus;
    processus = fork();
    if(processus == 0){
        printf("====================================\n");
        printf("je suis le fils\n");
        printf("mon pid est %d\n", getpid());
        printf("mon père a pour pid %d\n", getppid());
        printf("je vais exécuter la commande ps -l\n");
        execl("/bin/ps", "ps", "-l", NULL);
        printf("je suis le fils et je meurs\n");
        exit(1);
    } else {
        printf("====================================\n");
        printf("je suis le père\n");
        int *terminaison = malloc(sizeof(int));
        pid_t res = waitpid(processus, terminaison, WCONTINUED);
        printf("/// waitpid a retourné %d ///\n", res);
        printf("avec la terminaison %d\n", *terminaison);
        printf("qui s'interprète comme exit code %d\n", WEXITSTATUS(*terminaison));
        free(terminaison);

        exit(0);
    }
}

void exo10b() {
    /*
     * Même chose, mais avec execv
     */
    int processus;
    processus = fork();
    if (processus == 0) {
        printf("====================================\n");
        printf("je suis le fils\n");
        printf("mon pid est %d\n", getpid());
        printf("mon père a pour pid %d\n", getppid());
        printf("je vais exécuter la commande ps -l\n");
        char *args[] = {"ps", "-l", NULL};
        execv("/bin/ps", args);
        printf("je suis le fils et je meurs\n");
        exit(1);
    } else {
        printf("====================================\n");
        printf("je suis le père\n");
        int *terminaison = malloc(sizeof(int));
        pid_t res = waitpid(processus, terminaison, WCONTINUED);
        printf("/// waitpid a retourné %d ///\n", res);
        printf("avec la terminaison %d\n", *terminaison);
        printf("qui s'interprète comme exit code %d\n",
               WEXITSTATUS(*terminaison));
        free(terminaison);
        exit(0);
    }
}

void exo11(){
    /*
     * Interpréteur de commandes simplifié
     * avec fork, execv et waitpid
     * qui lit depuis l'entrée standard
     */
    char *commande = malloc(80 * sizeof(char));
    char *args[80];
    int processus;
    processus = fork();
    if(processus == 0){
        printf("====================================\n");
        printf("PID %d (père: %d)\n", getpid(), getppid());
        printf("entrez une commande : ");
        scanf("%[^\n]", commande);
        if(strcmp(commande, "exit") == 0){
            exit(9);
        }
        char *token = strtok(commande, " ");
        int i = 0;
        while(token != NULL){
            args[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        args[i] = NULL;
        char *path = malloc(80 * sizeof(char));
        sprintf(path, "/bin/%s", args[0]);
        execv(path, args);
        printf("Commande inconnue.\n");
        exit(1);
    } else {
        int *terminaison = malloc(sizeof(int));
        pid_t res = waitpid(processus, terminaison, 0);
        printf("/// fin du PID %d ///\n", res);
        printf("avec la terminaison %d (exit code %d)\n",
               *terminaison,
               WEXITSTATUS(*terminaison));
        sleep(1);
        if(WEXITSTATUS(*terminaison) == 9){
            printf("Fin du programme.\n");
            free(terminaison);
            free(commande);
            exit(0);
        } else{
            free(terminaison);
            free(commande);
            exo11();
        }
        exit(1);
    }
}

int main(int argc, const char * argv[]) {
    exo11();
    return 0;
}
