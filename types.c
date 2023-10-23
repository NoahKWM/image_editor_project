#include <stdlib.h>
#include <stdio.h>
#include "types.h"


/*
@requires: rouge, vert et bleu sont compris entre 0 et 255
@assigns: nothing
@ensures: renvoie une couleur RVB en cas de succès, met fin au programme sinon
*/
couleur newCouleur(composante rouge, composante vert, composante bleu) {
    couleur *coul = (couleur*)malloc(sizeof(couleur));
    if (NULL == coul) {
        fprintf(stderr, "erreur lors de l'allocation mémoire de la couleur\n"); // affiche un messsage d'erreur si l'allocation a échoué
        exit(EXIT_FAILURE); // met fin au programme
    }
    /* on initialise les valeur de coul */
    coul->rouge = rouge;
    coul->vert = vert;
    coul->bleu = bleu;
    return *coul;
}

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie un nouveau pixel de couleur (0,0,0) et d'opacité 0 en cas de succès, NULL sinon
*/
pixel newPixel() {
    pixel *pix = (pixel*)malloc(sizeof(pixel));
    if (NULL == pix) {
        fprintf(stderr, "erreur lors de l'allocation mémoire d'un nouveau pixel\n"); // affiche un messsage d'erreur si l'allocation a échoué
        exit(EXIT_FAILURE); // met fin au programme
    }
    /* on initialise les valeurs de pix */
    pix->couleur = newCouleur(0,0,0);
    pix->opacite = 0;
    return *pix;
}

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie un tableau 2D de taille size*size et rempli de pixels noirs d'opacité 0 en cas de succès, NULL sinon
*/
calque newCalque(int size) {
    calque *c = (calque*)malloc(sizeof(calque));
    if (NULL == c) {
        fprintf(stderr, "erreur lors de l'allocation mémoire d'un nouveau calque\n"); // affiche un messsage d'erreur si l'allocation a échoué
        exit(EXIT_FAILURE); // met fin au programme
    }
    /* on initialise le calque avec la bonne taille */
    c->size = size;
    c->cells = (pixel**)malloc(size * sizeof(pixel*)); // on alloue la mémoire nécessaire pour les cellules
    if (NULL == c->cells) {
        fprintf(stderr,"erreur lors de l'allocation mémoire du tableau cells du calque\n"); // affiche un messsage d'erreur si l'allocation a échoué
        exit(EXIT_FAILURE); // met fin au programme
    }

    pixel pix = newPixel(); // on crée un pixel blanc d'opacite 0 pour remplir notre calque
    /* on boucle sur i et on alloue un tableau pour chaque case */
    int i,j;
    for (i=0; i<size; i+=1) {
        c->cells[i] = (pixel*)malloc(size * sizeof(pixel));
        if (NULL == c->cells[i]) {
            fprintf(stderr, "erreur lors de l'allocation de la i-ème ligne de l'image\n"); // message d'erreur si l'allocation a échoué
            exit(EXIT_FAILURE); // met fin au programme
        }

        /* on boucle sur j pour initialiser chaque case du nouveau tableau crée avec un nouveau pixel */
        for (j=0; j<size; j+=1) {
            c->cells[i][j] = pix; // on initialise chaque pixel du calque
        }
    }
    return *c; // on renvoie le calque crée et initialisé
}

/*
@requires: une adresse vers un calque valide
@assigns: calque c
@ensures: libère la mémoire occupée par le calque c
*/
void freeCalque(calque* c) {
    int i;
    int size = c->size;

    /* on libère chaque case du tableau */
    for (i=0; i<size; i+=1) {
        free(c->cells[i]);
    }

    free(c->cells);
}

