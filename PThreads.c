#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 2048

void Aloca_Memoria_Matriz(float **matriz);
void Desaloca_Memoria_Matriz(float **matriz);
int getNeighbors(float** grid, int i, int j);
float get_Values_Neighbors(float** grid, int i, int j);

int main(){

    int Numero_Iteracoes = 2000; // numero de iteracoes

    int Conta_celulas_vivas = 0; // contador de celulas vivas

    float **Tabuleiro_1;
    Aloca_Memoria_Matriz(Tabuleiro_1);// alocacao de matriz dinamicamente
    float **Tabuleiro_2;
    Aloca_Memoria_Matriz(Tabuleiro_2);// alocacao de matriz dinamicamente

    int lin = 1, col = 1; // linha e coluna do tabuleiro

    // Glider
    Tabuleiro_1[lin][col+1] = 1.0;
    Tabuleiro_1[lin+1][col+2] = 1.0;
    Tabuleiro_1[lin+2][col] = 1.0;
    Tabuleiro_1[lin+2][col+1] = 1.0;
    Tabuleiro_1[lin+2][col+2] = 1.0;

    //R-pentomino
    Tabuleiro_1[lin][col+1] = 1.0;
    Tabuleiro_1[lin][col+2] = 1.0;
    Tabuleiro_1[lin+1][col] = 1.0;
    Tabuleiro_1[lin+1][col+1] = 1.0;
    Tabuleiro_1[lin+2][col+1] = 1.0;

    for (int i = 0; i < N; i++){//conta o numero de celulas vivas
        for (int j = 0; j < N; j++){
            if (Tabuleiro_1[i][j] != 0.0){
                Conta_celulas_vivas++;
            }
        }
    }

    printf("Iteração 0: Numero de celulas vivas: %d\n", Conta_celulas_vivas);

    for (int iteracao = 1; iteracao <= Numero_Iteracoes; iteracao++){
        int celulas_vivas_nesta_iteracao = 0;

        for (int j = 0; j < N; j++){
            for (int k = 0; k < N; k++){
                int vizinhos = getNeighbors(Tabuleiro_1, j, k);
                if (Tabuleiro_1[j][k] == 1){
                    if (vizinhos < 2){
                        Tabuleiro_2[j][k] = 0;
                    }
                    else if (vizinhos > 3){
                        Tabuleiro_2[j][k] = 0;
                    }
                    else {
                        Tabuleiro_2[j][k] = 1;
                        celulas_vivas_nesta_iteracao++;
                    }
                }
                else{
                    if (vizinhos == 3){
                        //novas celulas revivem com a média dos valores da vizinhança
                        float Media_valores_vizinhos = get_Values_Neighbors(Tabuleiro_1, j, k) / 8.0;
                        Tabuleiro_2[j][k] = Media_valores_vizinhos;
                        celulas_vivas_nesta_iteracao++;
                    }
                }
            }
        }

        // Troca os tabuleiros
        float **aux = Tabuleiro_1;
        Tabuleiro_1 = Tabuleiro_2;
        Tabuleiro_2 = aux;

        printf("Iteracao %d: Numero de celulas vivas: %d\n", iteracao, celulas_vivas_nesta_iteracao);
    }

    Desaloca_Memoria_Matriz(Tabuleiro_1);// desalocacao de matriz dinamicamente
    Desaloca_Memoria_Matriz(Tabuleiro_2);// desalocacao de matriz dinamicamente

    return 0;
}

void Aloca_Memoria_Matriz(float **matriz){
    int **matriz;
    int i, j;

    // alocacao de memoria para as linhas da matriz
    matriz = (int **) malloc(N * sizeof(int *));

    // alocacao de memoria para as colunas da matriz
    for(i = 0; i < N; i++){
        matriz[i] = (int *) malloc(N * sizeof(int));
    }

    // inicializacao da matriz com 0
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            matriz[i][j] = 0.0;
        }
    }

}

void Desaloca_Memoria_Matriz(float **matriz){
    int i;

    // desalocacao de memoria para as linhas da matriz
    for(i = 0; i < N; i++){
        free(matriz[i]);
    }

    // desalocacao de memoria para as colunas da matriz
    free(matriz);
}

int getNeighbors(float** grid, int i, int j){//funcao que retorna o numero de vizinhos de uma celula
    int vizinhos = 0;
    int i2, j2;

    for(i2 = i-1; i2 <= i+1; i2++){
        for(j2 = j-1; j2 <= j+1; j2++){
            if(i2 == i && j2 == j){//se a celula for a propria celula, nao conta como vizinho
                continue;
            }
            if(i2 < 0 || i2 >= N){//se a celula estiver fora do tabuleiro, nao conta como vizinho
                continue;
            }
            if(j2 < 0 || j2 >= N){//se a celula estiver fora do tabuleiro, nao conta como vizinho
                continue;
            }
            if(grid[i2][j2] != 0.0){//se a celula for viva, conta como vizinho
                vizinhos++;
            }
        }
    }

    return vizinhos;
}

float get_Values_Neighbors(float** grid, int i, int j){//funcao que retorna a soma dos valores dos vizinhos de uma celula
    float soma = 0;
    int i2, j2;

    for(i2 = i-1; i2 <= i+1; i2++){
        for(j2 = j-1; j2 <= j+1; j2++){
            if(i2 == i && j2 == j){
                continue;
            }
            if(i2 < 0 || i2 >= N){
                continue;
            }
            if(j2 < 0 || j2 >= N){
                continue;
            }
            soma += grid[i2][j2];
        }
    }

    return soma;
}