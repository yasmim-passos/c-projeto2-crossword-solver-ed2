#include "grid.h"
#include <stddef.h>
#include <string.h>

void inicializarGrid(Grid* grid, int linhas, int colunas) {
    if (!grid) return;
    grid->linhas = linhas;
    grid->colunas = colunas;
    grid->numPalavras = 0;
    for(int i=0; i<TAMANHO_MAX_GRID; i++) {
        for(int j=0; j<TAMANHO_MAX_GRID; j++) {
            grid->celulas[i][j].tipo = CELULA_BLOQUEADA;
            grid->celulas[i][j].letra = '\0';
            grid->celulas[i][j].eFixa = false; // Jogador pode editar
            grid->celulas[i][j].numero = 0;
        }
    }
}

bool ehPosicaoValida(Grid* grid, int linha, int coluna) {
    if (!grid) return false;
    return (linha >= 0 && linha < grid->linhas && coluna >= 0 && coluna < grid->colunas);
}

Celula* obterCelula(Grid* grid, int linha, int coluna) {
    if (!ehPosicaoValida(grid, linha, coluna)) return NULL;
    return &grid->celulas[linha][coluna];
}

void limparGrid(Grid* grid) {
    if (!grid) return;
    for (int i = 0; i < grid->linhas; i++) {
        for (int j = 0; j < grid->colunas; j++) {
            if (!grid->celulas[i][j].eFixa) {
                grid->celulas[i][j].letra = '\0';
                if (grid->celulas[i][j].tipo == CELULA_PREENCHIDA) {
                    grid->celulas[i][j].tipo = CELULA_VAZIA;
                }
            }
        }
    }
}

Palavra* obterPalavra(Grid* grid, int indice) {
    if (!grid || indice < 0 || indice >= grid->numPalavras) return NULL;
    return &grid->palavras[indice];
}

bool gridEstaCompleto(Grid* grid) {
    if (!grid) return false;
    for (int i = 0; i < grid->numPalavras; i++) {
        if (!grid->palavras[i].estaCompleta) return false;
    }
    return true;
}

bool colocarPalavra(Grid* grid, Palavra* palavra, const char* texto) {
    if (!grid || !palavra || !texto) return false;
    
    int len = strlen(texto);
    if (len != palavra->tamanho) return false;
    
    strncpy(palavra->resposta, texto, TAMANHO_MAX_PALAVRA);
    
    palavra->textoAtual[0] = '\0';
    palavra->estaCompleta = false;
    
    int r = palavra->inicio.linha;
    int c = palavra->inicio.coluna;
    
    // Muda para espaço cheio, não pode escrever
    for (int i = 0; i < len; i++) {
        if (ehPosicaoValida(grid, r, c)) {
            grid->celulas[r][c].tipo = CELULA_VAZIA; 
        }
        
        if (palavra->direcao == DIRECAO_HORIZONTAL) c++;
        else r++;
    }
    
    return true;
}

void removerPalavra(Grid* grid, Palavra* palavra) {
    if (!grid || !palavra) return;
    
    int r = palavra->inicio.linha;
    int c = palavra->inicio.coluna;
    int len = palavra->tamanho;
    
    palavra->estaCompleta = false;
    palavra->textoAtual[0] = '\0';
    
    for (int i = 0; i < len; i++) {
        if (ehPosicaoValida(grid, r, c)) {
            if (grid->celulas[r][c].eFixa) {
            } else {
                bool partOfAnother = false;
                for (int w = 0; w < grid->numPalavras; w++) {
                    Palavra* other = &grid->palavras[w];
                    if (other == palavra) continue;
                    if (!other->estaCompleta) continue;
                    
                    // Checa interceção
                    int or = other->inicio.linha;
                    int oc = other->inicio.coluna;
                    
                    if (other->direcao == DIRECAO_HORIZONTAL) {
                        if (r == or && c >= oc && c < oc + other->tamanho) {
                            partOfAnother = true;
                            break;
                        }
                    } else {
                        if (c == oc && r >= or && r < or + other->tamanho) {
                            partOfAnother = true;
                            break;
                        }
                    }
                }
                
                if (!partOfAnother) {
                    grid->celulas[r][c].letra = '\0';
                    grid->celulas[r][c].tipo = CELULA_VAZIA;
                }
            }
        }
        
        if (palavra->direcao == DIRECAO_HORIZONTAL) c++;
        else r++;
    }
}
