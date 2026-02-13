#include "grid.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

Grid* criarGrid(int linhas, int colunas) {
    //Valida parâmetros
    if (linhas <= 0 || linhas > TAMANHO_MAX_GRID || colunas <= 0 || colunas > TAMANHO_MAX_GRID) {
        return NULL;
    }
    
    //Aloca memória para o grid
    Grid* grid = (Grid*)malloc(sizeof(Grid));
    if (grid == NULL) {
        return NULL;
    }
    
    //Inicializa dimensões
    grid->linhas = linhas;
    grid->colunas = colunas;
    grid->numPalavras = 0;
    
    //Inicializa todas as células como vazias
    for (int i = 0; i < TAMANHO_MAX_GRID; i++) {
        for (int j = 0; j < TAMANHO_MAX_GRID; j++) {
            grid->celulas[i][j].tipo = CELULA_VAZIA;
            grid->celulas[i][j].letra = '\0';
            grid->celulas[i][j].eFixa = false;
            grid->celulas[i][j].numero = 0;
        }
    }
    
    return grid;
}

void liberarGrid(Grid* grid) {
    if (grid != NULL) {
        free(grid);
    }
}

void limparGrid(Grid* grid) {
    if (grid == NULL) {
        return;
    }
    
    //Limpa apenas células não fixas
    for (int i = 0; i < grid->linhas; i++) {
        for (int j = 0; j < grid->colunas; j++) {
            Celula* celula = &grid->celulas[i][j];
            
            //Não limpa células bloqueadas ou fixas
            if (celula->tipo != CELULA_BLOQUEADA && !celula->eFixa) {
                celula->letra = '\0';
                celula->tipo = CELULA_VAZIA;
            }
        }
    }
    
    //Limpa texto atual de todas as palavras
    for (int i = 0; i < grid->numPalavras; i++) {
        grid->palavras[i].textoAtual[0] = '\0';
        grid->palavras[i].estaCompleta = false;
    }
}

Celula* obterCelula(Grid* grid, int linha, int coluna) {
    if (!ehPosicaoValida(grid, linha, coluna)) {
        return NULL;
    }
    
    return &grid->celulas[linha][coluna];
}

void definirCelula(Grid* grid, int linha, int coluna, Celula celula) {
    if (!ehPosicaoValida(grid, linha, coluna)) {
        return;
    }
    
    grid->celulas[linha][coluna] = celula;
}

char obterLetraCelula(Grid* grid, int linha, int coluna) {
    Celula* celula = obterCelula(grid, linha, coluna);
    if (celula == NULL) {
        return '\0';
    }
    
    return celula->letra;
}

bool definirLetraCelula(Grid* grid, int linha, int coluna, char letra) {
    Celula* celula = obterCelula(grid, linha, coluna);
    if (celula == NULL) {
        return false;
    }
    
    //Não pode modificar células bloqueadas ou fixas
    if (celula->tipo == CELULA_BLOQUEADA || celula->eFixa) {
        return false;
    }
    
    //Atualizar letra
    celula->letra = letra;
    
    //Atualizar tipo da célula
    if (letra == '\0') {
        celula->tipo = CELULA_VAZIA;
    } else {
        celula->tipo = CELULA_PREENCHIDA;
    }
    
    return true;
}