/*
@requires: un calque valide
@assigns: nothing
@ensures: affiche le calque passé en argument à l'écran
*/
void printCalque(calque c) {
    int size = c.size;
    int i,j;
    /* on boucle sur i,j et on affiche les 3 couleurs de chaque pixels */
    for (i=0; i<size; i+=1) {
        for (j=0; j<size; j+=1) {
            printf("%d %d %d / ", c.cells[i][j].couleur.rouge, c.cells[i][j].couleur.vert, c.cells[i][j].couleur.bleu);
        }
        printf("\n-- -- --\n");
    }
}

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie une pile de calques vide, de taille SIZE_OF_STACK en cas de succès, NULL sinon
*/
pile_of_calques newPile() {
    pile_of_calques *p_calques = (pile_of_calques*)malloc(sizeof(pile_of_calques));
    if (NULL == p_calques) {
        fprintf(stderr, "erreur lors de l'allocation mémoire d'une nouvelle pile de calques\n"); // message d'erreur si l'allocation a échoué
        exit(EXIT_FAILURE); // met fin au programme
    }

    p_calques->top = -1; // on initialise le haut de la pile à -1, ie la pile est vide
    return *p_calques; // on renvoie la pile de calques
}

/*
@requires: une pile de calques valide
@assigns: la pile de calques passée en argument
@ensures: ajoute le calque c en haut de pile si la pile n'est pas pleine, renvoie un message d'erreur et ne fait rien sinon
*/
void pushPile(pile_of_calques *p_calques, calque c) {
    if (SIZE_OF_STACK-1 == p_calques->top) {
        fprintf(stderr, "Stack is full\n"); // affiche un message d'erreur si la pile est pleine
        return; // et ne fait rien
    }
    p_calques->top += 1; // augmente l'indice du haut de pile
    p_calques->stack[p_calques->top] = c; // ajoute le calque en haut de la pile
}

/*
@requires: une pile de calques valide
@assigns: la pile de calques passée en argument
@ensures: depile et renvoie le calque situé en haut de la pile si la pile n'est pas vide, renvoie un calque de taille 0 sinon
*/
calque popPile(pile_of_calques *p_calques) {
    if (p_calques->top < 0) { // on regarde si la pile est vide
        printf("pile is empty, cannot pop last element\n"); // affiche un message d'erreur si la pile est vide
        return newCalque(0); // on return un calque de taille 0
    }
    // sinon ...
    calque* e = malloc(sizeof(calque)); // on alloue la mémoire pour le calque à renvoyer 
    *e = p_calques->stack[p_calques->top]; // on affecte à cette mémoire la valeur du calque du haut de pile
    p_calques->top -= 1; // on décrémente l'indice du haut de pile
    return *e; // on renvoie le calque
}

/*
@requires: une pile de calques valide
@assigns: la pile de calques passée en argument
@ensures: dépile le calque du haut et libère sa mémoire si la pile n'est pas vide, renvoie un message d'erreur et ne fait rien sinon
*/
void popFreeCalque(pile_of_calques *p_calques) {
    if (p_calques->top < 0) { // on regarde si la pile est vide
        printf("pile is empty, cannot pop last element\n"); // affiche un message d'erreur si la pile est vide
        return; // ne fait rien 
    }

    calque* e = malloc(sizeof(calque)); // on alloue la mémoire pour le calque à renvoyer
    *e = p_calques->stack[p_calques->top]; // on affecte à cette mémoire la valeur du calque du haut de pile
    p_calques->top -= 1; // on décrémente l'indice du haut de pile

    freeCalque(e); // on libère la mémoire du calque crée
}

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie un seau vide, ie NULL
*/
seau_couleur newSeauCouleur() {
    return NULL;
}

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie 1 si le seau de couleur passé en argument est vide, 0 sinon
*/
int isEmptySeauCouleur(seau_couleur seau) {
    return NULL == seau;
}

/*
@requires: une couleur c valide
@assigns: nothing
@ensures: ajoute par effet la couleur c au seau de couleur et renvoie un seau de couleur en cas de succès, met fin au programme sinon
*/
seau_couleur addEffetCouleur(seau_couleur seau, couleur c) {
    seau_couleur res = (seau_couleur)malloc(sizeof(struct Node));
    if (NULL == res) {
        fprintf(stderr, "erreur lors de l'allocation mémoire d'un nouveau seau de couleurs\n"); // message d'erreur si l'allocation a échoué 
        exit(EXIT_FAILURE); // met fin au programme
    }
    /* on ajoute la couleur c en tête de liste */
    res->couleur = c;
    res->next = seau;
    return res; // on renvoie le seau res
}

