/* 
* Trabalho 4 - Estrutura de dados II (SCC0224)
* Amanda Araujo Silva
* ICMC-USP 2024

* Caminhos mínimos em grafos 
* Algoritmo de Dijkstra & Algoritmo Floyd-Warshall
*/

#include <stdio.h>
#include <malloc.h>
#include <limits.h>

// Definição estruturas
typedef struct{ 
    int v_i;
    int v_j;
} PAR;

/* typedef struct{
    PAR *inicio;
} LISTA_PARES; */

// Protótipo de funções
void dijsktra(int G, int n);
void floyd_warshall(int **G, int n);
void inicializar(PAR *lista_pares, int K);

int main()
{
    // Informações
    int T, N, M;
    scanf("%d", &T);
    scanf("%d %d", &N, &M);

    // Estrutura grafo: matriz de adjacências
    // alocação vetor ponteiros para linhas
    int **G = malloc(N * sizeof(int*));

    // alocação cada uma das linhas
    for (int i = 0; i < N; i++){
        G[i] = malloc(N * sizeof(int)); 
    } 

    // Inicializar grafo
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            G[i][j] = 0;
        }
    }

    // Leitura grafo: matriz de adjacências (arestas (v_i, v_j) e custo (w))
    int v_i, v_j, w;
    for (int i = 0; i < M; i++)
    {
        scanf("%d %d %d", &v_i, &v_j, &w);
        G[v_i][v_j] = w;    // adicionar no grafo
    }

    // Informações caminhos
    int K;
    scanf("%d", &K);

    // Estrutura: lista pares
    PAR *lista_pares = (PAR *) malloc(K * sizeof(PAR));

    // Inicialização lista pares
    inicializar(lista_pares, K);

    // Leitura: vértices caminhos 
    for (int i = 0; i < K; i++){
        scanf("%d %d", &v_i, &v_j); // armazenamento caminhos
        lista_pares[i].v_i = v_i;
        lista_pares[i].v_j = v_j;
    }

    // Matriz A: custo caminho mínimos todos para todos
    // alocação vetor ponteiros para linhas
    int **A = malloc(N * sizeof(int*));

    // alocação cada uma das linhas
    for (int i = 0; i < N; i++){
        A[i] = malloc(N * sizeof(int)); 
    } 

    switch (T)
    {
    case 1:
        //dijsktra(G, N);
        // um para todos

        break;

    case 2:

        // Inicialização da matriz de custos A(0): custo inicial
        for (int u = 0; u < N; u++){
            for (int v = 0; v < N; v++){
                if (u == v) A[u][v] = 0;        // diagonal nula (sem custo autoloop)
                else if (G[u][v] == 0) A[u][v] = INT_MAX; // custo infinito: não há aresta (u, v) 
                else A[u][v] = G[u][v];                   // peso aresta (u, v)                
            }
        }
        
        // Mínimos caminhos
        floyd_warshall(A, N);

        // Custo dos caminhos
        for (int i = 0; i < K; i++){
            v_i = lista_pares[i].v_i;
            v_j = lista_pares[i].v_j;
            if (A[v_i][v_j] != INT_MAX) printf("%d\n", A[v_i][v_j]);
            else printf("-1\n");  
        }

        break;
    
    default:
        break;
    }

    // Liberação de memória alocada
    free(G);
    free(lista_pares);
    
    return 0;
}

// FUNÇÕES --------------------------------------------------------------

// Inicialização lista pares
void inicializar(PAR *lista_pares, int K){
    for (int i = 0; i < K; i++){
        lista_pares[i].v_i = -1;
        lista_pares[i].v_j = -1;
    }
}

// Algoritmo Floyd-Warshall
void floyd_warshall(int **A, int n){
    
    // Variáveis internas
    int k, u, v;

    // Algoritmo (On^3)
    for (k = 0; k < n; k++){
        for (u = 0; u < n; u++){
            for (v = 0; v < n; v++){
                if (A[u][k] != INT_MAX && A[k][v] != INT_MAX && A[u][k] + A[k][v] < A[u][v]){
                    A[u][v] = A[u][k] + A[k][v]; 
                }  
            }
        }
    }  

    return;
}

// Algoritmo de Dijkstra
// relaxamento
void relax(int u, int v, int w){
    if (d[v] > d[u] + W[u][v]) d[v] = d[u] + W[u][v]; 
}

void dijsktra(int **G, int N, int v_inicial){
    
    // Vetor de custo do caminho
    int *d = (int) malloc(N * sizeof(int)); 

    // Inicialização custo caminho (v_inicial = 0)
    for (int i = 0; i < N; i++){
        if (i == v_inicial) d[i] = 0;
        else d[i] = INT_MAX;
    }

    // Vetor solução
    int *sol = (int) malloc(N * sizeof(int));

    // Criação fila de prioridade dos vértices (v_inicial primeiro)
    // F: heap
    // rearranjar-heap

    // Inserção vértice inicial na solução
    //sol[0] = v_inicial;

    // Relaxamento arestas adjacentes 
    //while (F =! NULL){
        // u = retira vértice de F
        // S
    }




}