bool podeColocarPalavra(Grid* grid, Palavra* palavra, const char* texto) {
    if (grid == NULL || palavra == NULL || texto == NULL) {
        return false;
    }
    
    //Verifica se o texto tem o tamanho correto
    if (strlen(texto) != palavra->tamanho) {
        return false;
    }
    
    //Calcula posição final
    int linhaFinal = palavra->inicio.linha;
    int colunaFinal = palavra->inicio.coluna;
    
    if (palavra->direcao == DIRECAO_HORIZONTAL) {
        colunaFinal += palavra->tamanho - 1;
    } else {
        linhaFinal += palavra->tamanho - 1;
    }
    
    // Verificar se a palavra cabe no grid
    if (!ehPosicaoValida(grid, linhaFinal, colunaFinal)) {
        return false;
    }
    
    // Verificar cada posição da palavra
    int linha = palavra->inicio.linha;
    int coluna = palavra->inicio.coluna;
    
    for (int i = 0; i < palavra->tamanho; i++) {
        Celula* celula = obterCelula(grid, linha, coluna);
        
        // Verificar se célula é bloqueada
        if (celula->tipo == CELULA_BLOQUEADA) {
            return false;
        }
        
        // Se célula já tem letra, verificar se é compatível
        if (celula->letra != '\0') {
            // Converter ambas para maiúsculas para comparação
            char letraCelula = toupper(celula->letra);
            char letraPalavra = toupper(texto[i]);
            
            if (letraCelula != letraPalavra) {
                return false;  // Conflito!
            }
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

bool colocarPalavra(Grid* grid, Palavra* palavra, const char* texto) {
    // Verificar se pode colocar
    if (!podeColocarPalavra(grid, palavra, texto)) {
        return false;
    }
    
    // Colocar cada letra
    int linha = palavra->inicio.linha;
    int coluna = palavra->inicio.coluna;
    
    for (int i = 0; i < palavra->tamanho; i++) {
        char letra = toupper(texto[i]);  // Converter para maiúscula
        
        // Colocar letra (definirLetraCelula já valida)
        definirLetraCelula(grid, linha, coluna, letra);
        
        // Avançar para próxima posição
        if (palavra->direcao == DIRECAO_HORIZONTAL) {
            coluna++;
        } else {
            linha++;
        }
    }
    
    // Atualizar palavra
    strncpy(palavra->textoAtual, texto, TAMANHO_MAX_PALAVRA);
    palavra->textoAtual[TAMANHO_MAX_PALAVRA] = '\0';
    palavra->estaCompleta = true;
    
    return true;
}

bool removerPalavra(Grid* grid, Palavra* palavra) {
    if (grid == NULL || palavra == NULL) {
        return false;
    }
    
    // Remover cada letra da palavra
    int linha = palavra->inicio.linha;
    int coluna = palavra->inicio.coluna;
    
    for (int i = 0; i < palavra->tamanho; i++) {
        Celula* celula = obterCelula(grid, linha, coluna);
        
        if (celula != NULL && !celula->eFixa) {
            // Verificar se esta letra faz parte de outra palavra
            // Por simplicidade, sempre remove (pode ser melhorado depois)
            // TODO (Pessoa 1): Implementar verificação de cruzamento se necessário
            
            celula->letra = '\0';
            celula->tipo = CELULA_VAZIA;
        }
        
        // Avançar para próxima posição
        if (palavra->direcao == DIRECAO_HORIZONTAL) {
            coluna++;
        } else {
            linha++;
        }
    }
    
    // Atualizar palavra
    palavra->textoAtual[0] = '\0';
    palavra->estaCompleta = false;
    
    return true;
}

bool adicionarPalavra(Grid* grid, Palavra palavra) {
    if (grid == NULL) {
        return false;
    }
    
    // Verificar se há espaço
    if (grid->numPalavras >= MAX_PALAVRAS) {
        return false;
    }
    
    // Adicionar palavra
    grid->palavras[grid->numPalavras] = palavra;
    grid->numPalavras++;
    
    return true;
}

Palavra* obterPalavra(Grid* grid, int indice) {
    if (grid == NULL || indice < 0 || indice >= grid->numPalavras) {
        return NULL;
    }
    
    return &grid->palavras[indice];
}

/* ========== UTILIDADES ========== */

Grid* copiarGrid(Grid* origem) {
    if (origem == NULL) {
        return NULL;
    }
    
    // Criar novo grid
    Grid* copia = criarGrid(origem->linhas, origem->colunas);
    if (copia == NULL) {
        return NULL;
    }
    
    // Copiar todas as células
    for (int i = 0; i < TAMANHO_MAX_GRID; i++) {
        for (int j = 0; j < TAMANHO_MAX_GRID; j++) {
            copia->celulas[i][j] = origem->celulas[i][j];
        }
    }
    
    // Copiar palavras
    copia->numPalavras = origem->numPalavras;
    for (int i = 0; i < origem->numPalavras; i++) {
        copia->palavras[i] = origem->palavras[i];
    }
    
    return copia;
}

void imprimirGrid(Grid* grid) {
    if (grid == NULL) {
        printf("Grid é NULL\n");
        return;
    }
    
    printf("\n");
    printf("Grid %dx%d (%d palavras):\n", grid->linhas, grid->colunas, grid->numPalavras);
    printf("\n");
    
    // Imprimir cabeçalho de colunas
    printf("   ");
    for (int j = 0; j < grid->colunas; j++) {
        printf(" %2d", j);
    }
    printf("\n");
    
    printf("   ");
    for (int j = 0; j < grid->colunas; j++) {
        printf("---");
    }
    printf("\n");
    
    // Imprimir linhas
    for (int i = 0; i < grid->linhas; i++) {
        printf("%2d |", i);
        
        for (int j = 0; j < grid->colunas; j++) {
            Celula* celula = &grid->celulas[i][j];
            
            if (celula->tipo == CELULA_BLOQUEADA) {
                printf(" ##");  // Célula bloqueada
            } else if (celula->letra != '\0') {
                printf("  %c", celula->letra);  // Letra
            } else {
                printf("  .");  // Vazia
            }
        }
        
        printf("\n");
    }
    
    printf("\n");
    
    // Imprimir palavras
    if (grid->numPalavras > 0) {
        printf("Palavras:\n");
        for (int i = 0; i < grid->numPalavras; i++) {
            Palavra* palavra = &grid->palavras[i];
            printf("  %d. (%d,%d) %s tam=%d: %s\n", 
                   i + 1,
                   palavra->inicio.linha, 
                   palavra->inicio.coluna,
                   palavra->direcao == DIRECAO_HORIZONTAL ? "H" : "V",
                   palavra->tamanho,
                   palavra->dica);
            
            if (palavra->estaCompleta) {
                printf("     Atual: %s\n", palavra->textoAtual);
            }
        }
    }
    
    printf("\n");
}

bool ehPosicaoValida(Grid* grid, int linha, int coluna) {
    if (grid == NULL) {
        return false;
    }
    
    return (linha >= 0 && linha < grid->linhas && coluna >= 0 && coluna < grid->colunas);
}

int contarCelulasVazias(Grid* grid) {
    if (grid == NULL) {
        return 0;
    }
    
    int contador = 0;
    
    for (int i = 0; i < grid->linhas; i++) {
        for (int j = 0; j < grid->colunas; j++) {
            Celula* celula = &grid->celulas[i][j];
            
            // Contar células não-bloqueadas sem letra
            if (celula->tipo != CELULA_BLOQUEADA && celula->letra == '\0') {
                contador++;
            }
        }
    }
    
    return contador;
}

bool gridEstaCompleto(Grid* grid) {
    return (contarCelulasVazias(grid) == 0);
}
