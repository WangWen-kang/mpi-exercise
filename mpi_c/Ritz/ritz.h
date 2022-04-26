#define NPROCMAX 128

void dgemv(int n1,int n2 ,double *a, int lda, double *x, double *y);
void dist_index( int n, int *firstind );
void dist_matrix_colblock( int m, int n, double *a, double *al );
void collect_vector( int n, double *y, double *x );

void dist_matrix_rowblock( int m, int n, double *a, double *al );
void global_vector( int n, double *y, double *x );