/*
@requires: une couleur c valide
@assigns: le seau de couleur passé en argument
@ensures: ajoute la couleur c au seau de couleur en cas de succès, ne fait rien sinon
*/
void addCouleur(seau_couleur *seau, couleur c) {
    *seau = addEffetCouleur(*seau, c);
}

/*
@requires: nothing
@assigns: le seau de couleur passé en argument
@ensures: vide le seau de couleur passé en argument et libère la mémoire qu'il occupait
*/
void VideSeauCouleur(seau_couleur *seau) {
    seau_couleur a_supprimer = newSeauCouleur();
    while (!isEmptySeauCouleur(*seau)) { // boucle tant que le seau n'est pas vide
        a_supprimer = *seau; // a_supprimer devient le maillon actuel de la liste ...
        *seau = (*seau)->next; // on passe à l'élément suivant ...
        free(a_supprimer); // puis on free ce maillon
    }
}

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie un seau vide, ie NULL
*/
seau_opacite newSeauOpacite() {
    return NULL;
}

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie 1 si le seau d'opacité passé en argument est vide, 0 sinon
*/
int isEmptySeauOpacite(seau_opacite seau) {
    return seau == NULL;
}

/*
@requires: nothing
@assigns: nothing
@ensures: ajoute par effet l'opacité opacite au seau d'opacité et renvoie un seau d'opacité en cas de succès, renvoie NULL sinon
*/
seau_opacite addEffetOpacite(seau_opacite seau, composante opacite) {
    seau_opacite res = (seau_opacite)malloc(sizeof(struct Node_));
    if (NULL == res) {
        fprintf(stderr, "erreur lors de l'allocation de mémoire d'un nouveau seau d'opacité\n"); // message d'erreur si l'allocation a échoué 
        exit(EXIT_FAILURE); // met fin au programme
    }
    
    /* on ajoute l'opacite en tete de liste */
    res->opacite = opacite;
    res->next = seau;
    return res; // on renvoie le seau
}

/*
@requires: nothing
@assigns: le seau d'opacité passé en argument
@ensures: ajoute l'opacité au seau d'opacité en cas de succès, ne fait rien sinon
*/
void addOpacite(seau_opacite *seau, composante opacite) {
    *seau = addEffetOpacite(*seau, opacite);
}

/*
@requires: nothing
@assigns: le seau d'opacité passé en argument
@ensures: vide le seau d'opacité passé en argument et libère la mémoire qu'il occupait
*/
void VideSeauOpacite(seau_opacite *seau) {
    seau_opacite a_supprimer = newSeauOpacite();
    while (!isEmptySeauOpacite(*seau)) { // boucle tant que le seau n'est pas vide
        a_supprimer = *seau; // a_supprimer devient le maillon actuel de la liste ...
        *seau = (*seau)->next; // on passe à l'élément suivant ...
        free(a_supprimer); // puis on free ce maillon
    }
}

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie une machine initialisée comme suit:
    - pos = (0,0)
    - markedpos = (0,0)
    - direction = east
    - seau de couleur et d'opacité vides
    - une pile de calque composée d'un calque de taille size, blanc et d'opacite 0
*/
machine createMachine(int size) {
    machine *mach = (machine*)malloc(sizeof(machine));
    if (NULL == mach) {
        fprintf(stderr, "erreur lors de l'allocation mémoire de la nouvelle machine\n"); // affiche un message d'erreur si l'allocation a échoué
        exit(EXIT_FAILURE); // met fin au programme
    }

    /* on initialise chaque valeur des composantes de la machine */
    mach->pos[0] = 0; // on considère un repère X,Y, dont l'origine est en haut à gauche et l'axe des Y est orienté vers le bas, ainsi
    mach->pos[1] = 0; // pos (x,y) != case x,y et on a alors pos (x,y) <=> case d'indices [y][x]
    mach->markedpos[0] = 0;
    mach->markedpos[1] = 0;
    mach->direction = east; // orientation de base du curseur
    mach->seau_couleur = newSeauCouleur(); // 2 seaux vides
    mach->seau_opacite = newSeauOpacite();
    mach->calques = newPile(); // pile vide 
    pushPile(&(mach->calques), newCalque(size)); // on rajoute un calque de pixels (0,0,0,(0)) en sommet de pile
    return *mach;
}

