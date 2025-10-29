#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5  // Tamanho máximo da fila de peças

// Estrutura para representar uma peça
typedef struct {
    char nome;  // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;     // Identificador único da peça
} Peca;

// Estrutura para representar a fila circular
typedef struct {
    Peca itens[MAX_FILA];
    int frente;  // Índice da frente da fila
    int tras;    // Índice do final da fila
    int tamanho; // Quantidade de elementos na fila
} Fila;

// Função para inicializar a fila
void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->tras = 0;
    fila->tamanho = 0;
}

// Função para gerar uma peça aleatória
Peca gerarPeca(int id) {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L'};
    int indice = rand() % 4;
    novaPeca.nome = tipos[indice];
    novaPeca.id = id;
    return novaPeca;
}

// Função para adicionar uma peça ao final da fila (enqueue)
int enqueue(Fila *fila, Peca peca) {
    if (fila->tamanho == MAX_FILA) {
        printf("Fila cheia! Não é possível adicionar nova peça.\n");
        return 0;
    }
    fila->itens[fila->tras] = peca;
    fila->tras = (fila->tras + 1) % MAX_FILA;
    fila->tamanho++;
    return 1;
}

// Função para remover a peça da frente da fila (dequeue)
int dequeue(Fila *fila) {
    if (fila->tamanho == 0) {
        printf("Fila vazia! Não há peças para jogar.\n");
        return 0;
    }
    Peca removida = fila->itens[fila->frente];
    printf("Peça jogada: [%c %d]\n", removida.nome, removida.id);
    fila->frente = (fila->frente + 1) % MAX_FILA;
    fila->tamanho--;
    return 1;
}

// Função para exibir a fila de peças
void exibirFila(Fila *fila) {
    printf("Fila de peças:\n");
    for (int i = 0; i < fila->tamanho; i++) {
        int indice = (fila->frente + i) % MAX_FILA;
        printf("[%c %d] ", fila->itens[indice].nome, fila->itens[indice].id);
    }
    printf("\n");
}

// Função principal
int main() {
    Fila fila;
    int opcao;
    int contadorId = 0;  // Contador para gerar IDs únicos
    srand(time(NULL));   // Inicializa a semente aleatória

    inicializarFila(&fila);

    // Inicializa a fila com 5 peças
    for (int i = 0; i < MAX_FILA; i++) {
        Peca p = gerarPeca(contadorId++);
        enqueue(&fila, p);
    }

    do {
        exibirFila(&fila);
        printf("\nOpções de ação:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha a opção: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                dequeue(&fila);
                break;
            case 2:
                Peca nova = gerarPeca(contadorId++);
                enqueue(&fila, nova);
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
        printf("\n");
    } while(opcao != 0);

    return 0;
}
