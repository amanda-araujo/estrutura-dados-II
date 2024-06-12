/* 
* Trabalho 2 - Estrutura de dados II (SCC0224)
* Amanda Araujo Silva
* ICMC-USP 2024
*/

#include <stdio.h>
#include <malloc.h>
#include <string.h>

// Definição da estrutura
typedef struct{
    float mp;       // média ponderada
    int dp;         // reprovações
    char nome[50]; // nome
} REGISTRO;

void insertion_sort(REGISTRO *l, int n);
void heap_sort(REGISTRO *l, int n);
void rearranjar_heap_min(REGISTRO *l, int i, int n);
void construir_heap_min(REGISTRO *l, int n);
int comparar(REGISTRO a, REGISTRO b);

int main()
{
    // Informações 
    int T, N, M; // método ordenação, nº aplicantes, nº bolsas 
    scanf("%d", &T);
    scanf("%d %d", &N, &M);

    // Estrutura lista aplicações: tamanho nº aplicantes
    // alocação dinâmica: malloc
    REGISTRO *lista = (REGISTRO *) malloc(N * sizeof(REGISTRO));

    // Leitura aplicações
    for(int i = 0; i < N; i++){
        scanf("%f %d %s", &lista[i].mp, &lista[i].dp, lista[i].nome); //%[^\n]: nome composto
    }  
    
    /* for(int i = 0; i < N; i++){
        printf("Média ponderada: %2f, Nº reprovações: %d, Aluno: %s\n", lista[i].mp, lista[i].dp, lista[i].nome);
    } */

    //Ordenação (1 - mp; 2 - dp; empate >> ordem alfabética)
    switch (T)
    {
    case 1:
        insertion_sort(lista, N);
        break; 
    
    case 2:
        heap_sort(lista, N);
        break;
    
    default:
        break;
    }

    // Saída (output): nº bolsas concedidas & lista de aprovados - nome
    // Regras: max dp = 10
    int cont = 0;
    int limit = M;
    
    // Conta o número de alunos que receberão a bolsa, considerando empates 
    for (int i = 0; i < N; i++) {
        if (lista[i].dp <= 10) {
            cont++;
        }
        if (cont == limit) {
            while (i + 1 < N && comparar(lista[i], lista[i + 1]) == 0) {
                i++;
                cont++;
            }
            break;
        }
    }
    printf("%d\n", cont);

    cont = 0;

    // Imprime os alunos selecionados, considerando empates 
    for (int i = 0; i < N; i++) {
        if (lista[i].dp <= 10) {
            printf("%s\n", lista[i].nome);
            cont++;
        }
        if (cont == limit) {
            while (i + 1 < N && comparar(lista[i], lista[i + 1]) == 0) {
                i++;
                printf("%s\n", lista[i].nome);
                cont++;
            }
            break;
        }
    }

    // Liberação de memória alocada
    free(lista);
    return 0;
}


// FUNÇÕES --------------------------------------------------------------------------------------------
void insertion_sort(REGISTRO *l, int n) {
    // Ordenação decrescente com critério de desempate: mp decrescente, dp crescente, nome alfabético
    for (int i = 1; i < n; i++) {
        // Elemento a ser inserido
        REGISTRO aux = l[i];
        int j = i - 1;

        // Olha para o sub-vetor e desloca elementos maiores (ou iguais) à direita
        // Deslocamento em 3 casos: 1) mp <; 2) mp =, desempate dp >; 3) mp =, dp =, desempate nome  
        while (j >= 0 && (l[j].mp < aux.mp || (l[j].mp == aux.mp && l[j].dp > aux.dp) ||
                          (l[j].mp == aux.mp && l[j].dp == aux.dp && strcmp(aux.nome, l[j].nome) < 0))){
            l[j + 1] = l[j];
            j--;
        }

        // Insere o elemento na posição correta no sub-vetor ordenado
        l[j + 1] = aux;
    }
}

void rearranjar_heap_min(REGISTRO *l, int i, int n){
    int fe, fd, mf; //filho esquerdo, filho direito, menor filho

    // Identifica filhos esquerdo e direito
    fe = (2 * i + 1 < n) ? 2 * i + 1 : -1;
    fd = (2 * i + 2 < n) ? 2 * i + 2 : -1;

    // Se i folha: fim
    if (fe == -1){
        return;
    }

    // Identifica menor filho
    // Caso 1) um filho;
    mf = fe;
    // Caso 2) dois filhos >>> comparação com todos os critérios
    if (fd != -1){
        if (l[fd].mp < l[mf].mp || // Verifica menor mp
            (l[fd].mp == l[mf].mp && l[fd].dp > l[mf].dp) || // Em caso de empate em mp, verifica maior dp
            (l[fd].mp == l[mf].mp && l[fd].dp == l[mf].dp && strcmp(l[fd].nome, l[mf].nome) > 0)) // Em caso de empate em mp e dp, verifica nome em ordem alfabética
        {
            mf = fd; // Atualiza o menor filho
        }
    }

    if (l[i].mp < l[mf].mp ||
        (l[i].mp == l[mf].mp && l[i].dp > l[mf].dp) ||
        (l[i].mp == l[mf].mp && l[i].dp == l[mf].dp && strcmp(l[i].nome, l[mf].nome) > 0)){
            return;
        }

    // Troca elemento da posição i pelo menor filho
    REGISTRO aux = l[i];
    l[i] = l[mf];
    l[mf] = aux;

    // Chamada recursiva: rearranja a partir da posição do menor filho
    rearranjar_heap_min(l, mf, n); // Arruma próximo nível
}

void construir_heap_min(REGISTRO *l, int n){
    for (int i = n/2 - 1; i >= 0; i--){
        rearranjar_heap_min(l, i, n);
    }
}

void heap_sort(REGISTRO *l, int n){

    // Contruir heap mínima
    construir_heap_min(l, n);

    for (int i = n - 1; i > 0; i--){
        // Seleção do menor elemento: troca raiz para última posição
        REGISTRO aux = l[i];
        l[i] = l[0];
        l[0] = aux;

        // Diminuir tamanho heap
        n--;

        //Rearranjar heap mínima
        rearranjar_heap_min(l, 0, n);
    }
}

// Problema: empates (importante: mp = e dp = >> ambos serão chamados)
int comparar(REGISTRO a, REGISTRO b) {
    if (a.dp > b.dp) return -1;
    if (a.dp < b.dp) return 1;
    return 0; // Empate
}