/* Fonctions pour le calcul du pixel courant */
/*
@requires: nothing
@assigns: nothing
@ensures: renvoie la moyenne des opacités du seau d'opacité passé en argument sans le modifier si non vide, 255 sinon
*/
composante moyOpacite(seau_opacite seau) {
    int sum = 0; // de type int car si on prend un type composante une reduction modulo 256 est effectuée et le resultat est faux
    int cpt = 0;
    seau_opacite tmp = seau;
    if (isEmptySeauOpacite(tmp)) { // si le seau est vide, on retourne une opacite de 255
        composante res = 255;
        return res;
    }
    else {
        while (!isEmptySeauOpacite(tmp)) { // tant que le seau n'est pas vide
            sum += tmp->opacite; // on somme l'opacite du maillon courant
            tmp = tmp->next; // on passe au maillon suivant
            cpt +=1; // on incrémente le compteur
        }
    free(tmp);
    composante res = sum/cpt; // res est bien inférieur à 255 car c'est la moyenne
    return res;
    }
}

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie la couleur moyenne des couleurs du seau de couleur passé en argument sans le modifier si non vide, (0,0,0) sinon
*/
couleur moyCouleur(seau_couleur seau) {
    /* les 3 composantes de couleur de type int car si on prend un type composante une reduction modulo 256 est effectuée et le resultat est faux */
    int sum_rouge = 0;
    int sum_vert = 0;
    int sum_bleu = 0;
    int cpt = 0;
    seau_couleur tmp = seau;
    if (isEmptySeauCouleur(tmp)) { // si le seau est vide on renvoie la couleur (0,0,0)
        couleur res = newCouleur(0,0,0);
        return res;
    }
    else {
        while (!isEmptySeauCouleur(tmp)) { // tant que le seau n'est pas vide
            /* on somme les composantes de couleur du maillon courant */
            sum_rouge += tmp->couleur.rouge;
            sum_vert += tmp->couleur.vert;
            sum_bleu += tmp->couleur.bleu;

            tmp = tmp->next; // on passe au maillon suivant
            cpt += 1; // on incrémente le compteur
        }
        free(tmp);
        /* on fait la moyenne de chaque composante de la couleur */
        composante rouge = sum_rouge/cpt;
        composante vert = sum_vert/cpt;
        composante bleu = sum_bleu/cpt;
        
        couleur res = newCouleur(rouge, vert, bleu); // on crée la couleur moyenne via la fonction newCouleur()

        return res;
    }
}

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie le pixel courant de la machine, qui possède la couleur moyenne du seau de couleur et l'opacité moyenne du seau d'opacité
*/
pixel calculPixelCourant(machine machine) {
    pixel pix = newPixel(); // on crée un nouveau pixel
    composante op = moyOpacite(machine.seau_opacite); // on calcule l'opacite moyenne
    couleur moy = moyCouleur(machine.seau_couleur); // on calcule la couleur moyenne
    couleur coul = newCouleur(moy.rouge*op/255,moy.vert*op/255, moy.bleu*op/255); // on crée la couleur du pixel courant
    /* on met à jour les valeurs du pixel courant */
    pix.couleur = coul;
    pix.opacite = op;
    return pix; // on renvoie le pixel courant
}

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie le max de a et b
*/
int max(int a, int b) {
    if (a <= b) {
        return b;
    }
    return a;
}

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie 1 si a et b sont non nuls et de mêmes signes, 0 sinon
*/
int memeSigneNonNuls(int a, int b) {
    if (a*b > 0) {
        return 1;
    }
    return 0;
}

