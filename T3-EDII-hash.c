/* 
* Trabalho 3 - Estrutura de dados II (SCC0224)
* Amanda Araujo Silva
* ICMC-USP 2024

* Função de espalhamento (Hashing) 
* Hash estático fechado + sondagem quadrática
*/

#include <stdio.h>
#include <malloc.h>
#include <string.h> // strcpy, strcat, strcmp, strtok
#include <stdlib.h> // str to int

#define TAM_PALAVRA 30             // Define tamanho item tabela hash (palavra)
#define TAM_TEXTO   120000         // Define tamanho máximo texto de input
#define DELETED     "__DELETED__"  // Flag: String especial para marcar entradas removidas

// Definição da estrutura
typedef struct{ // tabela Hash
    char palavra[TAM_PALAVRA];  // entrada dicionário
    int  cont;                  // nº ocorrência da palavra
} ITEM;

// Protótipo de funções
void inicializar(ITEM *tabela, int S);
int  hash       (char *palavra, int S);
int  rehash     (char *palavra, int S, int i, int C1, int C2);
void inserir    (ITEM *tabela, char *palavra, int S, int C1, int C2);
void tamanho    (ITEM *tabela, int S);
int  buscar     (ITEM *tabela, char *palavra, int S, int C1, int C2);
void remover    (ITEM *tabela, char *palavra, int S, int C1, int C2);
void exibir     (ITEM *tabela, int S);


int main()
{
    // Informações
    int S, C1, C2; // tamanho tabela hash, cte 1, cte 2 (sondagem quadrática)
    scanf("%d %d %d%*c", &S, &C1, &C2); 

    // Estrutura: tabela hash
    ITEM *tabela = (ITEM *) malloc(S * sizeof(ITEM)); // Tabela hash: array de ponteiros do tipo ITEM 
    
    // Inicialização tabela hash (diferença espaço vazio e já antes ocupado)
    inicializar(tabela, S);

    // Leitura texto (linha única)
    char linha[TAM_TEXTO] = {0}; // Inicializada com zeros (%c: vazia)
    char *t = NULL;

    // Operações (T = {0, ..., 5})
    int T = -1;   // operação 
    int busca, n; // posição retorno função buscar(), nº de palavras a serem buscadas/removidas

    while (T != 0){
        scanf("%d%*c", &T);

        switch (T)
        {
        case 0: 

            T = 0;
            break;

        case 1:
            
            t = fgets(linha, TAM_TEXTO, stdin); // fgets: char *fgets(char *str, int n, FILE *stream);
    
            // Remove o caractere '\n' da linha, se houver
            linha[strcspn(linha, "\n")] = '\0';

            // Inserir cada palavra
            char *token = strtok(linha, " "); // delimitador: " " (espaço); // retorna o primeiro token 
    
            while (token != NULL){ // obtenção de cada token
                
                inserir(tabela, token, S, C1, C2); // add na tabela hash
                token = strtok(NULL, " ");         // zera token para ler o próximo
            }
            break;

        case 2:

            tamanho(tabela, S);
            break;
        
        case 3:

            scanf("%d%*c", &n); 

            for (int i = 0; i < n; i++)
            {
                t = fgets(linha, TAM_PALAVRA, stdin); 

                // Remove o caractere '\n' da linha, se houver
                linha[strcspn(linha, "\n")] = '\0';

                // Busca na tabela
                busca = buscar(tabela, linha, S, C1, C2);

                // Imprime resultado da busca
                if (busca != -1){
                    printf("%s encontrada %d\n", linha, tabela[busca].cont);
                }
                else printf("%s nao encontrada\n", linha);

            }
            break;

        case 4:
            
            scanf("%d%*c", &n); 

            for (int i = 0; i < n; i++)
            {
                t = fgets(linha, TAM_PALAVRA, stdin); 

                // Remove o caractere '\n' da linha, se houver
                linha[strcspn(linha, "\n")] = '\0';

                // Remoção
                remover(tabela, linha, S, C1, C2);
            }
            break;
        
        case 5:
            
            printf("imprimindo tabela hash\n");
            exibir(tabela, S);
            printf("fim da tabela hash\n");
            break;

        default:
            break;
        }

    }

    // Liberação de memória alocada
    free(tabela);

    return 0;
}


// FUNÇÕES --------------------------------------------------------------------------------------

// Inicialização da tabela hash
/* // Diferença entre espaço vazio nunca antes ocupado e já ocupado
   // '\0': vazio nunca antes ocupado; DELETED: já ocupado
   // contador: 0 */
void inicializar(ITEM *tabela, int S){
    for (int i = 0; i < S; i++){
        tabela[i].palavra[0] = '\0'; // char NULL (vazio nunca antes ocupado)
        tabela[i].cont = 0;          // contador: 0
    }
}

// Inserção tabela hash
void inserir(ITEM *tabela, char *palavra, int S, int C1, int C2){

    // Checar se a palavra já não está presente na tabela; se tiver atualizar contador
    int busca = buscar(tabela, palavra, S, C1, C2);
    if (busca != -1){
        tabela[busca].cont++;  // atualizar contador
        return;
    } 

    // Função de espalhamento: posição na tabela hash
    int pos = hash(palavra, S);
    
    // Checagem posição vazia (== 0: vazia; != 0: colisão)
    // Loop até encontrar posição válida (buscar enquanto não vazia e não marcada com a flag)
    int iter = 1;
    while (strcmp(tabela[pos].palavra, "") != 0 && strcmp(tabela[pos].palavra, DELETED) != 0){
                
        // Palavra repetida
        if (strcmp(tabela[pos].palavra, palavra) == 0){ // palavra repetida
            tabela[pos].cont++;                         // atualizar contador
            return;
        }

        // Colisão: Rehash
        pos = rehash(palavra, S, iter, C1, C2);
        iter++;
    }

    // Inserção
    strcpy(tabela[pos].palavra, palavra); // nova palavra
    tabela[pos].cont++;                   // atualizar contador  

    return;
}

