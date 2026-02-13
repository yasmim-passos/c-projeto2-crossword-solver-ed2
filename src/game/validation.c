#include "validation.h"
#include "../grid.h"
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// ========== FUNÇÃO PRINCIPAL DE VALIDAÇÃO ==========

bool validarPalavra(Grid *grid, Palavra *palavra, const char *texto) {
  // Validação de parâmetros
  if (grid == NULL || palavra == NULL || texto == NULL) {
    return false;
  }

  // Executar todas as validações em sequência
  // Se qualquer uma falhar, retorna false

  if (!validarTamanho(palavra, texto)) {
    return false;
  }

  if (!validarLimites(grid, palavra)) {
    return false;
  }

  if (!validarConflitos(grid, palavra)) {
    return false;
  }

  if (!validarIntersecoes(grid, palavra, texto)) {
    return false;
  }

  // Todas as validações passaram
  return true;
}

// ========== VALIDAÇÕES ESPECÍFICAS ==========

bool validarTamanho(Palavra *palavra, const char *texto) {
  if (palavra == NULL || texto == NULL) {
    return false;
  }

  // Verifica se o comprimento do texto corresponde ao tamanho esperado
  int tamanhoTexto = strlen(texto);

  if (tamanhoTexto != palavra->tamanho) {
    return false;
  }

  return true;
}

bool validarLimites(Grid *grid, Palavra *palavra) {
  if (grid == NULL || palavra == NULL) {
    return false;
  }

  // Calcular posição final da palavra baseado na direção
  int linhaFinal = palavra->inicio.linha;
  int colunaFinal = palavra->inicio.coluna;

  if (palavra->direcao == DIRECAO_HORIZONTAL) {
    // Para horizontal, avança nas colunas
    colunaFinal += palavra->tamanho - 1;
  } else {
    // Para vertical, avança nas linhas
    linhaFinal += palavra->tamanho - 1;
  }

  // Verificar se a posição inicial é válida
  if (!ehPosicaoValida(grid, palavra->inicio.linha, palavra->inicio.coluna)) {
    return false;
  }

  // Verificar se a posição final está dentro dos limites
  if (!ehPosicaoValida(grid, linhaFinal, colunaFinal)) {
    return false;
  }

  return true;
}

bool validarConflitos(Grid *grid, Palavra *palavra) {
  if (grid == NULL || palavra == NULL) {
    return false;
  }

  // Percorrer cada posição que a palavra ocuparia
  int linha = palavra->inicio.linha;
  int coluna = palavra->inicio.coluna;

  for (int i = 0; i < palavra->tamanho; i++) {
    // Obter célula na posição atual
    Celula *celula = obterCelula(grid, linha, coluna);

    if (celula == NULL) {
      return false;
    }

    // Verificar se a célula está bloqueada
    if (celula->tipo == CELULA_BLOQUEADA) {
      return false; // Não pode colocar palavra em célula bloqueada
    }

    // Avançar para próxima posição
    if (palavra->direcao == DIRECAO_HORIZONTAL) {
      coluna++;
    } else {
      linha++;
    }
  }

  return true;
}

bool validarIntersecoes(Grid *grid, Palavra *palavra, const char *texto) {
  if (grid == NULL || palavra == NULL || texto == NULL) {
    return false;
  }

  // Percorrer cada letra da palavra
  int linha = palavra->inicio.linha;
  int coluna = palavra->inicio.coluna;

  for (int i = 0; i < palavra->tamanho; i++) {
    // Obter célula na posição atual
    Celula *celula = obterCelula(grid, linha, coluna);

    if (celula == NULL) {
      return false;
    }

    // Se a célula já tem uma letra (interseção com outra palavra)
    if (celula->letra != '\0') {
      // Converter ambas as letras para maiúsculas para comparação
      char letraCelula = toupper(celula->letra);
      char letraPalavra = toupper(texto[i]);

      // Verificar se as letras são compatíveis
      if (letraCelula != letraPalavra) {
        // Conflito! A letra da palavra não bate com a letra já no grid
        return false;
      }

      // Se a célula é fixa, também precisa coincidir
      // (já verificado acima, mas deixando explícito)
      if (celula->eFixa && letraCelula != letraPalavra) {
        return false;
      }
    }

    // Avançar para próxima posição
    if (palavra->direcao == DIRECAO_HORIZONTAL) {
      coluna++;
    } else {
      linha++;
    }
  }

  // Todas as interseções são válidas
  return true;
}

// ========== FUNÇÕES UTILITÁRIAS ==========

int contarLetrasCorretas(Grid *grid, Palavra *palavra, const char *texto) {
  if (grid == NULL || palavra == NULL || texto == NULL) {
    return 0;
  }

  int contador = 0;
  int linha = palavra->inicio.linha;
  int coluna = palavra->inicio.coluna;

  // Percorrer cada letra da palavra
  for (int i = 0; i < palavra->tamanho; i++) {
    Celula *celula = obterCelula(grid, linha, coluna);

    if (celula != NULL && celula->letra != '\0') {
      // Comparar letras (case-insensitive)
      if (toupper(celula->letra) == toupper(texto[i])) {
        contador++;
      }
    }

    // Avançar para próxima posição
    if (palavra->direcao == DIRECAO_HORIZONTAL) {
      coluna++;
    } else {
      linha++;
    }
  }

  return contador;
}
