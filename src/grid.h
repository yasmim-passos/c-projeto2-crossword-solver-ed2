#ifndef GRID_H
#define GRID_H

#include "structs.h"
#include <stdbool.h>

//Cria e inicializa um novo grid
Grid* criarGrid(int linhas, int colunas);

//Libera memória alocada para o grid
void liberarGrid(Grid* grid);

//Limpa todas as células do grid
//Remove todas as letras não fixas do grid, mantendo células bloqueadas
//e letras fixas intactas.
void limparGrid(Grid* grid);

//Obtém ponteiro para célula em posição específica
Celula* obterCelula(Grid* grid, int linha, int coluna);

//Define célula em posição específica
void definirCelula(Grid* grid, int linha, int coluna, Celula celula);

//Obtém letra de uma célula
char obterLetraCelula(Grid* grid, int linha, int coluna);

//Define letra em uma célula
//Só funciona se a célula não for bloqueada e não for fixa
bool definirLetraCelula(Grid* grid, int linha, int coluna, char letra);

/*
Coloca uma palavra no grid
 
Tenta colocar a palavra especificada no grid, começando na posição
definida em palavra->inicio e seguindo palavra->direcao.
 
Verifica se:
 - A palavra cabe no grid
 - Não sobrescreve células bloqueadas
 - Não conflita com letras fixas
 - Não conflita com outras palavras (letras de cruzamento devem coincidir)
 */
bool colocarPalavra(Grid* grid, Palavra* palavra, const char* texto);

/*
Remove uma palavra do grid
 
Remove todas as letras da palavra especificada, exceto:
 - Letras fixas
 - Letras que fazem parte de outras palavras já colocadas
 */
bool removerPalavra(Grid* grid, Palavra* palavra);

/*
Verifica se pode colocar uma palavra no grid

Verifica se a palavra pode ser colocada sem modificar o grid
Útil para validação antes de tentar colocar
 */
bool podeColocarPalavra(Grid* grid, Palavra* palavra, const char* texto);

//Adiciona uma palavra à lista de palavras do grid
bool adicionarPalavra(Grid* grid, Palavra palavra);

//Obtém ponteiro para palavra em índice específico
Palavra* obterPalavra(Grid* grid, int indice);

/*
Cria uma cópia profunda do grid
 
Útil para salvar estado antes de tentar colocar palavras
*/
Grid* copiarGrid(Grid* origem);

//Imprime grid no console 
void imprimirGrid(Grid* grid);

//Verifica se posição é válida no grid
bool ehPosicaoValida(Grid* grid, int linha, int coluna);

//Conta número de células vazias no grid
int contarCelulasVazias(Grid* grid);

/*
Verifica se o grid está completo

Um grid está completo quando todas as células não-bloqueadas têm letras
 */
bool gridEstaCompleto(Grid* grid);

#endif 
