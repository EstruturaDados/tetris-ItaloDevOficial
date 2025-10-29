/*
  tetris_stack.c
  Gerenciador de peças para "Tetris Stack"
  - Fila circular de próximas peças (tamanho 5)
  - Pilha de reserva (tamanho 3)
  Operações:
    1 - Jogar peça (remover frente da fila)
    2 - Enviar peça da fila para a pilha (reservar)
    3 - Usar peça reservada (desempilhar)
    4 - Trocar frente da fila com topo da pilha
    5 - Trocar os 3 primeiros da fila com as 3 peças da pilha (se ambas tiverem 3)
    0 - Sair
  Observações:
    - A cada remoção da fila (jogar ou reservar), uma nova peça é gerada automaticamente
      e enfileirada para manter a fila cheia (quando possível).
    - Peças removidas (jogadas ou usadas da reserva) não retornam ao sistema.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

/* Representação de uma peça */
typedef struct {
    char nome;  /* 'I', 'O', 'T', 'L' */
    int id;     /* id único incremental */
} Peca;

/* Fila circular */
typedef struct {
    Peca itens[MAX_FILA];
    int frente;   /* índice do elemento da frente */
    int tras;     /* índice para inserir (próxima posição livre no final) */
    int tamanho;  /* número atual de elementos */
} Fila;

/* Pilha simples */
typedef struct {
    Peca itens[MAX_PILHA];
    int topo; /* índice do topo (-1 quando vazia) */
} Pilha;

/* --- Protótipos --- */
void inicializarFila(Fila *f);
void inicializarPilha(Pilha *p);
Peca gerarPeca(int id);
int enqueue(Fila *f, Peca p);
int dequeue(Fila *f, Peca *ret);
int push(Pilha *p, Peca item);
int pop(Pilha *p, Peca *ret);
void exibirEstado(const Fila *f, const Pilha *p);
int trocarFrenteComTopo(Fila *f, Pilha *p);
int trocarTres(Fila *f, Pilha *p);

/* --- Implementações --- */

/* Inicializa fila vazia */
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = 0;
    f->tamanho = 0;
}

/* Inicializa pilha vazia */
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

/* Gera peça aleatória com id fornecido */
Peca gerarPeca(int id) {
    char tipos[4] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = id;
    return p;
}

/* Enfileira se houver espaço. Retorna 1 se ok, 0 se cheio */
int enqueue(Fila *f, Peca p) {
    if (f->tamanho == MAX_FILA) return 0; /* cheio */
    f->itens[f->tras] = p;
    f->tras = (f->tras + 1) % MAX_FILA;
    f->tamanho++;
    return 1;
}

/* Desenfileira a peça da frente. Retorna 1 se ok, 0 se vazia. */
int dequeue(Fila *f, Peca *ret) {
    if (f->tamanho == 0) return 0; /* vazia */
    *ret = f->itens[f->frente];
    f->frente = (f->frente + 1) % MAX_FILA;
    f->tamanho--;
    return 1;
}

/* Empilha. Retorna 1 se ok, 0 se cheia */
int push(Pilha *p, Peca item) {
    if (p->topo == MAX_PILHA - 1) return 0; /* cheia */
    p->itens[++(p->topo)] = item;
    return 1;
}

/* Desempilha. Retorna 1 se ok, 0 se vazia */
int pop(Pilha *p, Peca *ret) {
    if (p->topo == -1) return 0; /* vazia */
    *ret = p->itens[(p->topo)--];
    return 1;
}

/* Exibe estado atual de fila e pilha */
void exibirEstado(const Fila *f, const Pilha *p) {
    printf("\n=======================================\n");
    printf("Estado atual:\n\n");

    /* Exibe fila da frente para o final */
    printf("Fila de peças: ");
    if (f->tamanho == 0) {
        printf("(vazia)");
    } else {
        for (int i = 0; i < f->tamanho; i++) {
            int idx = (f->frente + i) % MAX_FILA;
            printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        }
    }
    printf("\n");

    /* Exibe pilha do topo para a base */
    printf("Pilha de reserva (Topo -> Base): ");
    if (p->topo == -1) {
        printf("(vazia)");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
    }
    printf("\n=======================================\n\n");
}

/* Troca a peça da frente da fila com o topo da pilha.
   Retorna 1 se troca realizada, 0 caso contrário. */
int trocarFrenteComTopo(Fila *f, Pilha *p) {
    if (f->tamanho == 0) {
        printf("Não há peça na fila para trocar.\n");
        return 0;
    }
    if (p->topo == -1) {
        printf("Não há peça na pilha para trocar.\n");
        return 0;
    }
    /* Indices */
    int idxFrente = f->frente;
    /* troca simples */
    Peca temp = f->itens[idxFrente];
    f->itens[idxFrente] = p->itens[p->topo];
    p->itens[p->topo] = temp;
    return 1;
}

