#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//======================================================================
// 1. DEFINIÇÕES E ESTRUTURAS DE DADOS
//======================================================================

// Constantes para definir o tamanho máximo das estruturas
#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura para representar uma peça do jogo
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L')
    int id;    // Identificador único da peça
} Peca;

// Estrutura para a Fila Circular de peças
typedef struct {
    Peca itens[TAM_FILA];
    int inicio;
    int fim;
    int quantidade;
} FilaCircular;

// Estrutura para a Pilha de peças reservadas
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;


//======================================================================
// 2. VARIÁVEIS GLOBAIS
//======================================================================

FilaCircular filaDePecas;
Pilha pilhaDeReserva;
int proximoId = 0; // Contador global para gerar IDs únicos


//======================================================================
// 3. PROTÓTIPOS DAS FUNÇÕES
//======================================================================

// Funções de Geração e Inicialização
Peca gerarPeca();
void inicializarEstruturas();

// Funções de Manipulação da Fila
void enqueue(FilaCircular *f, Peca p);
Peca dequeue(FilaCircular *f);
int filaCheia(FilaCircular *f);
int filaVazia(FilaCircular *f);

// Funções de Manipulação da Pilha
void push(Pilha *p, Peca peca);
Peca pop(Pilha *p);
int pilhaCheia(Pilha *p);
int pilhaVazia(Pilha *p);

// Funções de Ação do Jogo (Menu)
void jogarPecaDaFila();
void reservarPecaParaPilha();
void usarPecaDaReserva();
void trocarPecaFilaPilha();
void trocarBlocoPecas();

// Funções de Interface com o Usuário
void exibirEstadoAtual();
void exibirMenu();


//======================================================================
// 4. FUNÇÃO PRINCIPAL (main)
//======================================================================

int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));
    
    // Prepara a fila e a pilha para o início do jogo
    inicializarEstruturas();

    int opcao;
    do {
        // Mostra o estado atual das peças antes de cada jogada
        exibirEstadoAtual();
        exibirMenu();

        // Lê a escolha do usuário
        printf("\nOpcao escolhida: ");
        scanf("%d", &opcao);
        printf("\n");

        // Executa a ação com base na escolha
        switch (opcao) {
            case 1:
                jogarPecaDaFila();
                break;
            case 2:
                reservarPecaParaPilha();
                break;
            case 3:
                usarPecaDaReserva();
                break;
            case 4:
                trocarPecaFilaPilha();
                break;
            case 5:
                trocarBlocoPecas();
                break;
            case 0:
                printf("Encerrando o programa. Ate mais!\n");
                break;
            default:
                printf("--- ATENCAO: Opcao invalida! Tente novamente. ---\n");
                break;
        }
        printf("\n--------------------------------------------------------------\n\n");

    } while (opcao != 0);

    return 0;
}


//======================================================================
// 5. IMPLEMENTAÇÃO DAS FUNÇÕES
//======================================================================

// --- Funções de Geração e Inicialização ---

/**
 * Gera uma nova peça com um tipo aleatório e um ID sequencial.
 */
Peca gerarPeca() {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L'};
    novaPeca.nome = tipos[rand() % 4]; // Escolhe um tipo aleatoriamente
    novaPeca.id = proximoId++;         // Atribui o ID atual e incrementa para o próximo
    return novaPeca;
}

/**
 * Inicializa a fila com 5 peças e a pilha vazia.
 */
void inicializarEstruturas() {
    // Inicializa a Fila
    filaDePecas.inicio = 0;
    filaDePecas.fim = -1;
    filaDePecas.quantidade = 0;
    // Preenche a fila com as peças iniciais
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&filaDePecas, gerarPeca());
    }

    // Inicializa a Pilha (vazia)
    pilhaDeReserva.topo = -1;
}

// --- Funções da Fila Circular ---

int filaCheia(FilaCircular *f) {
    return f->quantidade == TAM_FILA;
}

int filaVazia(FilaCircular *f) {
    return f->quantidade == 0;
}

/**
 * Adiciona um elemento no final da fila circular.
 */
void enqueue(FilaCircular *f, Peca p) {
    if (!filaCheia(f)) {
        f->fim = (f->fim + 1) % TAM_FILA; // Avança o 'fim' de forma circular
        f->itens[f->fim] = p;
        f->quantidade++;
    }
}

/**
 * Remove e retorna o elemento do início da fila circular.
 */
Peca dequeue(FilaCircular *f) {
    Peca pecaRemovida;
    if (!filaVazia(f)) {
        pecaRemovida = f->itens[f->inicio];
        f->inicio = (f->inicio + 1) % TAM_FILA; // Avança o 'início' de forma circular
        f->quantidade--;
        return pecaRemovida;
    }
    // Retorno padrão caso a fila esteja vazia (não deve acontecer neste programa)
    pecaRemovida.id = -1;
    pecaRemovida.nome = ' ';
    return pecaRemovida;
}


// --- Funções da Pilha ---

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

/**
 * Adiciona um elemento no topo da pilha.
 */
void push(Pilha *p, Peca peca) {
    if (!pilhaCheia(p)) {
        p->topo++;
        p->itens[p->topo] = peca;
    }
}

/**
 * Remove e retorna o elemento do topo da pilha.
 */