/*
@requires: une machine valide
@assigns: nothing
@ensures: trace une ligne de la valeur du pixel courant de la machine passée en arguments entre les positions init et final
*/
void tracerLigne(int init[2], int final[2], machine* machine) {
    /* on recupere abscisses et ordonnees des positions initale et finale en faisant attention à l'ordre des arguments */
    int x0 = init[1];
    int y0 = init[0];
    int x1 = final[1];
    int y1 = final[0];
    /* on calcule les delta */
    int delta_x = x1 - x0;
    int delta_y = y1 - y0;
    int d = max(abs(delta_x), abs(delta_y));

    int s;
    if (memeSigneNonNuls(delta_x, delta_y)) {
        s = 0;
    }
    else {
        s=1;
    }

    int x = x0*d + (d-s)/2;
    int y = y0*d + (d-s)/2;

    int i;
    pixel pixel_courant = calculPixelCourant(*machine); // on recupere la valeur du pixel courant
    /* on met a jour chaque pixel entre celui inital inclus et celui final exclus*/
    for (i=0; i<d; i+=1) {
        (machine->calques.stack[machine->calques.top].cells[y/d][x/d]).couleur = pixel_courant.couleur;
        (machine->calques.stack[machine->calques.top].cells[y/d][x/d]).opacite = pixel_courant.opacite;
        x += delta_x;
        y += delta_y;
    }
    /* on met a jour le pixel final */
    (machine->calques.stack[machine->calques.top].cells[y1][x1]).couleur = pixel_courant.couleur;
    (machine->calques.stack[machine->calques.top].cells[y1][x1]).opacite = pixel_courant.opacite;
}

/*
@requires: 2 pixels valides
@assigns: nothing
@ensures: renvoie 1 si les pixels p1 et p2 sont égaux, 0 sinon
*/
int areEqualPixels(pixel p1, pixel p2) {
    /* on verifie que chacune des composantes de couleur et l'opacite sont egales dans les deux pixels */
    return (p1.couleur.rouge == p2.couleur.rouge && p1.couleur.vert == p2.couleur.vert && p1.couleur.bleu == p2.couleur.bleu && p1.opacite == p2.opacite);
}

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie une liste vide points, ie NULL
*/
points newPoints() {
    return NULL;
}

/*
@requires: une liste valide
@assigns: nothing
@ensures: renvoie une liste composé du point p en tête et de la liste l en cas de succès, NULL sinon
*/
points addEffetPoint(points l, point p) {
    points res = (points)malloc(sizeof(struct Node__));
    if (NULL == res) {
        fprintf(stderr, "erreur lors de l'allocation de mémoire d'un nouveau seau d'opacité\n"); // affiche un message d'erreur si l'allocation memoire a echoue
        exit(EXIT_FAILURE); // met fin au programme
    }
    
    /* on ajoute le point p en tete de liste */
    res->p = p;
    res->next = l;
    return res;
}

/*
@requires: nothing
@assigns: la valeur de la liste pointée
@ensures: ajoute par valeur le point p à la liste l
*/
void addPoint(points *l, point p) { // on ajoute en début de liste
    *l = addEffetPoint(*l, p);
}

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie 1 si la liste est vide, 0 sinon
*/
int isEmptyPoints(points p) {
    return p == NULL;
}

/*
@requires: une liste de points l non vide
@assigns: la liste pointée
@ensures: la tête de la liste 
*/
point popPoints(points *l) {
    point* p = malloc(sizeof(point));
    *p = (*l)->p; // on recupere l'element en haut de pile
    *l = (*l)->next; // on avance d'un element dans la liste, ie la pile
    return *p;
}

/*
@requires: une calque c valide, deux pixels ancien et nouveau valides et 2 entiers x et y positifs inférieurs à c.size-1
@assigns: calque c
@ensures: remplit la zone autour de x,y selon l'algorithme de remplissage par diffusion
*/
matrix matrixCreate(int size) {
    matrix m;
    m.size = size;
    m.cells = (int**)malloc(size * sizeof(int*));

    int i;
    for (i = 0; i < size; i++) {
        m.cells[i] = (int*)calloc(size, sizeof(int));
    }

    return m;
}

