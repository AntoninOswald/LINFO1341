#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "../include/matrix.h"


#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

Matrix * allocate_matrix(int m, int n) {
	Matrix * mat = (Matrix*) malloc(sizeof(Matrix));
	mat->m = m, mat->n = n;
	mat->data = (double*) calloc(m*n,sizeof(double));
	mat->a = (double**) calloc(m,sizeof(double*));
	for(int i = 0; i < m; i++)
		mat->a[i] = mat->data+i*n;
	return mat;
}

void free_matrix(Matrix * mat) {
	free(mat->a);
	free(mat->data);
	free(mat);
}

void print_vector(double * v, int n) {
	for(int i = 0; i < n; i++)
		printf("%.3e ", v[i]);
	printf("\n");
}

void print_matrix(Matrix *A){

	int m = A->m;
	int n = A->n;
	double **a = A->a;

	printf("=====================================\n");
	printf("Dimension %d x %d : \n",m,n);

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf("%.12f ",a[i][j]);
		}
		printf("\n");
	}
}

int is_symmetric(Matrix * K) {
	int symmetric = 1;
	for(int i = 0; i < K->m; i++)
		for(int j = i+1; j < K->n; j++)
			if(fabs((K->a[i][j] - K->a[j][i]) / K->a[i][j]) > 1e-12) {
				printf("%d %d\n", i, j);
				printf("%lf %lf\n", K->a[i][j], K->a[j][i]);
				symmetric = 0;
			}
	return symmetric;
}

//######################## 
typedef struct Ysort{
	double y;
	int idx;
} Ysort;



typedef struct Xsort{
	double x;
	int idx;
} Xsort;


int compare_coords(const void *e0, const void *e1){
    Xsort *first = (Xsort *)e0;
    Xsort *second = (Xsort *)e1;

    if (first->x < second->x) {
        return -1;
    } else if (first->x > second->x) {
        return 1;
    } 

	return 0;
}



int compute_permutation(int * perm, double * coord, int n_nodes){


	printf("Nodes : %d\n",n_nodes);
	
	//tri selon x 
	Xsort *sort = (Xsort *)malloc(sizeof(Xsort)*n_nodes);

	for (int i = 0; i < n_nodes; i++)
	{

		sort[i].x = coord[2*i];
		sort[i].idx = i;

	}

	qsort(sort,n_nodes,sizeof(Xsort),compare_coords);

	//permutation globales
	
	for (int i = 0; i < n_nodes; i++)
	{
		
		perm[2*sort[i].idx] = 2*i;
		perm[2*sort[i].idx+1] = 2*i+1;
	}

	free(sort);
	return 0;
}

//##############""
void convert_to_band(Matrix*A, BandMatrix*B){


	int k = B->k;

	for (int i = 0; i < B->m; i++) {
		for (int j = MAX(i-k,0); j < MIN(i+k+1,B->m); j++) {
			B->a[i][j] = A->a[i][j];
			
		}
	}
}


//défini tel que l'élément du milieu = élément diagonal
BandMatrix * allocate_band_matrix(int m, int k) {
    BandMatrix * A = (BandMatrix *) malloc(sizeof(BandMatrix));
	
    A->m = m;
    A->k = k;
   
    A->data = (double *) calloc(m * (2 * k + 1),sizeof(double));
   
    A->a = (double **) malloc(m*sizeof(double *));
    A->a[0] = A->data+k; 
	
	for (int i = 1; i < m; i++)
	{
		
		A->a[i] = A->a[i-1] + 2*k;  
	}
	return A;
}


void print_band_matrix(BandMatrix *A){
	int k = A->k;
	int m = A->m;
	double *data = A->data;
	printf("===========================================\n");
	printf("Matrice band avec k = %d et m = %d :\n",k,m);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < 2*k+1; j++) {
			printf("%f, ",data[i*(2*k+1) + j]);
		}

		printf("\n");
	}
}


int compute_band(Matrix *A){
	//on applique la définition donnée dans le devoir
    int band_size = 0;
	double **matrix = A->a;
	int m = A->n;
	int n = A->n;
    for (int i = 0; i<m; i++) {
		for (int j = 0; j <n; j++) {
			if (fabs(matrix[i][j]) > 0.0 && abs(j-i) > band_size){

				band_size = abs(j-i);	
			}
		}
	}

	

	return band_size;
}
//####################""
// Utilitaire 



void free_band_matrix(BandMatrix * mat){
	free(mat->data);
	free(mat->a);
	free(mat);
}


double norm(Matrix *A){
	float sum = 0.0;
	
	double **a = A->a;

	int m = A->m;
	int n = A->n;

	for (int i = 0; i < m; i++) {
		
		for (int j =0; j < n; j++) {
			
			sum += a[i][j]*a[i][j];
		}
	}


	return sqrt(sum);
}


void divide(Matrix *A, float div){
	double **a = A->a;

	int m = A->m;
	int n = A->n;

	for (int i = 0; i < m; i++) {
		
		for (int j =0; j < n; j++) {
			
			a[i][j] = a[i][j]/div;
		}
	}
	
}

double dot_product(Matrix *A, Matrix *B){


	int m1 = A->m;
	double **a = A->a;
	double **b = B->a;

	double res = 0.0;

	for (int i = 0; i<m1; i++) {
		
		res += a[i][0]*b[i][0];
	}

	return res;
}

int mult_matrix(Matrix * A, Matrix * B, Matrix * C) {
	if((A->m != C->m) || (B->m != C->m) || (A->n != B->m)){
		printf("ERROR : wrong matrix dimension\n");
		return -1;
	}
	for(int i = 0; i < A->m; i++){
		for(int j = 0; j < B->n; j++){
			double s = 0;
			for(int k = 0; k < A->n; k++){
				s += A->a[i][k]*B->a[k][j];
			}
			C->a[i][j] = s;
		}
	}
	return 0;
}


void matrix_to_file(Matrix *A, FILE *fd){
	

	int m = A->m;
	int n = A->n;
	double **a = A->a;
	
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			fprintf(fd, "%.20f ",a[i][j]);
		}
		fprintf(fd, "\n");
	}
}