#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 2048
#define MAX_ITER 2000
#define MAX_THREADS 4
#define STEP (N / MAX_THREADS)

typedef struct viz_t{
    float media;
    int vivos;
} viz_t;

typedef struct args_t{
    int start;
    float **grid;
    float **new_grid;
} args_t;

void alocarMatriz(float ***matriz);
void desalocarMatriz(float **matriz);
void copiarMatriz(float **m1, float **m2);
void vizinhos(viz_t *viz, float **grid, int x, int y);
void *threadFunc(void *arg);
int celulasVivas(float **grid);

int main() {
    float **grid, **new_grid;
    alocarMatriz(&grid);
    alocarMatriz(&new_grid);

    int lin = 1, col = 1;
    grid[lin][col + 1] = 1.0;
    grid[lin + 1][col + 2] = 1.0;
    grid[lin + 2][col] = 1.0;
    grid[lin + 2][col + 1] = 1.0;
    grid[lin + 2][col + 2] = 1.0;
    lin = 10, col = 30;
    grid[lin][col + 1] = 1.0;
    grid[lin][col + 2] = 1.0;
    grid[lin + 1][col] = 1.0;
    grid[lin + 1][col + 1] = 1.0;
    grid[lin + 2][col + 1] = 1.0;

    printf("Condicao Inicial: %d\n", celulasVivas(grid));
    pthread_t t[MAX_THREADS];
    args_t args[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; i++) {
        alocarMatriz(&(args[i].grid));
        alocarMatriz(&(args[i].new_grid));
        copiarMatriz(args[i].grid, grid);
        copiarMatriz(args[i].new_grid, new_grid);
        
        args[i].start = i * STEP;
    }

    for (int k = 1; k <= MAX_ITER; k++) {
        for (int i = 0; i < MAX_THREADS; i++) pthread_create(&t[i], NULL, threadFunc, (void *)&args[i]);
        for (int i = 0; i < MAX_THREADS; i++) pthread_join(t[i], NULL);

        //printando uma matriz
        // for(int i = 0; i < 512; i++){
        //     for(int j = 0; j < 512; j++){
        //         if(args[0].grid[i][j] != 0.0)
        //             printf("%f ", args[0].grid[i][j]);
        //     }
        // }

        for(int k = 0; k < MAX_THREADS; k++){
            for (int i = 0; i < N; i++) {
                for (int j = k * STEP; j < (k * STEP) + STEP; j++) {
                    new_grid[j][i] = args[k].new_grid[j][i];
                }
                printf("print %d\n", k);
            }
        }
         
        printf("entra aqui");

        grid = new_grid;
        printf("Geracao %d: %d\n", k, celulasVivas(new_grid));
    }

    desalocarMatriz(grid);
    desalocarMatriz(new_grid);

    pthread_exit(NULL);
}

int celulasVivas(float **grid) {
    int celulas_vivas = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] != 0.0) {
                celulas_vivas++;
            }
        }
    }
    return celulas_vivas;
}

void *threadFunc(void *arg) {
    //printf("Entrando na thread\n");
    args_t *args = (args_t *)arg;

    for (int j = args->start; j < args->start + STEP; j++) {
        for (int k = 0; k < N; k++) {
            viz_t viz;
            viz.media = 0.0;
            viz.vivos = 0;
            vizinhos(&viz, args->grid, j, k);

            if (args->grid[j][k] != 0.0) { // celula atual viva
                if (viz.vivos < 2 || viz.vivos > 3)
                    args->new_grid[j][k] = 0.0;
                else
                    args->new_grid[j][k] = 1.0;
            } else { // celula atual morta
                if (viz.vivos == 3)
                    args->new_grid[j][k] = viz.media;
                else
                    args->new_grid[j][k] = 0.0;
            }
        }
    }

    pthread_exit(NULL);
}


void alocarMatriz(float ***matriz) {
    *matriz = (float **)malloc(N * sizeof(float *));
    for (int i = 0; i < N; i++) (*matriz)[i] = (float *)malloc(N * sizeof(float));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            (*matriz)[i][j] = 0.0;
        }
    }
}


void desalocarMatriz(float **matriz) {
    for (int i = 0; i < N; i++) free(matriz[i]);
    free(matriz);
}


void copiarMatriz(float **m1, float **m2){
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            m1[i][j] = m2[i][j];
        }
    }
}

void vizinhos(viz_t *viz, float **grid, int x, int y) {
    int aux_i, aux_j;

    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (i == x && j == y) continue;

            aux_i = i;
            aux_j = j;
            // simular borda infinita
            if (i < 0) i = 2047;
            else if (i >= N) i = 0;
            if (j < 0) j = 2047;
            else if (j >= N) j = 0;

            if (grid[i][j] != 0.0) viz->vivos++;
            viz->media += grid[i][j];
            i = aux_i;
            j = aux_j;
        }
    }
    viz->media /= 8.0;
}
