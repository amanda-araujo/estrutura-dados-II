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

#define false 0
#define true 1
typedef int bool;

// Definição estruturas
typedef struct{ 
    int v_i;
    int v_j;
} PAR;

// Protótipo de funções
int *dijsktra(int **G, int n, int v_inicial);
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
    int **G = (int **) malloc(N * sizeof(int *));

    // alocação cada uma das linhas
    for (int i = 0; i < N; i++){
        G[i] = (int *) malloc(N * sizeof(int)); 
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
        lista_pares[i].v_i = v_i;   // vértice origem
        lista_pares[i].v_j = v_j;   // vértice destino
    }

    // Algoritmos caminhos mínimos
    // "Switch case" (comando switch não permite declaração dentro de cada case!)
    if (T == 1){
        // Dijkstra: custo caminho mínimo origem para todos

        // Custo dos caminhos
        for (int i = 0; i < K; i++){
            // Par (v_i, v_j)
            v_i = lista_pares[i].v_i; // vértice origem
            v_j = lista_pares[i].v_j; // vértice destino
            
            // Algoritmo de Dijkstra
            int *d = dijsktra(G, N, v_i);

            // Custo do caminho (v_i, v_j)
            if (d[v_j] != INT_MAX) printf("%d\n", d[v_j]);
            else printf("-1\n");  
        }        
    }

    else if (T == 2){
        // Floyd-Warshall: custo caminhos mínimos todos para todos
        
        // Matriz A: custo caminho mínimos
        // alocação vetor ponteiros para linhas
        int **A = (int **) malloc(N * sizeof(int *));

        // alocação cada uma das linhas
        for (int i = 0; i < N; i++){
            A[i] = (int *) malloc(N * sizeof(int)); 
        }

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

        // Liberação de memória
        // libera memória da matriz A
        for (int i = 0; i < N; i++) free(A[i]);
        free (A);
    }

    else printf("T inválido!\n");

    // Liberação de memória alocada
    // libera memória do grafo 
    for (int i = 0; i < N; i++) free(G[i]);
    free (G);
    // libera memória lista de pares 
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

    return;
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
void relax(int **G, int *d, int N, int u){
    // relaxamento arestas adjacentes a u
    for (int v = 0; v < N; v++){
        if (G[u][v] != 0 && d[u] != INT_MAX && d[v] > d[u] + G[u][v]){
            d[v] = d[u] + G[u][v];
        } 
    }

    return;
}

int * dijsktra(int **G, int N, int v_inicial){
    // output: Retorna vetor de distâncias
    int v;

    // Vetor de distâncias
    int *d = (int *) malloc(N * sizeof(int)); 

    // Inicialização custo caminho (v_inicial = 0, outros: infinito)
    for (v = 0; v < N; v++){
        if (v == v_inicial) d[v] = 0;
        else d[v] = INT_MAX;
    }

    // Vetor aberto ("fila", nós que ainda não estão na solução)
    bool *aberto = (bool *) malloc(N * sizeof(bool));

    // Inicialização vetor aberto (todos vértices em aberto)
    for (v = 0; v < N; v++) aberto[v] = true;

    while (true){ // fila
        
        // Vértice prioritário na "fila": vértice de menor custo em d que não está em sol
        int custo_min = INT_MAX;
        int u = -1;
        for (int i = 0; i < N; i++){
            if (aberto[i] == true && d[i] < custo_min){
                custo_min = d[i];
                u = i;
            }
        }

        if (u == -1) break; // se não encontrou vértice válido, sai do loop

        aberto[u] = false; // retira da "fila" (fecha u)

        // Relaxamento arestas de u
        relax(G, d, N, u);
 
    }

    // Liberação de memória
    free(aberto);

    return d;
}

// RASCUNHO ----------------------------------------------------------------
// Dijkstra
/* // Vetor solução
    int c = 0;
    int *sol = (int *) malloc(N * sizeof(int));

// Inicializar solução (vazia)
    for (v = 0; v < N; v++) sol[v] = -1; */
// pegar u:
//sol[c] = u;        // adiciona u no conjunto solção
//c++;               // atualiza contador conj solução
/*Check para saber se ainda existe aberto
    /* bool fila = false; // true: ainda há vértices | false: todos os vértices já foram
    for (v = 0; v < N; v++){
        if (aberto[v] == true) fila = true;
    } */
/*// Check se ainda há fila (se existe aberto)
        fila = false;
        for (v = 0; v < N; v++){
            if (aberto[v] == true) fila = true;
        } */
// ou: pegar sempre vértice de menor custo d[v] que NÃO está em sol
    // Criação fila de prioridade dos vértices (v_inicial primeiro)
    // F: heap
    // rearranjar-heap

    // Inserção vértice inicial na solução
    //sol[0] = v_inicial;

    // Relaxamento arestas adjacentes 
    //while (F =! NULL){
        // u = retira vértice de F
        // S