/*
@requires: une calque c valide, deux pixels ancien et nouveau valides et 2 entiers x et y positifs inférieurs à c.size-1
@assigns: calque c
@ensures: remplit la zone autour de x,y selon l'algorithme de remplissage par diffusion
*/
void fill(int x, int y, pixel ancien, pixel nouveau, calque c) {
    /* si l'ancien pixel et le nouveau sont egaux on ne fait rien */
    if (areEqualPixels(nouveau, ancien)) {
        return;
    }
    
    /* on recupere la taille size du calque et on cree une matrice de boolen de taille size*size */
    int size = c.size;
    matrix tab_bool = matrixCreate(size);
    tab_bool.cells[y][x] = 1; // on indique que le point de postion (x,y) est deja dans la pile des points a traiter en remplacant la valeur
    // de la case d'indice y,x par 1

    /* on cree la pile des points qu'il reste a traiter*/
    points pile = newPoints();
    /* on cree un point qui correspond au point courant et initialise avec les valeurs x,y */
    point* p = malloc(sizeof(point));
    p->x = x;
    p->y = y;
    int x0,y0;
    addPoint(&pile, *p); // on ajoute le point p a la pile des points a traiter

    while (!(isEmptyPoints(pile))) { // tant que la pile des points a traiter n'est pas vide
        *p = popPoints(&pile); // on recupere le point en haut de pile
        x0 = p->x; // on met a jour les variables x0 et y0 avec les coordonnees du point p
        y0 = p->y;

        /* on suit l'algorithme de remplissage par diffusion */
        if (areEqualPixels(c.cells[y0][x0], ancien)) { // on verifie l'egalite du pixel au point p avec le pixel ancien 
            c.cells[y0][x0] = nouveau; // on le met a jour

            /* puis on va regarder tous ses voisins directs */
            if (x0>0) { // si on est pas au bord de l'image ...
                /* on met à jour p */
                p->x = x0-1;
                p->y = y0;
                if (!(tab_bool.cells[p->y][p->x])) { // si p n'est pas deja dans la pile des points a traiter
                    addPoint(&pile, *p); // on l'ajoute 
                    tab_bool.cells[p->y][p->x] = 1; // on met a jour la matrice de boolen
                }
            }
            if (y0>0) { // si on est pas au bord de l'image ...
                /* on met à jour p */
                p->y = y0-1;
                p->x = x0;
                if (!(tab_bool.cells[p->y][p->x])) { // si p n'est pas deja dans la pile des points a traiter
                    addPoint(&pile, *p); // on l'ajoute 
                    tab_bool.cells[p->y][p->x] = 1; // on met a jour la matrice de boolen
                }
            }
            if (x0<size-1) { // si on est pas au bord de l'image ...
                /* on met à jour p */
                p->x = x0+1;
                p->y = y0;
                if (!(tab_bool.cells[p->y][p->x])) { // si p n'est pas deja dans la pile des points a traiter
                    addPoint(&pile, *p); // on l'ajoute 
                    tab_bool.cells[p->y][p->x] = 1; // on met a jour la matrice de boolen
                }
            }
            if (y0<size-1) { // si on est pas au bord de l'image ...
                /* on met à jour p */
                p->y = y0+1;
                p->x = x0;
                if (!(tab_bool.cells[p->y][p->x])) { // si p n'est pas deja dans la pile des points a traiter
                    addPoint(&pile, *p); // on l'ajoute 
                    tab_bool.cells[p->y][p->x] = 1; // on met a jour la matrice de boolen
                }
            }
        }
    }
    /* on libère l'espace memoire alloue */
    free(pile);
    free(p);
}

/*
@requires: 2 calques valides de meme taille
@assigns: calque c1
@ensures: fusionne le calque c0 (celui du haut) sur le calque c1 (celui du bas)
*/
void fusionCalques(calque c0, calque c1) {
    int i,j;
    int size = c0.size; // on recupere la taille des calques 
    composante alpha0;

    /* on suit l'algorithme decrit en mettant a jour chaque pixel de c1 */
    for (i=0; i<size; i+=1) {
        for (j=0; j<size; j+=1) {
            alpha0 = c0.cells[i][j].opacite;

            c1.cells[i][j].opacite = alpha0 + c1.cells[i][j].opacite * (255 - alpha0) / 255;
            c1.cells[i][j].couleur.rouge = c0.cells[i][j].couleur.rouge + c1.cells[i][j].couleur.rouge * (255 - alpha0) / 255;
            c1.cells[i][j].couleur.vert = c0.cells[i][j].couleur.vert + c1.cells[i][j].couleur.vert * (255 - alpha0) / 255;
            c1.cells[i][j].couleur.bleu = c0.cells[i][j].couleur.bleu + c1.cells[i][j].couleur.bleu * (255 - alpha0) / 255;
        }
    }
}

