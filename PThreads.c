#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 2048
#define MAX_ITER 2000
#define MAX_THREADS 4
#define STEP N / MAX_THREADS

typedef struct viz_t{
    float media;
    int vivos;
}viz_t;

typedef struct args_t{
    int start;
    float **grid;
    float **new_grid;
}args_t;

void alocarMatriz(float ***matriz);
void desalocarMatriz(float **matriz);
void vizinhos(viz_t *viz, float** grid, int x, int y);
void *threadFunc(void *arg);

int main(){

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

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            if (grid[i][j] != 0.0){
                celulas_vivas++;
            }
        }
    }

    printf("Condicao Inicial: %d\n", celulas_vivas);
    pthread_t t[MAX_THREADS];
    args_t *args = malloc(sizeof(args_t));
    args->grid = grid;
    args->new_grid = new_grid;
    args->start = 0;
    for(int i = 0; i < MAX_THREADS; i++){
        pthread_create(&t[i], NULL, threadFunc, (void *) args);
        args->start += STEP;
    }
    //esperar threads
    for(int i = 0; i < MAX_THREADS; i++){
        pthread_join(t[i], NULL);
    }
    desalocarMatriz(grid);
    desalocarMatriz(new_grid);

    pthread_exit(NULL);
}


void *threadFunc(void *arg){
    printf("entrando na thread");
    args_t *args = (args_t *) arg;
    int celulas_vivas;
    for (int i = 1; i <= MAX_ITER; i++){
        celulas_vivas = 0;

        for (int j = args->start; j < args->start + STEP; j++){
            for (int k = 0; k < N; k++){
                viz_t viz;
                viz.media = 0.0;
                viz.vivos = 0;
                vizinhos(&viz, args->grid, j, k);

                if (args->grid[j][k] != 0.0){ // celula atual viva
                    if (viz.vivos < 2 || viz.vivos > 3) args->new_grid[j][k] = 0.0;
                    else{
                        args->new_grid[j][k] = 1.0;
                        celulas_vivas++;
                    }
                }
                else{ // celula atual morta
                    if (viz.vivos == 3){
                        args->new_grid[j][k] = viz.media;
                        celulas_vivas++;
                    }
                    else args->new_grid[j][k] = 0.0;
                }
            }
        }

        float **aux = args->grid;
        args->grid = args->new_grid;
        args->new_grid = aux;
        printf("Geracao %d: %d\n", i, celulas_vivas);
    }
    pthread_exit(NULL);
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
            
            if(grid[i][j] != 0.0) viz->vivos++;
            viz->media += grid[i][j];
            i = aux_i;
            j = aux_j;      
        }
    }
    viz->media /= 8.0;
}