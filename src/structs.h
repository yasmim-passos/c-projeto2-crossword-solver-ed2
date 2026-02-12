#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>

#define TAMANHO_MAX_GRID 15        // Tamanho máximo do grid (15x15)
#define MAX_PALAVRAS 50            // Número máximo de palavras no puzzle
#define TAMANHO_MAX_PALAVRA 15     // Tamanho máximo de uma palavra
#define TAMANHO_MAX_DICA 200       // Tamanho máximo da dica

typedef enum {
    CELULA_VAZIA = 0,        // Célula vazia (pode ser preenchida)
    CELULA_BLOQUEADA = 1,    // Célula bloqueada (preta)
    CELULA_PREENCHIDA = 2    // Célula com letra
} TipoCelula;

typedef enum { //Direção da palavra no grid
    DIRECAO_HORIZONTAL = 0,
    DIRECAO_VERTICAL = 1
} Direcao;

typedef enum {
    MODO_SOLVER = 0,
    MODO_USUARIO = 1
} ModoJogo;

    TipoCelula tipo;    // Tipo da célula (vazia, bloqueada, preenchida)
    char letra;         // Letra atual ('\0' se vazia)
    bool eFixa;         // true = letra fixa (não pode ser alterada)
    int numero;         // Número da célula, 0 se não numerada
} Celula;

typedef struct {
    int linha;    // Linha: 0 a TAMANHO_MAX_GRID-1 (de cima para baixo)
    int coluna;   // Coluna 0 a TAMANHO_MAX_GRID-1 (da esquerda para direita)
} Posicao;

/**
 * Palavra no puzzle
 * 
 * Representa uma palavra que deve ser colocada no grid.
 * 
 * CAMPOS:
 * - inicio: Posição inicial da palavra
 * - direcao: Direção (horizontal ou vertical)
 * - tamanho: Tamanho da palavra
 * - dica: Dica/significado da palavra
 * - resposta: Resposta correta (para validação)
 * - textoAtual: Texto atual preenchido (pode estar incompleto)
 * - estaCompleta: Se a palavra está completamente preenchida
 * 
 * USO:
 * - Pessoa 1: Iterar palavras, tentar colocar no backtracking
 * - Pessoa 3: Mostrar dicas, destacar palavra selecionada
 * - Pessoa 4: Salvar/carregar palavras do puzzle
 */
typedef struct {
    Posicao inicio;                           // Posição inicial
    Direcao direcao;                          // Direção da palavra
    int tamanho;                              // Tamanho da palavra
    char dica[TAMANHO_MAX_DICA];             // Dica/significado
    char resposta[TAMANHO_MAX_PALAVRA + 1];  // Resposta correta
    char textoAtual[TAMANHO_MAX_PALAVRA + 1]; // Texto atual preenchido
    bool estaCompleta;                        // Se está completamente preenchida
} Palavra;

/**
 * Grid (tabuleiro) de palavras cruzadas
 * 
 * Estrutura principal que representa o tabuleiro completo.
 * 
 * CAMPOS:
 * - celulas: Matriz de células (TAMANHO_MAX_GRID x TAMANHO_MAX_GRID)
 * - linhas: Número de linhas usadas (≤ TAMANHO_MAX_GRID)
 * - colunas: Número de colunas usadas (≤ TAMANHO_MAX_GRID)
 * - palavras: Array de palavras no puzzle
 * - numPalavras: Número de palavras no puzzle
 * 
 * USO:
 * - Pessoa 1: Estrutura principal para backtracking
 * - Pessoa 3: Fonte de dados para desenhar o tabuleiro
 * - Pessoa 4: Estrutura para salvar/carregar
 * 
 * IMPORTANTE: 
 * - Sempre use as funções de grid.h para manipular o grid
 * - Não acesse celulas[][] diretamente, use obterCelula/definirCelula
 */
typedef struct {
    Celula celulas[TAMANHO_MAX_GRID][TAMANHO_MAX_GRID];  // Matriz de células
    int linhas;                                           // Linhas usadas
    int colunas;                                          // Colunas usadas
    Palavra palavras[MAX_PALAVRAS];                       // Array de palavras
    int numPalavras;                                      // Número de palavras
} Grid;

/**
 * Estado completo do jogo
 * 
 * Representa o estado completo do jogo, incluindo grid e estatísticas.
 * 
 * CAMPOS:
 * - grid: Grid atual
 * - modo: Modo de jogo (solver ou usuário)
 * - tentativas: Número de tentativas (para estatísticas)
 * - retrocessos: Número de backtracks (para visualização)
 * - estaCompleto: Se o puzzle está completo
 * - estaResolvido: Se o puzzle está resolvido corretamente
 * 
 * USO:
 * - Pessoa 1: Rastrear tentativas e retrocessos
 * - Pessoa 3: Mostrar estatísticas na UI
 * - Pessoa 4: Salvar/carregar estado completo do jogo
 */
typedef struct {
    Grid grid;              // Grid atual
    ModoJogo modo;          // Modo de jogo
    int tentativas;         // Número de tentativas
    int retrocessos;        // Número de backtracks
    bool estaCompleto;      // Se o puzzle está completo
    bool estaResolvido;     // Se está resolvido corretamente
} EstadoJogo;

#endif // STRUCTS_H
