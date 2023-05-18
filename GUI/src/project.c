#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmshc.h> 


#include "../include/matrix.h"
#include "../include/elasticity.h"
#include "../include/design.h"



#include <lapacke.h>
#include <lapack.h>
#include <cblas.h>
#include <sys/time.h>


double get_wtime(){
    struct timeval timecheck;
    gettimeofday(&timecheck, NULL);
    long usecs = (long)timecheck.tv_sec * 1000000 + (long)timecheck.tv_usec;
    return usecs * 1e-6;
}


int is_in_array(size_t val, size_t *arr, size_t n) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == val) {
            return 1;
        }
    }
    return 0;
}


void apply_boundary_conditions(Matrix *M, Matrix *K, Matrix *Mr, Matrix *Kr, size_t *nodes, size_t n_boundary_nodes){
    int row = 0; int col = 0;
    for (int i = 0; i < M->m; i++)
    {
        if (!is_in_array(i,nodes,2*n_boundary_nodes))
        {   
            col = 0;
            for (int j = 0; j < M->n; j++)
            {
                if (!is_in_array(j,nodes,2*n_boundary_nodes))
                {
                    Mr->a[row][col] = M->a[i][j];
                    Kr->a[row][col] = K->a[i][j];
                    col++; 
                }
            }
            row++;    
        }
    }
}


 #define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })


int main (int argc, char *argv[]) {

    if (argc < 2){
        printf("Usage: \n"
                "./project <k> <out> <r1> <r2> <e> <l> <meshSize>\n" 
                "---------------------------- \n\n"
                "- k is the number of frequencies to compute. \n "
                "- out is the output file to write the frequencies. \n "
        " r1 = inner radius (half-distance between prongs) \n"
        " r2 = outer radius (half-width of fork) \n"
        " e  = length of handle \n"
        " l  = length of prongs \n"
        " meshsize = 0.3 for precision and 0.5 for speed\n"
        "\n");
        return -1;
    }

    
    // Define physical constants
    double E = 0.7e11;  // Young's modulus for Aluminum
    double nu = 0.3;    // Poisson coefficient
    double rho = 3000;  // Density of Aluminum

        
    int ierr;
    gmshInitialize(argc, argv, 0, 0, &ierr);

    designTuningFork(atof(argv[3]), atof(argv[4]), atof(argv[5]), atof(argv[6]), atof(argv[7]), NULL); // 0.3 à la place de 0.5
    
    // Number of vibration modes to find
    int k = atoi(argv[1]);

    // Assemble the 2 matrices of the linear elasticity problem: 
    // M is the mass matrix
    // K is the stiffness matrix
    Matrix *K, *M;
    size_t* boundary_nodes;
    size_t n_boundary_nodes;
    double * coord;
    
    printf("============ Assembling system ==================\n");
    assemble_system(&K, &M, &coord, &boundary_nodes, &n_boundary_nodes, E, nu, rho);

    //computing reduced matrix
    Matrix *Mr = allocate_matrix(M->m-2*n_boundary_nodes, M->n-2*n_boundary_nodes);
    Matrix *Kr = allocate_matrix(K->m-2*n_boundary_nodes, K->n-2*n_boundary_nodes);
    size_t *nodes = (size_t *)malloc(sizeof(size_t)*2*n_boundary_nodes);

    for (int i = 0; i < n_boundary_nodes; i++)
    {
        nodes[2*i] = 2*boundary_nodes[i];
        nodes[2*i+1] = 2*boundary_nodes[i]+1;
    }


    printf("=============== Apply boundary conditions==============\n");
    apply_boundary_conditions(M, K, Mr, Kr, nodes, n_boundary_nodes);

    double *new_coord = (double *)malloc(Kr->m*sizeof(double));
    int id = 0;

    for (int i = 0; i < K->m; i++) {
        if (!is_in_array(i/2, boundary_nodes, n_boundary_nodes)) {
            new_coord[id] = coord[i];
            id++;
        }
    }

    
    printf("================Starting routine ===========\n");
    int N = Mr->m;
    int LDZ = Mr->m;
    int LDA = N;
    int LDB = N;


    double *w = malloc(sizeof(double)*N);
    double Z[LDZ*N];
    int info,m,iFail;
    double abstol = LAPACKE_dlamch('S');
    int lda = LDA, ldb = LDB, ldz = LDZ;

    
    double tic = get_wtime();
    info = LAPACKE_dsygv(LAPACK_ROW_MAJOR, 1, 'N', 'U', N, Kr->data, lda, Mr->data, ldb, w);
    double tac = get_wtime();

    printf("Time for solving : %f\n",tac-tic);

    FILE *fd = fopen(argv[2],"w");
    for (int i = 0; i < k; i++) {
        printf("EIGENVALUES : %.20f\n",w[i]);
        double eig = w[i];
        double freq = sqrt(eig)/(2*M_PI);
        printf("FREQUENCE : %.12f\n",freq);
        if (i == k-1) {
            fprintf(fd, "%.12f",freq);
        }else {
            fprintf(fd, "%.12f ",freq);
        }

    }

    fclose(fd);
    free(new_coord);
    free(w);
    free_matrix (K);
    free_matrix (M);
    free_matrix(Kr);
    free_matrix(Mr);
    free(boundary_nodes);
    free(coord);

    return 0;
}
