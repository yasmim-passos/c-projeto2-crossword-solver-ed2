#ifndef GRID_H
#define GRID_H

#include "structs.h"

// Verifica se uma posição (linha, coluna) está dentro dos limites do grid
bool ehPosicaoValida(Grid* grid, int linha, int coluna);

// Retorna um ponteiro para a célula na posição especificada, ou NULL se inválido
Celula* obterCelula(Grid* grid, int linha, int coluna);

// Inicializa um grid
void inicializarGrid(Grid* grid, int linhas, int colunas);

// Limpa o grid (reseta células não fixas)
void limparGrid(Grid* grid);

// Obtém ponteiro para palavra pelo índice
Palavra* obterPalavra(Grid* grid, int indice);

// Verifica se todas as palavras estão completas
bool gridEstaCompleto(Grid* grid);

// Coloca uma palavra no grid preenchendo as células
bool colocarPalavra(Grid* grid, Palavra* palavra, const char* texto);

// Remove uma palavra do grid (respeitando interseções)
void removerPalavra(Grid* grid, Palavra* palavra);

#endif // GRID_H
