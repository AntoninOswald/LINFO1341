#ifndef MATRIX_H // le header guard
#define MATRIX_H 

#include <stdio.h>

typedef struct Triplet {
	int i,j;	// index of unknowns
    double val;	// value of entry to add to matrix
} Triplet;

typedef struct Matrix {
	int m, n;       // dimensions de la matrice
	double * data;  // tableau 1D de taille m*n contenant les entrées de la matrice
	double ** a;    // tableau 1D de m pointeurs vers chaque ligne, pour pouvoir appeler a[i][j]
} Matrix;

typedef struct BandMatrix {
	int m, k;		// dimension de la matrice et largeur de bande
	double * data;	// tableau 1D de taille m*(2*k+1) contenant les entrées de la matrice
	double ** a;	// tableau 1D de m pointeurs vers chaque ligne, pour pouvoir appeler a[i][j]
} BandMatrix;


Matrix * allocate_matrix(int m, int n); // alloue une matrice de dimensions données
BandMatrix * allocate_band_matrix(int m, int k); // alloue une matrice bande de dimensions données
void free_matrix(Matrix * mat);         // libère la mémoire de la structure Matrix donnée
void free_band_matrix(BandMatrix * mat);         // libère la mémoire de la structure BandMatrix donnée

// Calcule une permutation pour réduire le fill-in.
// On peut utiliser les coordonnées des noeuds ainsi que les entrées de la matrice (éventuellement)
int compute_permutation(int * perm, double * coord, int n_nodes); 

void print_vector(double * v, int n);   // imprime le contenu d'un vecteur (tableau) de taille n
void print_matrix(Matrix * A);          // imprime le contenu d'une matrice

int is_symmetric(Matrix * K);


//////////////////
//Added
int mult_matrix(Matrix *A, Matrix *B, Matrix *C);
double norm(Matrix *A); //Calcule la norme en faisant le carré de chaque élément puis racine
//Principalement utilisé quand on utilise la structure matricielle pour un vecteur colonne par exemple.
void divide(Matrix *A, float div); 
double dot_product(Matrix *A, Matrix *B); //must be vectors good sized. The user must be aware of that

///////////////////

int compute_band(Matrix *A); //Calcule la bande d'une matrice symmétrique

void print_band_matrix(BandMatrix *A);//affiche les bandes d'une BandMatrix

void convert_to_band(Matrix*A, BandMatrix*B);//Permet la conversion
//Vers une matrice bande déjà allouée.

//Ecrit dans un fichier déjà ouvert la matrice A, afin de pouvoir l'utiliser
// avec une fonction du type numpy.loadtxt
// Il convient à l'utilisateur de fermer manuellementle fichier par la suite
void matrix_to_file(Matrix *A, FILE *fd);

#endif