/* Troca os 3 primeiros da fila com as 3 peças da pilha.
   A correspondência é:
     pilha.top   -> fila.front (posição 0)
     pilha[top-1]-> fila.pos 1
     pilha[top-2]-> fila.pos 2
   Condição: fila deve ter >=3 e pilha deve ter >=3.
   Retorna 1 se efetuada, 0 caso contrário.
*/
int trocarTres(Fila *f, Pilha *p) {
    if (f->tamanho < 3) {
        printf("Fila não tem pelo menos 3 peças para trocar.\n");
        return 0;
    }
    if (p->topo < 2) {
        printf("Pilha não tem pelo menos 3 peças para trocar.\n");
        return 0;
    }

    /* Guardamos os 3 primeiros da fila */
    Peca fila3[3];
    for (int i = 0; i < 3; i++) {
        int idx = (f->frente + i) % MAX_FILA;
        fila3[i] = f->itens[idx];
    }

    /* Guardamos as 3 da pilha (topo -> topo-2) */
    Peca pilha3[3];
    for (int i = 0; i < 3; i++) {
        pilha3[i] = p->itens[p->topo - i]; /* pilha3[0] = topo, pilha3[1] = topo-1, ... */
    }

    /* Coloca as 3 da pilha nas 3 posições iniciais da fila
       Note a ordem: pilha.top -> fila.pos0, pilha[top-1] -> fila.pos1, ... */
    for (int i = 0; i < 3; i++) {
        int idx = (f->frente + i) % MAX_FILA;
        f->itens[idx] = pilha3[i];
    }

    /* Coloca as 3 primeiras da fila na pilha, preservando a ordem tal que
       a antiga fila.pos2 (fila3[2]) vira novo topo da pilha,
       seguindo exemplo especificado no enunciado. Precisamos atribuir:
         novo topo   = fila3[2]
         abaixo topo = fila3[1]
         abaixo      = fila3[0]
       Assim, iteramos e sobrescrevemos p->itens[topo], p->itens[topo-1], p->itens[topo-2].
    */
    for (int i = 0; i < 3; i++) {
        p->itens[p->topo - i] = fila3[2 - i];
    }

    return 1;
}

/* --- Programa principal --- */
int main(void) {
    Fila fila;
    Pilha pilha;
    int opcao;
    int contadorId = 0;

    srand((unsigned) time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    /* Inicializa fila preenchendo com MAX_FILA peças */
    for (int i = 0; i < MAX_FILA; i++) {
        Peca p = gerarPeca(contadorId++);
        enqueue(&fila, p);
    }

    do {
        exibirEstado(&fila, &pilha);

        printf("Opções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("0 - Sair\n");
        printf("Opção escolhida: ");

        if (scanf("%d", &opcao) != 1) {
            /* entrada inválida: consome resto da linha e continua */
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Entrada inválida. Digite um número.\n");
            opcao = -1;
            continue;
        }

        if (opcao == 1) {
            /* Jogar peça: remove frente da fila e gera nova peça para manter fila cheia */
            Peca jogada;
            if (dequeue(&fila, &jogada)) {
                printf("Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                /* gera nova peça e enfileira (tenta manter fila sempre cheia) */
                Peca nova = gerarPeca(contadorId++);
                if (!enqueue(&fila, nova)) {
                    /* caso improvável se houve erro de sincronização */
                    printf("Não foi possível preencher a fila com nova peça.\n");
                } else {
                    printf("Nova peça adicionada à fila: [%c %d]\n", nova.nome, nova.id);
                }
            } else {
                printf("Fila vazia. Nada para jogar.\n");
            }
        }
        else if (opcao == 2) {
            /* Reservar: move frente da fila para topo da pilha se houver espaço.
               Se a pilha estiver cheia, não remove nada. */
            if (pilha.topo == MAX_PILHA - 1) {
                printf("Pilha cheia. Não é possível reservar.\n");
            } else {
                Peca removida;
                if (dequeue(&fila, &removida)) {
                    if (push(&pilha, removida)) {
                        printf("Peça enviada para reserva: [%c %d]\n", removida.nome, removida.id);
                        /* gera nova peça para manter fila cheia */
                        Peca nova = gerarPeca(contadorId++);
                        if (!enqueue(&fila, nova)) {
                            printf("Erro ao repor nova peça na fila.\n");
                        } else {
                            printf("Nova peça adicionada à fila: [%c %d]\n", nova.nome, nova.id);
                        }
                    } else {
                        /* Se o push falhar, devolvemos a peça (raro) */
                        printf("Erro: não foi possível empilhar. Devolvendo peça à fila.\n");
                        enqueue(&fila, removida);
                    }
                } else {
                    printf("Fila vazia. Nada para reservar.\n");
                }
            }
        }
        else if (opcao == 3) {
            /* Usar peça reservada (desempilhar e descartar) */
            Peca usada;
            if (pop(&pilha, &usada)) {
                printf("Peça usada da reserva: [%c %d]\n", usada.nome, usada.id);
                /* NÃO repomos à fila: peça foi usada */
            } else {
                printf("Sem peças na reserva para usar.\n");
            }
        }
        else if (opcao == 4) {
            /* Trocar peça da frente da fila com o topo da pilha */
            if (trocarFrenteComTopo(&fila, &pilha)) {
                printf("Troca entre frente da fila e topo da pilha realizada com sucesso.\n");
            } else {
                printf("Troca não realizada.\n");
            }
        }
        else if (opcao == 5) {
            /* Troca múltipla: 3 primeiros da fila <-> 3 peças da pilha */
            if (trocarTres(&fila, &pilha)) {
                printf("Troca múltipla (3 primeiras da fila ↔ 3 da pilha) realizada.\n");
            } else {
                printf("Troca múltipla não realizada (falta de peças).\n");
            }
        }
        else if (opcao == 0) {
            printf("Encerrando programa. Até mais!\n");
        }
        else {
            printf("Opção inválida. Tente novamente.\n");
        }

        /* Pequena pausa visual (opcional): usuário vê a mensagem antes do próximo estado.
           Não usamos system("pause") para manter portabilidade; apenas solicita enter. */
        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            int c = getchar(); /* consome possível '\n' remanescente do scanf */
            while (c != '\n' && c != EOF) c = getchar();
            /* agora aguarda um Enter real */
            getchar();
        }

    } while (opcao != 0);

    return 0;
}