Peca pop(Pilha *p) {
    Peca pecaRemovida;
    if (!pilhaVazia(p)) {
        pecaRemovida = p->itens[p->topo];
        p->topo--;
        return pecaRemovida;
    }
    // Retorno padrão caso a pilha esteja vazia
    pecaRemovida.id = -1;
    pecaRemovida.nome = ' ';
    return pecaRemovida;
}


// --- Funções de Ação do Jogo ---

/**
 * Ação 1: Remove a peça da frente da fila e gera uma nova para manter a fila cheia.
 */
void jogarPecaDaFila() {
    Peca pecaJogada = dequeue(&filaDePecas);
    printf("Acao: Peca [%c %d] foi jogada e removida do jogo.\n", pecaJogada.nome, pecaJogada.id);
    // Adiciona uma nova peça para manter a fila com 5 elementos
    enqueue(&filaDePecas, gerarPeca());
}

/**
 * Ação 2: Move a peça da frente da fila para a pilha de reserva.
 */
void reservarPecaParaPilha() {
    if (pilhaCheia(&pilhaDeReserva)) {
        printf("Acao: Nao foi possivel reservar. A pilha esta cheia!\n");
    } else {
        Peca pecaMovida = dequeue(&filaDePecas);
        push(&pilhaDeReserva, pecaMovida);
        printf("Acao: Peca [%c %d] movida da fila para a pilha de reserva.\n", pecaMovida.nome, pecaMovida.id);
        // Adiciona uma nova peça para manter a fila com 5 elementos
        enqueue(&filaDePecas, gerarPeca());
    }
}

/**
 * Ação 3: Remove (usa) a peça do topo da pilha de reserva.
 */
void usarPecaDaReserva() {
    if (pilhaVazia(&pilhaDeReserva)) {
        printf("Acao: Nao foi possivel usar. A pilha de reserva esta vazia!\n");
    } else {
        Peca pecaUsada = pop(&pilhaDeReserva);
        printf("Acao: Peca reservada [%c %d] foi usada e removida do jogo.\n", pecaUsada.nome, pecaUsada.id);
    }
}

/**
 * Ação 4: Troca a peça da frente da fila com a do topo da pilha.
 */
void trocarPecaFilaPilha() {
    if (filaVazia(&filaDePecas) || pilhaVazia(&pilhaDeReserva)) {
        printf("Acao: Nao foi possivel trocar. Fila e/ou pilha estao vazias!\n");
    } else {
        // Acessa os elementos diretamente para a troca, sem removê-los
        Peca temp = filaDePecas.itens[filaDePecas.inicio];
        filaDePecas.itens[filaDePecas.inicio] = pilhaDeReserva.itens[pilhaDeReserva.topo];
        pilhaDeReserva.itens[pilhaDeReserva.topo] = temp;
        printf("Acao: Troca realizada entre a frente da fila e o topo da pilha.\n");
    }
}

/**
 * Ação 5: Troca as 3 primeiras peças da fila com as 3 peças da pilha.
 */
void trocarBlocoPecas() {
    // Verifica se a pilha está cheia (condição para a troca)
    if (!pilhaCheia(&pilhaDeReserva)) {
        printf("Acao: Troca em bloco falhou. A pilha de reserva nao esta cheia (precisa ter 3 pecas).\n");
    } else {
        // A fila sempre terá 5 peças, então a verificação de >=3 não é estritamente necessária,
        // mas é uma boa prática.
        Peca temp;
        // Itera 3 vezes para trocar os elementos
        for (int i = 0; i < 3; i++) {
            // Calcula o índice do elemento na fila circular
            int indiceFila = (filaDePecas.inicio + i) % TAM_FILA;
            
            // Realiza a troca (swap)
            temp = filaDePecas.itens[indiceFila];
            filaDePecas.itens[indiceFila] = pilhaDeReserva.itens[pilhaDeReserva.topo - i];
            pilhaDeReserva.itens[pilhaDeReserva.topo - i] = temp;
        }
        printf("Acao: Troca realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
    }
}


// --- Funções de Interface com o Usuário ---

/**
 * Exibe o estado atual da Fila e da Pilha no console.
 */
void exibirEstadoAtual() {
    printf("Estado atual:\n\n");

    // Exibe a Fila
    printf("Fila de pecas: ");
    if (filaVazia(&filaDePecas)) {
        printf("(vazia)");
    } else {
        for (int i = 0; i < filaDePecas.quantidade; i++) {
            int index = (filaDePecas.inicio + i) % TAM_FILA;
            printf("[%c %d] ", filaDePecas.itens[index].nome, filaDePecas.itens[index].id);
        }
    }
    printf("\n");

    // Exibe a Pilha
    printf("Pilha de reserva (Topo -> base): ");
    if (pilhaVazia(&pilhaDeReserva)) {
        printf("(vazia)");
    } else {
        for (int i = pilhaDeReserva.topo; i >= 0; i--) {
            printf("[%c %d] ", pilhaDeReserva.itens[i].nome, pilhaDeReserva.itens[i].id);
        }
    }
    printf("\n");
}

/**
 * Exibe o menu de opções para o usuário.
 */
void exibirMenu() {
    printf("\nOpcoes disponiveis:\n");
    printf("1. Jogar peca da frente da fila\n");
    printf("2. Enviar peca da fila para a pilha de reserva\n");
    printf("3. Usar peca da pilha de reserva\n");
    printf("4. Trocar peca da frente da fila com o topo da pilha\n");
    printf("5. Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
    printf("0. Sair\n");
}