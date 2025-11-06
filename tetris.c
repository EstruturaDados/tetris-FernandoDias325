#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Tamanhos das estruturas
#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura da peca
typedef struct {
    char nome;  // tipo: I, O, T, L
    int id;     // identificador unico
} Peca;

// Fila circular
typedef struct {
    Peca itens[TAM_FILA];
    int inicio;
    int fim;
    int qtd;
} FilaCircular;

// Pilha de reserva
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// Variaveis globais
FilaCircular fila;
Pilha pilha_reserva;
int proximo_id = 0;

// Prototipos
Peca gerarPeca();
void inicializar();
void enqueue(FilaCircular *f, Peca p);
Peca dequeue(FilaCircular *f);
int filaCheia(FilaCircular *f);
int filaVazia(FilaCircular *f);
void push(Pilha *p, Peca peca);
Peca pop(Pilha *p);
int pilhaCheia(Pilha *p);
int pilhaVazia(Pilha *p);
void jogarPeca();
void reservarPeca();
void usarReservada();
void trocarTopoPrimeiroFila();
void trocarBloco();
void mostrarEstado();
void mostrarMenu();


int main() {
    srand(time(NULL));
    inicializar();

    int opcao;
    
    do {
        mostrarEstado();
        mostrarMenu();

        printf("\nEscolha: ");
        scanf("%d", &opcao);
        printf("\n");

        switch (opcao) {
            case 1:
                jogarPeca();
                break;
            case 2:
                reservarPeca();
                break;
            case 3:
                usarReservada();
                break;
            case 4:
                trocarTopoPrimeiroFila();
                break;
            case 5:
                trocarBloco();
                break;
            case 0:
                printf("Saindo do jogo. Ate logo!\n");
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
        
        if(opcao != 0) {
            printf("\n");
        }

    } while (opcao != 0);

    return 0;
}


// Gera uma peca aleatoria com ID sequencial
Peca gerarPeca() {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id = proximo_id++;
    return nova;
}

// Inicializa a fila com 5 pecas e pilha vazia
void inicializar() {
    // fila
    fila.inicio = 0;
    fila.fim = -1;
    fila.qtd = 0;
    
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    // pilha começa vazia
    pilha_reserva.topo = -1;
}


// Funcoes da fila
int filaCheia(FilaCircular *f) {
    return f->qtd == TAM_FILA;
}

int filaVazia(FilaCircular *f) {
    return f->qtd == 0;
}

void enqueue(FilaCircular *f, Peca p) {
    if (!filaCheia(f)) {
        f->fim = (f->fim + 1) % TAM_FILA;
        f->itens[f->fim] = p;
        f->qtd++;
    }
}

Peca dequeue(FilaCircular *f) {
    Peca removida;
    if (!filaVazia(f)) {
        removida = f->itens[f->inicio];
        f->inicio = (f->inicio + 1) % TAM_FILA;
        f->qtd--;
        return removida;
    }
    // retorna peca invalida se vazia
    removida.id = -1;
    removida.nome = ' ';
    return removida;
}


// Funcoes da pilha
int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

void push(Pilha *p, Peca peca) {
    if (!pilhaCheia(p)) {
        p->topo++;
        p->itens[p->topo] = peca;
    }
}

Peca pop(Pilha *p) {
    Peca removida;
    if (!pilhaVazia(p)) {
        removida = p->itens[p->topo];
        p->topo--;
        return removida;
    }
    removida.id = -1;
    removida.nome = ' ';
    return removida;
}


// Acao 1: joga a peca da frente
void jogarPeca() {
    Peca jogada = dequeue(&fila);
    printf(">>> Peca [%c-%d] foi jogada!\n", jogada.nome, jogada.id);
    // adiciona nova peca pra manter a fila com 5
    enqueue(&fila, gerarPeca());
}

// Acao 2: reserva a peca da frente
void reservarPeca() {
    if (pilhaCheia(&pilha_reserva)) {
        printf("ERRO: Pilha de reserva cheia!\n");
    } else {
        Peca p = dequeue(&fila);
        push(&pilha_reserva, p);
        printf(">>> Peca [%c-%d] reservada na pilha.\n", p.nome, p.id);
        // coloca nova peca na fila
        enqueue(&fila, gerarPeca());
    }
}

// Acao 3: usa peca reservada
void usarReservada() {
    if (pilhaVazia(&pilha_reserva)) {
        printf("ERRO: Pilha de reserva vazia!\n");
    } else {
        Peca usada = pop(&pilha_reserva);
        printf(">>> Peca reservada [%c-%d] foi usada!\n", usada.nome, usada.id);
    }
}

// Acao 4: troca topo da pilha com primeira da fila
void trocarTopoPrimeiroFila() {
    if (filaVazia(&fila) || pilhaVazia(&pilha_reserva)) {
        printf("ERRO: Fila ou pilha vazias!\n");
    } else {
        // faz a troca direta
        Peca temp = fila.itens[fila.inicio];
        fila.itens[fila.inicio] = pilha_reserva.itens[pilha_reserva.topo];
        pilha_reserva.itens[pilha_reserva.topo] = temp;
        printf(">>> Troca realizada: topo da pilha <-> primeira da fila\n");
    }
}

// Acao 5: troca as 3 primeiras da fila com as 3 da pilha
void trocarBloco() {
    if (!pilhaCheia(&pilha_reserva)) {
        printf("ERRO: Pilha precisa estar cheia (3 pecas) para trocar em bloco!\n");
    } else {
        Peca temp;
        
        // troca os 3 elementos
        for (int i = 0; i < 3; i++) {
            int idx_fila = (fila.inicio + i) % TAM_FILA;
            
            temp = fila.itens[idx_fila];
            fila.itens[idx_fila] = pilha_reserva.itens[pilha_reserva.topo - i];
            pilha_reserva.itens[pilha_reserva.topo - i] = temp;
        }
        
        printf(">>> Troca em bloco realizada: 3 primeiras da fila <-> 3 da pilha\n");
    }
}


// Mostra o estado atual
void mostrarEstado() {
    printf("\n========================================\n");
    printf("        ESTADO ATUAL DO JOGO\n");
    printf("========================================\n\n");

    // mostra a fila
    printf("FILA: ");
    if (filaVazia(&fila)) {
        printf("(vazia)");
    } else {
        for (int i = 0; i < fila.qtd; i++) {
            int idx = (fila.inicio + i) % TAM_FILA;
            printf("[%c-%d] ", fila.itens[idx].nome, fila.itens[idx].id);
        }
    }
    printf("\n");

    // mostra a pilha
    printf("PILHA: ");
    if (pilhaVazia(&pilha_reserva)) {
        printf("(vazia)");
    } else {
        for (int i = pilha_reserva.topo; i >= 0; i--) {
            printf("[%c-%d] ", pilha_reserva.itens[i].nome, pilha_reserva.itens[i].id);
        }
    }
    printf("\n");
}

// Menu de opcoes
void mostrarMenu() {
    printf("\n--- MENU ---\n");
    printf("1. Jogar peca (remove da fila)\n");
    printf("2. Reservar peca (fila -> pilha)\n");
    printf("3. Usar peca reservada (remove da pilha)\n");
    printf("4. Trocar topo da pilha com 1a da fila\n");
    printf("5. Trocar bloco (3 da fila <-> 3 da pilha)\n");
    printf("0. Sair\n");
}