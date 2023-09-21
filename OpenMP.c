#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
//#include <pthread.h>

#define N 2048
#define MAX_ITER 2000
#define MAX_THREADS 2

typedef struct viz_t{
    float media;
    int vivos;
}viz_t;

void alocarMatriz(float ***matriz);
void desalocarMatriz(float **matriz);
void vizinhos(viz_t *viz, float** grid, int x, int y);

int main(){

    struct timeval start_time, end_time;
    double elapsed_time;

    omp_set_num_threads(MAX_THREADS);

    int celulas_vivas = 0;
    float **grid, **new_grid;
    alocarMatriz(&grid);
    alocarMatriz(&new_grid);

    int lin = 1, col = 1;
    grid[lin][col+1] = 1.0;
    grid[lin+1][col+2] = 1.0;
    grid[lin+2][col] = 1.0;
    grid[lin+2][col+1] = 1.0;
    grid[lin+2][col+2] = 1.0;
    lin = 10, col = 30;
    grid[lin][col+1] = 1.0;
    grid[lin][col+2] = 1.0;
    grid[lin+1][col] = 1.0;
    grid[lin+1][col+1] = 1.0;
    grid[lin+2][col+1] = 1.0;

    gettimeofday(&start_time, NULL);

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            if (grid[i][j] != 0.0){
                celulas_vivas++;
            }
        }
    }

    printf("Condicao Inicial: %d\n", celulas_vivas);


    
    for (int i = 1; i <= MAX_ITER; i++){
        celulas_vivas = 0;

            #pragma omp parallel for reduction(+:celulas_vivas)
            for (int j = 0; j < N; j++){
                for (int k = 0; k < N; k++){
                    viz_t viz;
                    viz.media = 0.0;
                    viz.vivos = 0;
                    vizinhos(&viz, grid, j, k);

                    if (grid[j][k] != 0.0){ // celula atual viva
                        if (viz.vivos < 2 || viz.vivos > 3) new_grid[j][k] = 0.0;
                        else{
                            new_grid[j][k] = 1.0;
                            celulas_vivas++;
                        }
                    }
                    else{ // celula atual morta
                        if (viz.vivos == 3){
                            new_grid[j][k] = viz.media;
                            celulas_vivas++;
                        }
                        else new_grid[j][k] = 0.0;
                    }
                }
            
        }

        float **aux = grid;
        grid = new_grid;
        new_grid = aux;
        printf("Geracao %d: %d\n", i, celulas_vivas);
    }

    gettimeofday(&end_time, NULL);

    printf("-------Execução OpenMP finalizada(%d Threads)-------\n", MAX_THREADS);

    elapsed_time = (end_time.tv_sec - start_time.tv_sec) + 
                   (end_time.tv_usec - start_time.tv_usec) / 1000000.0;

    printf("Tempo total de execução: %lf segundos\n", elapsed_time);

    desalocarMatriz(grid);
    desalocarMatriz(new_grid);

    return 0;
}


void alocarMatriz(float ***matriz){
    *matriz = (float **)malloc(N * sizeof(float *));
    for (int i = 0; i < N; i++) (*matriz)[i] = (float *)malloc(N * sizeof(float));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            (*matriz)[i][j] = 0.0;
        }
    }
}


void desalocarMatriz(float **matriz){
    for(int i = 0; i < N; i++) free(matriz[i]);
    free(matriz);
}


void vizinhos(viz_t *viz, float** grid, int x, int y){
    int aux_i, aux_j;


    // int num_threads = omp_get_num_threads();         
    
    // printf("Número de threads: %d\n", num_threads);

    //#pragma omp parallel for private(aux_i, aux_j) shared(viz)
    for(int i = x - 1; i <= x + 1; i++){
        for(int j = y - 1; j <= y + 1; j++){
            if(i == x && j == y) continue;
            
            aux_i = i;
            aux_j = j;
            // simular borda infinita
            if(i < 0) i = 2047;
            else if(i >= N) i = 0;
            if(j < 0) j = 2047;
            else if(j >= N) j = 0;
            
            if(grid[i][j] != 0.0){
                //#pragma omp atomic
                viz->vivos++;
            }
            //#pragma omp atomic
            viz->media += grid[i][j];
            i = aux_i;
            j = aux_j;      
        }
    }
    viz->media /= 8.0;
}