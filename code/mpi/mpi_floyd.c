#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

// INFINITY means no direct path 
const int INFINITY = 1000000;

void read_matrix(int matrix[], int n, int myid, int p, MPI_Comm comm);
void print_matrix(int matrix[], int n, int myid, int p, MPI_Comm comm);
void run_floyd(int matrix[], int n, int myid, int p, MPI_Comm comm);
int get_myid(int k, int p, int n);
void get_rowk(int matrix[], int n, int p, int row_k[], int k);

int main(int argc, char* argv[]) {
    int n;
    int* matrix;
    MPI_Comm comm;
    int p, myid;
    double start, finish;

    MPI_Init(&argc, &argv);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &p);
    MPI_Comm_rank(comm, &myid);

    if (myid == 0) {
      // read the number of vertices
        scanf("%d", &n);
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, comm);
    matrix = malloc(n * n / p * sizeof(int));

    read_matrix(matrix, n, myid, p, comm);
    if (myid == 0) {
      // start counting time
      start = MPI_Wtime();
    }

    run_floyd(matrix, n, myid, p, comm);

    if (myid == 0) {
      finish = MPI_Wtime();
      printf("time elapsed %f\n", finish - start);
    }
    // print solution if necessary
    // print_matrix(matrix, n, myid, p, comm);

    free(matrix);
    MPI_Finalize();

    return 0;
}

int get_myid(int k, int p, int n) { return k / (n / p); }

void get_rowk(int matrix[], int n, int p, int row_k[], int k) {
    int j;
    int local_k = k % (n / p);

    for (j = 0; j < n; j++) row_k[j] = matrix[local_k * n + j];
}

void run_floyd(int matrix[], int n, int myid, int p, MPI_Comm comm) {
    int k, i, j, dist;
    int root;
    int* row_k = malloc(n * sizeof(int));

    for (k = 0; k < n; k++) {
        root = get_myid(k, p, n);
        if (myid == root) get_rowk(matrix, n, p, row_k, k);
        MPI_Bcast(row_k, n, MPI_INT, root, comm);
        for (i = 0; i < n / p; i++)
            for (j = 0; j < n; j++) {
                dist = matrix[i * n + k] + row_k[j];
                if (dist < matrix[i * n + j]) matrix[i * n + j] = dist;
            }
    }
    free(row_k);
}

void read_matrix(int matrix[], int n, int myid, int p, MPI_Comm comm) {
    int i, j;
    int* tmp = NULL;

    if (myid == 0) {
        tmp = malloc(n * n * sizeof(int));
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++) scanf("%d", &tmp[i * n + j]);
        MPI_Scatter(tmp, n * n / p, MPI_INT, matrix, n * n / p, MPI_INT,
                    0, comm);
        free(tmp);
    } else {
        MPI_Scatter(tmp, n * n / p, MPI_INT, matrix, n * n / p, MPI_INT,
                    0, comm);
    }

}


void print_matrix(int matrix[], int n, int myid, int p, MPI_Comm comm) {
    int i, j;
    int* tmp = NULL;

    if (myid == 0) {
        tmp = malloc(n * n * sizeof(int));
        MPI_Gather(matrix, n * n / p, MPI_INT, tmp, n * n / p, MPI_INT,
                   0, comm);
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++)
                if (tmp[i * n + j] == INFINITY)
                    printf("i ");
                else
                    printf("%d ", tmp[i * n + j]);
            printf("\n");
        }
        free(tmp);
    } else {
        MPI_Gather(matrix, n * n / p, MPI_INT, tmp, n * n / p, MPI_INT,
                   0, comm);
    }
}