/*
@requires: 2 calques valides
@assigns: calque c1
@ensures: découpe le calque c1 (celui du bas) par rapport à l'opacité de c0 (celui du haut)
*/
void decoupageCalque(calque c0, calque c1) {
    int i,j;
    int size = c0.size;
    composante alpha0;

    /* on suit l'algorithme decrit en mettant a jour chaque pixel de c1 */
    for (i=0; i<size; i+=1) {
        for (j=0; j<size; j+=1) {
            alpha0 = c0.cells[i][j].opacite;

            c1.cells[i][j].opacite = c1.cells[i][j].opacite * alpha0 / 255;
            c1.cells[i][j].couleur.rouge = c1.cells[i][j].couleur.rouge * alpha0 / 255;
            c1.cells[i][j].couleur.vert = c1.cells[i][j].couleur.vert * alpha0 / 255;
            c1.cells[i][j].couleur.bleu = c1.cells[i][j].couleur.bleu * alpha0 / 255;
        }
    }
}

/*
@requires: une adresse vers une machine valide
@assigns: la machine pointée
@ensures: avance le curseur d'un pixel vers la direction de la machine, en s'assurant de "passer de l'autre côté" si un bord est atteint
*/
void avanceDirection(machine* machine) {
    /* on recupere les coordonnes du curseur ainsi que la taille des calques */
    int y = machine->pos[0];
    int x = machine->pos[1];
    int size = machine->calques.stack[machine->calques.top].size; // on récupère la taille des calques
    /* on regarde la direction du curseur puis si on se trouve sur les bords et on met à jour la position en consequence*/
    if (machine->direction == north) {
        if ((y-1) == -1) {
            machine->pos[0] = size-1;
        }
        else machine->pos[0] = y-1;
    }
    else if (machine->direction == east) {
        if ((x+1) == size) {
            machine->pos[1] = 0;
        }
        else machine->pos[1] = x+1;
    }
    else if (machine->direction == south) {
        if ((y+1) == size) {
            machine->pos[0] = 0;
        }
        else machine->pos[0] = y+1;
    }
    else {
        if ((x-1) == -1) {
            machine->pos[1] = size-1;
        }
        else machine->pos[1] = x-1;
    }
}

/*
@requires: une adresse vers une machine valide
@assigns: la machine pointée
@ensures: effectue un changement de la direction dans le sens horaire
*/
void rotationHoraire(machine* machine) {
    /* on regarde la direction courante et on la met à jour avec la suivant dans le sens horaire */
    if (machine->direction == north) {
        machine->direction = east;
    }
    else if (machine->direction == east) {
        machine->direction = south;
    }
    else if (machine->direction == south) {
        machine->direction = west;
    }
    else {
        machine->direction = north;
    }
}

/*
@requires: une adresse vers une machine valide
@assigns: la machine pointée
@ensures: effectue un changement de la direction dans le sens anti-horaire
*/
void rotationAntiHoraire(machine* machine) {
    /* on regarde la direction courante et on la met à jour avec la suivant dans le sens anti-horaire */
    if (machine->direction == north) {
        machine->direction = west;
    }
    else if (machine->direction == east) {
        machine->direction = north;
    }
    else if (machine->direction == south) {
        machine->direction = east;
    }
    else {
        machine->direction = south;
    }
}