// Função de espalhamento (função de hash)
int hash(char *palavra, int S){ // palavra, tamanho tabela
    int v = 0;
    for (int i = 0; palavra[i] != '\0'; i++){
        v += v * 3 + (int)palavra[i]; // valor ASCII
        v = v % S;                    // evita segfault
    }
    v = v % S;
    return v;
}

// Função rehash: tratamento de colisões
int rehash(char *palavra, int S, int iter, int C1, int C2){
    // Tratamento de colisão: Sondagem quadrática
    // input: palavra, tamanho tabela hash, iteração, cte 1, cte 2 (sondagem quadrática)
    return ( (hash(palavra, S) + C1*iter + C2*iter*iter) % S );
}

// Função retorna tamanho do dicionário e palavra mais frequente
void tamanho(ITEM *tabela, int S){

    int n = 0;  // nº de palavras diferentes
    int freq = 0;   // contagem palavra mais frequente
    int index = 0;  // posição palavra mais frequente

    for (int i = 0; i < S; i++){

        // Contagem de palavras
        if (strcmp(tabela[i].palavra, "") != 0 && strcmp(tabela[i].palavra, DELETED) != 0){ // posição ocupada 
            n++; // + uma palavra

            // Palavra mais frequente (desempate: ordem alfabética)
            if ((tabela[i].cont > freq) || ((tabela[i].cont == freq) && (strcmp(tabela[i].palavra, tabela[index].palavra)) < 0)){
                freq = tabela[i].cont;
                index = i;
            }
        } 
    }

    if (n != 0){
        printf("foram encontradas %d palavras diferentes\n", n);
        printf("palavra mais frequente = %s, encontrada %d vezes\n", tabela[index].palavra, tabela[index].cont);
    }
    else{ // vazio
        printf("nenhuma palavra encontrada");
    }

    return;
}

// Função busca
int buscar(ITEM *tabela, char *palavra, int S, int C1, int C2){
    // Retorna posição da palavra (-1 caso contrário)
    
    // Função de hash
    int pos = hash(palavra, S);
    
    // Condição de parada: 
    /* 1) rodou toda a tabela sem sucesso;
     * 2) encontrou vazio nunca antes ocupado; 
     * 3) encontrou a palavra. */
    int iter = 0;
    while ( iter < S && (tabela[pos].palavra[0] != '\0') && (strcmp(tabela[pos].palavra, palavra) != 0)){
        // Continuar busca
        iter++;
        pos = rehash(palavra, S, iter, C1, C2);
    } 

    if(iter < S && strcmp(tabela[pos].palavra, palavra) == 0) return pos; 
    else return -1;
}

// Função exibição do dicionário (palavras preenchidas)
void exibir(ITEM *tabela, int S){
    
    for (int i = 0; i < S; i++){
        // Exibição de palavras
        if (strcmp(tabela[i].palavra, "") != 0 && strcmp(tabela[i].palavra, DELETED) != 0){ // posição ocupada 
            printf("%s %d\n", tabela[i].palavra, i); 
        }
    }
    return;
}

// Função remoção
void remover(ITEM *tabela, char *palavra, int S, int C1, int C2){
    
    // Buscar palavra na tabela
    int index = buscar(tabela, palavra, S, C1, C2);
    
    if (index < 0) printf("%s nao encontrada\n", palavra);
    else{
        strcpy(tabela[index].palavra, DELETED);  // flag: vazio, mas já antes ocupada
        tabela[index].cont = 0;                  // contador: 0
        printf("%s removida\n", palavra);
    }
    return;
}

// DESCRIÇÃO TAREFA ----------------------------------------------------------------------------
/*  Essa opção [leitura texto] recebe no máximo 100.000 caracteres.
    O tamanho máximo de uma palavra é 30 caracteres.
*/
/* Descrição casos
    Operação T = {0, 1, 2, 3, 4, 5}

    T = 0
    Fim do programa

    T = 1
    Lê em seguida uma linha de texto até atingir o caractere ascii ‘\n’, adicionando
    cada palavra no dicionário

    T = 2
    Exibe o número de palavras diferentes que constam no dicionário até agora, na
    próxima linha exibe a palavra mais frequente do dicionário e quantas vezes ela
    aparece  

    T = 3
    Lê um inteiro n, as próximas n linhas contém uma palavra cada, para cada
    palavra que esteja no dicionário a saída exibe <palavra> encontrada e em seguida o
    número de ocorrências, ou caso contrário exibe “<palavra> nao encontrada”

    T = 4
    Lê um inteiro n, as próximas n linhas contém uma palavra cada, a qual deve
    ser removida do dicionário caso seja encontrada, do contrário nada precisa ser feito.
    A saída exibe <palavra> removida caso a palavra tenha sido encontrada, ou do
    contrário exibe <palavra> nao encontrada.
            
    T = 5
    Exibe todas as palavras contidas no dicionário e suas respectivas posições na
    tabela hash 
 */