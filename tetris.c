#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// Estrutura da peça
typedef struct {
    char nome;
    int id;
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[MAX_FILA];
    int frente, tras, tamanho;
} Fila;

// Estrutura da pilha
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

// Inicializa fila
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = 0;
    f->tamanho = 0;
}

// Inicializa pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Gera uma peça aleatória
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = id;
    return p;
}

// Enfileirar
void enqueue(Fila *f, Peca p) {
    f->itens[f->tras] = p;
    f->tras = (f->tras + 1) % MAX_FILA;
    f->tamanho++;
}

// Desenfileirar
Peca dequeue(Fila *f) {
    Peca p = f->itens[f->frente];
    f->frente = (f->frente + 1) % MAX_FILA;
    f->tamanho--;
    return p;
}

// Empilhar
int push(Pilha *p, Peca item) {
    if (p->topo == MAX_PILHA - 1) return 0;
    p->itens[++p->topo] = item;
    return 1;
}

// Desempilhar
int pop(Pilha *p, Peca *ret) {
    if (p->topo == -1) return 0;
    *ret = p->itens[p->topo--];
    return 1;
}

// Exibir estruturas
void exibir(Fila *f, Pilha *p) {
    printf("\nFila de peças:\n");
    for (int i = 0; i < f->tamanho; i++) {
        int idx = (f->frente + i) % MAX_FILA;
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
    }

    printf("\n\nPilha de reserva (Topo -> Base):\n");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n\n");
}

int main() {
    Fila fila;
    Pilha pilha;
    int opcao, id = 0;
    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa fila cheia
    for (int i = 0; i < MAX_FILA; i++)
        enqueue(&fila, gerarPeca(id++));

    do {
        exibir(&fila, &pilha);

        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        if (opcao == 1) { // Jogar
            dequeue(&fila);
            enqueue(&fila, gerarPeca(id++));
        }
        else if (opcao == 2) { // Reservar
            if (push(&pilha, dequeue(&fila)))
                enqueue(&fila, gerarPeca(id++));
            else
                printf("Reserva cheia!\n");
        }
        else if (opcao == 3) { // Usar reserva
            Peca usada;
            if (pop(&pilha, &usada))
                printf("Peça usada: [%c %d]\n", usada.nome, usada.id);
            else
                printf("Sem peças reservadas!\n");
        }

    } while (opcao != 0);

    return 0;
}