/*
@requires: une adresse vers une machine valide
@assigns: la machine pointée
@ensures: effectue l'action associée au caractère passé conformément à l'énoncé et modifie l'état de la machine en conséquence,
si le caractère ne fait pas parti de la liste des actions autorisées, ne fait rien
*/
void lecture(char character, machine *machine) {
    /* variables potentiellement utiles */
    int x,y;
    pixel pix = newPixel();
    /* on effectue un swicth et on met à jour la machine */
    switch(character)
    {
    case 'n':
        addCouleur(&(machine->seau_couleur), newCouleur(0,0,0));
        break;
    case 'r':
        addCouleur(&(machine->seau_couleur), newCouleur(255,0,0));
        break;
    case 'g':
        addCouleur(&(machine->seau_couleur), newCouleur(0,255,0));
        break;
    case 'b':
        addCouleur(&(machine->seau_couleur), newCouleur(0,0,255));
        break;
    case 'y':
        addCouleur(&(machine->seau_couleur), newCouleur(255,255,0));
        break;
    case 'm':
        addCouleur(&(machine->seau_couleur), newCouleur(255,0,255));
        break;
    case 'c':
        addCouleur(&(machine->seau_couleur), newCouleur(0,255,255));
        break;
    case 'w':
        addCouleur(&(machine->seau_couleur), newCouleur(255,255,255));
        break;
    case 't':
        addOpacite(&(machine->seau_opacite), 0);
        break;
    case 'o':
        addOpacite(&(machine->seau_opacite), 255);
        break;
    case 'i':
        VideSeauCouleur(&(machine->seau_couleur));
        VideSeauOpacite(&(machine->seau_opacite));
        break;
    case 'v':
        avanceDirection(machine);
        break;
    case 'h':
        rotationHoraire(machine);
        break;
    case 'a':
        rotationAntiHoraire(machine);
        break;
    case 'p': // met a jour la postion marquee
        machine->markedpos[0] = machine->pos[0];
        machine->markedpos[1] = machine->pos[1];
        break;
    case 'l': // trace une ligne entre la position marquee et la postion courante du curseur 
        tracerLigne(machine->pos, machine->markedpos, machine);
        break;
    case 'f': // appelle la fonction fill pour remplir la zone de couleur autour de la position courante du curseur
        x = machine->pos[1];
        y = machine->pos[0];
        pix = calculPixelCourant(*machine);
        fill(x, y, machine->calques.stack[machine->calques.top].cells[y][x], pix, machine->calques.stack[machine->calques.top]);
        break;
    case 's': // ajoute un nouveau calque à la pile de calques
        pushPile(&(machine->calques), newCalque(machine->calques.stack[machine->calques.top].size));
        break;
    case 'e': // appelle la fonction de fusion des 2 calques du haut 
        if (machine->calques.top<=0) {
            return;
        }
        else {
            fusionCalques(machine->calques.stack[machine->calques.top],machine->calques.stack[machine->calques.top-1]);
            popFreeCalque(&(machine->calques)); //on free le calque qui sert à rien
            break;
        }
    case 'j': // appelle la fonction de decoupage des 2 calques du haut
        if (machine->calques.top<=0) {
            return;
        }
        else {
            decoupageCalque(machine->calques.stack[machine->calques.top],machine->calques.stack[machine->calques.top-1]);
            popFreeCalque((&machine->calques));
        }
        break;
    default:
        // pour toute autre lettre que celle precedentes on ne fait rien
        break;
    }
}

/*
@requires: une calque valide et la taille de ce calque
@assigns: nothing
@ensures: renvoie un tableau 1D des couleurs du calque passé en arguments, sans les opacités
*/
couleur* createBtm(calque c, int size) {
    couleur* btm = (couleur*)malloc(sizeof(couleur) * size*size);

    /* on crée un tableau 1D contenant uniquement les couleurs des pixel du calque c */
    int i,j;
    for (i=0; i<size; i+=1) {
        for (j=0; j<size; j+=1) {
            btm[i*size+j] = c.cells[i][j].couleur;
        }
    }
    
    return btm;
}

/*
@requires: une calque valide et la taille de ce calque, un descripteur de fichier valide
@assigns: nothing
@ensures: écrit l'entête d'un fichier PPM dans le fichier pointé par fd puis écrit la bitmap associée au calque dans le ficher pointé par le fd
*/
void creationPPM(calque c, int size, FILE* fd) {
    couleur* btm = createBtm(c,size); // on cree la bitmap du calque

    fprintf(fd, "P6\n%d %d\n255\n", size, size); // on ecrit l'entete du fichier PPM

    fwrite(btm, sizeof(couleur), size*size, fd); // on ecrit les pixel de couleur dans le fichier pointe par fd a partir de la bitmap
}
