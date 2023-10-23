#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>
#include <stdio.h>
#define SIZE_OF_STACK 10 

/* Le type indiquant la direction */
enum direction{north, east, south, west};
typedef enum direction direction;

/* Le type composante */
typedef unsigned char composante;

/* Le type couleur */
typedef struct {
    composante rouge;
    composante vert;
    composante bleu;
} couleur;

/*
@requires: rouge, vert et bleu sont compris entre 0 et 255
@assigns: nothing
@ensures: renvoie une couleur RVB en cas de succès, met fin au programme sinon
*/
couleur newCouleur(composante rouge, composante vert, composante bleu);

/* Le type pixel */
typedef struct {
    couleur couleur;
    composante opacite;
} pixel;

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie un nouveau pixel de couleur (0,0,0) et d'opacité 0 en cas de succès, NULL sinon
*/
pixel newPixel();

/* Le type calque */
typedef struct {
    int size;
    pixel** cells;
} calque;

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie un tableau 2D de taille size*size et rempli de pixels noirs d'opacité 0 en cas de succès, NULL sinon
*/
calque newCalque(int size);

/*
@requires: une adresse vers un calque valide
@assigns: calque c
@ensures: libère la mémoire occupée par le calque c
*/
void freeCalque(calque* c);

/*
@requires: un calque valide
@assigns: nothing
@ensures: affiche le calque passé en argument à l'écran
*/
void printCalque(calque c);

/* Le type pile de calques */
typedef struct {
    calque stack[SIZE_OF_STACK];
    int top;
} pile_of_calques;

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie une pile de calques vide, de taille SIZE_OF_STACK en cas de succès, NULL sinon
*/
pile_of_calques newPile();

/*
@requires: une pile de calques valide
@assigns: la pile de calques passée en argument
@ensures: ajoute le calque c en haut de pile si la pile n'est pas pleine, renvoie un message d'erreur et ne fait rien sinon
*/
void pushPile(pile_of_calques* p_calques, calque c);

/*
@requires: une pile de calques valide
@assigns: la pile de calques passée en argument
@ensures: renvoie le calque situé en haut de la pile si la pile n'est pas vide, renvoie un calque de taille 0 sinon
*/
calque popPile(pile_of_calques* p_calques);

/*
@requires: une pile de calques valide
@assigns: la pile de calques passée en argument
@ensures: dépile le calque du haut et libère sa mémoire si la pile n'est pas vide, renvoie un message d'erreur et ne fait rien sinon
*/
void popFreeCalque(pile_of_calques *p_calques);

/* Le type seau de couleur */
typedef struct Node *seau_couleur;
struct Node {
    couleur couleur;
    seau_couleur next;
};

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie un seau vide, ie NULL
*/
seau_couleur newSeauCouleur();

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie 1 si le seau de couleur passé en argument est vide, 0 sinon
*/
int isEmptySeauCouleur(seau_couleur seau);

/*
@requires: nothing
@assigns: nothing
@ensures: ajoute par effet la couleur c au seau de couleur et renvoie un seau de couleur en cas de succès, met fin au programme sinon
*/
seau_couleur addEffetCouleur(seau_couleur seau, couleur c);

/*
@requires: nothing
@assigns: le seau de couleur passé en argument
@ensures: ajoute la couleur c au seau de couleur en cas de succès, ne fait rien sinon
*/
void addCouleur(seau_couleur *seau, couleur c);

/*
@requires: nothing
@assigns: le seau de couleur passé en argument
@ensures: vide le seau de couleur passé en argument et libère la mémoire qu'il occupait
*/
void VideSeauCouleur(seau_couleur *seau);

/* Le type seau d'opacité */
typedef struct Node_ *seau_opacite;
struct Node_ {
    composante opacite;
    seau_opacite next;
};

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie un seau vide, ie NULL
*/
seau_opacite newSeauOpacite();

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie 1 si le seau d'opacité passé en argument est vide, 0 sinon
*/
int isEmptySeauOpacite(seau_opacite seau);

/*
@requires: nothing
@assigns: nothing
@ensures: ajoute par effet l'opacité opacite au seau d'opacité et renvoie un seau d'opacité en cas de succès, renvoie NULL sinon
*/
seau_opacite addEffetOpacite(seau_opacite seau, composante opacite);

/*
@requires: nothing
@assigns: le seau d'opacité passé en argument
@ensures: ajoute l'opacité au seau d'opacité en cas de succès, ne fait rien sinon
*/
void addOpacite(seau_opacite *seau, composante opacite);

/*
@requires: nothing
@assigns: le seau d'opacité passé en argument
@ensures: vide le seau d'opacité passé en argument et libère la mémoire qu'il occupait
*/
void VideSeauOpacite(seau_opacite *seau);

/* Le type machine */
typedef struct {
    int pos[2]; // indice i,j de la matrice correspond coordonées y,x avec l'axe des y orienté vers le bas,
    int markedpos[2]; // l'axe des x vers la droite et l'origine du repère en haut à gauche
    direction direction;
    seau_couleur seau_couleur;
    seau_opacite seau_opacite;
    pile_of_calques calques; 
} machine;

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
machine createMachine(int size);

