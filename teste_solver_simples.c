#include "src/game/game_mode.h"
#include "src/game/solver.h"
#include "src/game/validation.h"
#include "src\data\grid.h"
#include "src\data\structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * ========================================
 * TESTE SIMPLIFICADO DO SOLVER
 * ========================================
 *
 * Este teste não depende de dictionary.c ou api_client.c
 * Usa palavras mockadas diretamente no código
 */

// Mock da função dict_search_by_size
void dict_search_by_size(int size, char ***out_words, int *out_count) {
  // Retornar palavras mock baseado no tamanho
  if (size == 5) {
    *out_count = 3;
    *out_words = malloc(sizeof(char *) * 3);
    (*out_words)[0] = strdup("HELLO");
    (*out_words)[1] = strdup("WORLD");
    (*out_words)[2] = strdup("TESTE");
  } else if (size == 4) {
    *out_count = 2;
    *out_words = malloc(sizeof(char *) * 2);
    (*out_words)[0] = strdup("CASA");
    (*out_words)[1] = strdup("VIDA");
  } else if (size == 3) {
    *out_count = 2;
    *out_words = malloc(sizeof(char *) * 2);
    (*out_words)[0] = strdup("SOL");
    (*out_words)[1] = strdup("MAR");
  } else {
    *out_count = 0;
    *out_words = NULL;
  }
}

void imprimirSeparador(void) {
  printf("\n");
  printf("========================================\n");
}

void testarCriacaoGrid(void) {
  imprimirSeparador();
  printf("TESTE 1: Criacao de Grid\n");
  imprimirSeparador();

  Grid *grid = criarGrid(5, 5);

  if (grid != NULL) {
    printf("✓ Grid 5x5 criado com sucesso!\n");
    printf("  Linhas: %d\n", grid->linhas);
    printf("  Colunas: %d\n", grid->colunas);
    printf("  Palavras: %d\n", grid->numPalavras);

    liberarGrid(grid);
  } else {
    printf("✗ Falha ao criar grid\n");
  }
}

void testarValidacao(void) {
  imprimirSeparador();
  printf("TESTE 2: Validacao de Palavras\n");
  imprimirSeparador();

  Grid *grid = criarGrid(5, 5);

  // Criar uma palavra horizontal
  Palavra palavra1;
  palavra1.inicio.linha = 0;
  palavra1.inicio.coluna = 0;
  palavra1.direcao = DIRECAO_HORIZONTAL;
  palavra1.tamanho = 5;
  strcpy(palavra1.dica, "Teste horizontal");
  palavra1.estaCompleta = false;

  // Testar validação de tamanho
  printf("\nTestando validacao de tamanho:\n");
  if (validarTamanho(&palavra1, "TESTE")) {
    printf("✓ Tamanho correto (5 letras)\n");
  } else {
    printf("✗ Erro na validacao de tamanho\n");
  }

  if (!validarTamanho(&palavra1, "OLA")) {
    printf("✓ Rejeitou tamanho incorreto (3 letras)\n");
  } else {
    printf("✗ Deveria rejeitar tamanho incorreto\n");
  }

  // Testar validação de limites
  printf("\nTestando validacao de limites:\n");
  if (validarLimites(grid, &palavra1)) {
    printf("✓ Palavra cabe no grid\n");
  } else {
    printf("✗ Erro na validacao de limites\n");
  }

  // Testar validação completa
  printf("\nTestando validacao completa:\n");
  if (validarPalavra(grid, &palavra1, "TESTE")) {
    printf("✓ Palavra valida aceita\n");
  } else {
    printf("✗ Erro na validacao completa\n");
  }

  liberarGrid(grid);
}

void testarModoJogo(void) {
  imprimirSeparador();
  printf("TESTE 3: Controle de Modo de Jogo\n");
  imprimirSeparador();

  // Estado inicial
  printf("\nEstado inicial:\n");
  printf("  Modo: %s\n", obterNomeModo());
  printf("  Estado Solver: %s\n", obterNomeEstadoSolver());

  // Mudar para modo solver
  printf("\nMudando para modo solver:\n");
  definirModo(MODO_SOLVER);
  printf("  Modo: %s\n", obterNomeModo());
  printf("  E modo solver? %s\n", estaModoSolver() ? "Sim" : "Nao");

  // Definir estado do solver
  printf("\nIniciando solver:\n");
  definirEstadoSolver(SOLVER_EXECUTANDO);
  printf("  Estado Solver: %s\n", obterNomeEstadoSolver());
  printf("  Solver executando? %s\n", solverEstaExecutando() ? "Sim" : "Nao");

  // Resetar
  printf("\nResetando:\n");
  resetarModoJogo();
  printf("  Modo: %s\n", obterNomeModo());
  printf("  Estado Solver: %s\n", obterNomeEstadoSolver());
}

void testarSolverSimples(void) {
  imprimirSeparador();
  printf("TESTE 4: Solver com Grid Simples\n");
  imprimirSeparador();

  // Criar grid pequeno
  Grid *grid = criarGrid(5, 5);

  // Adicionar uma palavra horizontal simples
  Palavra palavra1;
  palavra1.inicio.linha = 0;
  palavra1.inicio.coluna = 0;
  palavra1.direcao = DIRECAO_HORIZONTAL;
  palavra1.tamanho = 5;
  strcpy(palavra1.dica, "Primeira palavra");
  strcpy(palavra1.resposta, "HELLO");
  palavra1.estaCompleta = false;
  palavra1.textoAtual[0] = '\0';

  adicionarPalavra(grid, palavra1);

  printf("\nGrid inicial:\n");
  imprimirGrid(grid);

  // Tentar resolver
  printf("\nIniciando solver...\n");
  EstadoSolver estado;

  if (resolverPuzzle(grid, &estado)) {
    printf("\n✓ PUZZLE RESOLVIDO!\n");
    printf("\nEstatisticas:\n");
    printf("  Tentativas: %d\n", estado.tentativas);
    printf("  Retrocessos: %d\n", estado.retrocessos);
    printf("  Profundidade: %d\n", estado.profundidade);

    printf("\nGrid resolvido:\n");
    imprimirGrid(grid);
  } else {
    printf("\n✗ Nao foi possivel resolver o puzzle\n");
    printf("\nEstatisticas:\n");
    printf("  Tentativas: %d\n", estado.tentativas);
    printf("  Retrocessos: %d\n", estado.retrocessos);
  }

  liberarGrid(grid);
}

int main(void) {
  printf("\n");
  printf("╔════════════════════════════════════════╗\n");
  printf("║  TESTE DO SOLVER DE PALAVRAS CRUZADAS  ║\n");
  printf("╚════════════════════════════════════════╝\n");

  // Executar testes
  testarCriacaoGrid();
  testarValidacao();
  testarModoJogo();
  testarSolverSimples();

  // Resumo final
  imprimirSeparador();
  printf("TESTES CONCLUIDOS\n");
  imprimirSeparador();
  printf("\n");
  printf("Os modulos estao prontos para integracao!\n");
  printf("\n");

  return 0;
}