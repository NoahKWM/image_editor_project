#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "types.h"

int main(int argc, char* argv[]) {
    
    /* Aucun argument */
    if (argc == 1) {

        /* On récupère la taille de l'image que l'on stocke dans size */
        int size;
        char buf[256];
        fgets(buf, 256, stdin);
        sscanf(buf, "%d", &size);

        /* On initialise la machine avec la bonne taille */
        machine mach = createMachine(size);

        /* On lit les caractères 1 à 1 et on fait appelle à la fonction lecture() qui met à jour l'état de la mahcine */
        char c;
        while (c != EOF) {
            c = fgetc(stdin);
            lecture(c, &mach);
        }

        /* On récupère le calque en haut de la machine pour former l'image que l'on écrit sur stdout */
        calque cal = popPile(&(mach.calques));
        creationPPM(cal,size,stdout);

        return 0;
    }

    /* Un seul argument: un fichier .ipi dans lequel on lit les caractères */
    if (argc == 2) {

        /* on ouvre le fichier .ipi pour lire les caractères */
        FILE* fd = fopen(argv[1], "r");
        if (fd == NULL) {
            fprintf(stderr, "erreur lors de l'ouverture du fichier\n");
            exit(2);
        }

        /* On récupère la taille du fichier */
        char buf[256];
        int size;
        fgets(buf, 256, fd);
        sscanf(buf, "%d", &size);

        /* On initialise la machine avec la bonne taille */
        machine mach = createMachine(size);

        /* On lit les caractères 1 à 1 et on fait appelle à la fonction lecture() qui met à jour l'état de la mahcine */
        char c;
        while (c != EOF) {
            c = fgetc(fd);
            lecture(c, &mach);
        }

        /* On récupère le calque en haut de la machine pour former l'image que l'on écrit sur stdout */
        calque cal = popPile(&(mach.calques));
        creationPPM(cal,size,stdout);

        return 0;
    }

    /* 2 fichiers en paramètres: un .ipi dans lequel on lit les caractères, un .ppm dans lequel on écrit l'image */
    if (argc == 3) {
        /* on ouvre le fichier .ipi pour lire les caractères */
        FILE* fd = fopen(argv[1], "r");
        if (fd == NULL) {
            fprintf(stderr, "erreur lors de l'ouverture du fichier\n");
            exit(2);
        }

        /* On récupère la taille du fichier */
        char buf[256];
        int size;
        fgets(buf, 256, fd);
        sscanf(buf, "%d", &size);

        /* On initialise la machine avec la bonne taille */
        machine mach = createMachine(size);

        /* On lit les caractères 1 à 1 et on fait appelle à la fonction lecture() qui met à jour l'état de la mahcine */
        char c;
        while (c != EOF) {
            c = fgetc(fd);
            lecture(c, &mach);
        }

        fclose(fd); // On ferme le fichier .ipi

        /* On récupère le calque en haut de la machine pour former l'image que l'on écrit sur stdout */
        calque cal = popPile(&(mach.calques));

        /* On ouvre le fichier .ppm dans lequel on souhaite écrire et on écrit dedans avec creationPPM() */
        FILE* fdout = fopen(argv[2], "wb");
        creationPPM(cal,size,fdout);

        return 0;
    }


    /* Plus de 2 arguments: le programme s'execute comme s'il n'y avait que les 2 premiers */
    if (argc > 3) {

        fprintf(stderr, "Attention, le programme prend au maximum 2 arguments\n");

        /* on ouvre le fichier .ipi pour lire les caractères */
        FILE* fd = fopen(argv[1], "r");
        if (fd == NULL) {
            fprintf(stderr, "erreur lors de l'ouverture du fichier\n");
            exit(2);
        }

        /* On récupère la taille du fichier */
        char buf[256];
        int size;
        fgets(buf, 256, fd);
        sscanf(buf, "%d", &size);

        /* On initialise la machine avec la bonne taille */
        machine mach = createMachine(size);

        /* On lit les caractères 1 à 1 et on fait appelle à la fonction lecture() qui met à jour l'état de la mahcine */
        char c;
        while (c != EOF) {
            c = fgetc(fd);
            lecture(c, &mach);
        }

        fclose(fd); // On ferme le fichier .ipi

        /* On récupère le calque en haut de la machine pour former l'image que l'on écrit sur stdout */
        calque cal = popPile(&(mach.calques));

        /* On ouvre le fichier .ppm dans lequel on souhaite écrire et on écrit dedans avec creationPPM() */
        FILE* fdout = fopen(argv[2], "wb");
        creationPPM(cal,size,fdout);

        return 0;
    }
}