/* Fonctions pour le calcul du pixel courant */
/*
@requires: nothing
@assigns: nothing
@ensures: renvoie la moyenne des opacités du seau d'opacité passé en argument sans le modifier si non vide, 255 sinon
*/
composante moyOpacite(seau_opacite seau);

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie la couleur moyenne des couleurs du seau de couleur passé en argument sans le modifier si non vide, (0,0,0) sinon
*/
couleur moyCouleur(seau_couleur seau);

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie le pixel courant de la machine, qui possède la couleur moyenne du seau de couleur et l'opacité moyenne du seau d'opacité
*/
pixel calculPixelCourant(machine machine);

/* Fonctions pour le tracer de lignes */
/*
@requires: nothing
@assigns: nothing
@ensures: renvoie le max de a et b
*/
int max(int a, int b);

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie 1 si a et b sont non nuls et de mêmes signes, 0 sinon
*/
int memeSigneNonNuls(int a, int b);

/*
@requires: une machine valide
@assigns: nothing
@ensures: trace une ligne de la valeur du pixel courant de la machine passée en arguments entre les positions init et final
*/
void tracerLigne(int init[2], int final[2], machine* machine);

/* Fonctions pour le remplissage */
/*
@requires: 2 pixels valides
@assigns: nothing
@ensures: renvoie 1 si les pixels p1 et p2 sont égaux, 0 sinon
*/
int areEqualPixels(pixel p1, pixel p2);

/* Le type point */
typedef struct point {
    int x;
    int y;
} point;

/* Le type points, qui est une liste chainées de point*/
typedef struct Node__ *points;
struct Node__ {
    point p;
    points next;
};

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie une liste vide points, ie NULL
*/
points newPoints();

/*
@requires: une liste valide
@assigns: nothing
@ensures: renvoie une liste composé du point p en tête et de la liste l en cas de succès, NULL sinon
*/
points addEffetPoint(points l, point p);

/*
@requires: nothing
@assigns: la valeur de la liste pointée
@ensures: ajoute par valeur le point p à la liste l
*/
void addPoint(points *l, point p);

/*
@requires: nothing
@assigns: nothing
@ensures: renvoie 1 si la liste est vide, 0 sinon
*/
int isEmptyPoints(points p);

/*
@requires: une liste de points l non vide
@assigns: la liste pointée
@ensures: la tête de la liste 
*/
point popPoints(points *l);

/* Le type matrix */
typedef struct {
    int size;
    int** cells;
} matrix;

/*
@requires: size > 0
@assigns: nothing
@ensures: renvoie une matrice 2D de taille size*size initalisee avec des 0
*/
matrix matrixCreate(int size);

/*
@requires: une calque c valide, deux pixels ancien et nouveau valides et 2 entiers x et y positifs inférieurs à c.size-1
@assigns: calque c
@ensures: remplit la zone autour de x,y selon l'algorithme de remplissage par diffusion
*/
void fill(int x, int y, pixel ancien, pixel nouveau, calque c);

/* Fonction Fusion de Calques */
/*
@requires: 2 calques valides de meme taille
@assigns: calque c1
@ensures: fusionne le calque c0 (celui du haut) sur le calque c1 (celui du bas)
*/
void fusionCalques(calque c0, calque c1);

/*
@requires: 2 calques valides
@assigns: calque c1
@ensures: découpe le calque c1 (celui du bas) par rapport à l'opacité de c0 (celui du haut)
*/
void decoupageCalque(calque c0, calque c1);

/* Fonctions liées au format d'entrée */
/*
@requires: une adresse vers une machine valide
@assigns: la machine pointée
@ensures: avance le curseur d'un pixel vers la direction de la machine, en s'assurant de "passer de l'autre côté" si un bord est atteint
*/
void avanceDirection(machine* machine);

/*
@requires: une adresse vers une machine valide
@assigns: la machine pointée
@ensures: effectue un changement de la direction dans le sens horaire
*/
void rotationHoraire(machine* machine);

/*
@requires: une adresse vers une machine valide
@assigns: la machine pointée
@ensures: effectue un changement de la direction dans le sens anti-horaire
*/
void rotationAntiHoraire(machine* machine);

/*
@requires: une adresse vers une machine valide
@assigns: la machine pointée
@ensures: effectue l'action associée au caractère passé conformément à l'énoncé et modifie l'état de la machine en conséquence,
si le caractère ne fait pas parti de la liste des actions autorisées, ne fait rien
*/
void lecture(char character, machine *machine);

/* Fonctions pour la création de l'image */
/*
@requires: une calque valide et la taille de ce calque
@assigns: nothing
@ensures: renvoie un tableau 1D des couleurs du calque passé en arguments, sans les opacités
*/
couleur* createBtm(calque c, int size);

/*
@requires: une calque valide et la taille de ce calque, un descripteur de fichier valide
@assigns: nothing
@ensures: écrit l'entête d'un fichier PPM dans le fichier pointé par fd puis écrit la bitmap associée au calque dans le ficher pointé par le fd
*/
void creationPPM(calque c, int size, FILE* fd);

#endif