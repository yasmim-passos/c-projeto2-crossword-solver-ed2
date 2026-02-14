#include "puzzle_generator.h"
#include "solver.h"
#include "../data/grid.h"
#include "../data/dictionary.h" // Needed for fetching random words
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

static bool seeded = false;

void Generator_Init(void) {
    if (!seeded) {
        srand(time(NULL));
        seeded = true;
    }
}

// --- FUNCOES AUXILIARES ---

// Verifica se a colocacao de uma palavra valida (limites e vizinhos)
// Retorna true se valida.
static bool IsPlacementValid(Grid* g, int row, int col, int len, Direcao dir, const char* word) {
    int r = row;
    int c = col;
    
    // 1. Limites Basicos
    if (r < 0 || c < 0) return false;
    if (dir == DIRECAO_HORIZONTAL) {
        if (c + len > g->colunas) return false;
    } else {
        if (r + len > g->linhas) return false;
    }

    // 2. Colisao e Vizinhos
    // Iteramos sobre as celulas da nova palavra
    for (int i = 0; i < len; i++) {
        // Posicao atual da celula
        int curR = (dir == DIRECAO_HORIZONTAL) ? r : r + i;
        int curC = (dir == DIRECAO_HORIZONTAL) ? c + i : c;
        char letter = word[i];

        // Verifica conteudo desta celula
        Celula* cell = &g->celulas[curR][curC];
        
        // Se a celula ja esta preenchida
        if (cell->tipo == CELULA_PREENCHIDA) {
            // Deve coincidir a letra exata
            if (cell->letra != letter) return false;
        } 
        else if (cell->tipo == CELULA_BLOQUEADA || cell->tipo == CELULA_VAZIA) {
             // Se vazia/bloqueada, devemos garantir que nao estamos tocando outras palavras "de lado" 
             // (criando nao-palavras de 2 letras), A MENOS que seja o ponto de cruzamento.
             // Essa logica complexa. 
             // Simplificado: regras padrao exigem que qualquer celula adjacente 
             // perpendicular ao fluxo deve ser vazia/bloqueada, a menos que seja um cruzamento valido.
             
             // MAS, como estamos construindo *construtivamente*, podemos simplificar:
             // Apenas permitir colocacao se a celula for VAZIA/BLOQUEADA.
        }
    }
    
    // 3. Verifica Limites da Palavra (Inicio/Fim)
    // Celula "antes" da palavra deve ser vazia/bloqueada/limite
    int preR = (dir == DIRECAO_HORIZONTAL) ? r : r - 1;
    int preC = (dir == DIRECAO_HORIZONTAL) ? c - 1 : c;
    if (preR >= 0 && preC >= 0 && preR < g->linhas && preC < g->colunas) {
        if (g->celulas[preR][preC].tipo == CELULA_PREENCHIDA) return false;
    }
    
    // Celula "depois" da palavra deve ser vazia/bloqueada/limite
    int postR = (dir == DIRECAO_HORIZONTAL) ? r : r + len;
    int postC = (dir == DIRECAO_HORIZONTAL) ? c + len : c;
    if (postR < g->linhas && postC < g->colunas) {
        if (g->celulas[postR][postC].tipo == CELULA_PREENCHIDA) return false;
    }

    // 4. Checagem "Paralela" (evitar palavras lado a lado)
    // Iterar novamente, verificando estritamente vizinhos perpendiculares
    for (int i = 0; i < len; i++) {
        int curR = (dir == DIRECAO_HORIZONTAL) ? r : r + i;
        int curC = (dir == DIRECAO_HORIZONTAL) ? c + i : c;
        
        // Se esta celula ja estava preenchida, e um cruzamento, entao vizinhos sao OK (pertencem a palavra cruzada)
        if (g->celulas[curR][curC].tipo == CELULA_PREENCHIDA) continue; 

        // Verifica Vizinhos Perpendiculares
        int n1R = curR + ((dir == DIRECAO_HORIZONTAL) ? -1 : 0);
        int n1C = curC + ((dir == DIRECAO_HORIZONTAL) ? 0 : -1);
        int n2R = curR + ((dir == DIRECAO_HORIZONTAL) ? 1 : 0);
        int n2C = curC + ((dir == DIRECAO_HORIZONTAL) ? 0 : 1);

        if (n1R >= 0 && n1C >= 0 && n1R < g->linhas && n1C < g->colunas) {
            if (g->celulas[n1R][n1C].tipo == CELULA_PREENCHIDA) return false;
        }
        if (n2R >= 0 && n2C >= 0 && n2R < g->linhas && n2C < g->colunas) {
            if (g->celulas[n2R][n2C].tipo == CELULA_PREENCHIDA) return false;
        }
    }

    return true;
}


bool Generator_GenerateLevel(Grid* grid, int levelIndex) {
    if (!grid) return false;
    Generator_Init();

    // 1. Resetar Grid (Tudo Bloqueado/Azul)
    inicializarGrid(grid, TAMANHO_MAX_GRID, TAMANHO_MAX_GRID);
    grid->numPalavras = 0;

    // 2. Determinar Dificuldade
    int targetWords = 6 + (levelIndex * 2); 
    if (targetWords > MAX_PALAVRAS) targetWords = MAX_PALAVRAS;
    
    // 3. Colocar Primeira Palavra (Centro)
    // Obter candidato aleatorio 5-8 chars
    int len = 5 + (rand() % 4);
    char** candidates = NULL;
    int numCandidates = 0;
    dict_search_by_size(len, &candidates, &numCandidates);
    
    if (numCandidates > 0) {
        char* startWord = candidates[rand() % numCandidates];
        
        Palavra* p = &grid->palavras[grid->numPalavras];
        p->direcao = DIRECAO_HORIZONTAL;
        p->tamanho = len;
        p->inicio.linha = 7; // Centro Y
        p->inicio.coluna = 7 - (len/2); // Centro X
        strcpy(p->resposta, startWord);
        strcpy(p->textoAtual, "");
        p->estaCompleta = false;
        // Buscar dica
        dict_check_word(p->resposta, p->dica, TAMANHO_MAX_DICA);
        if (strlen(p->dica) == 0) strcpy(p->dica, "Sem Dica");

        // Colocar no Grid
        for(int k=0; k<len; k++) {
            grid->celulas[p->inicio.linha][p->inicio.coluna + k].letra = p->resposta[k];
            grid->celulas[p->inicio.linha][p->inicio.coluna + k].tipo = CELULA_PREENCHIDA;
        }
        grid->numPalavras++;
    }
    // Limpar primeiros candidatos
    if (candidates) {
        for(int i=0; i<numCandidates; i++) free(candidates[i]);
        free(candidates);
    }
    
    // 4. Loop de Crescimento
    int fails = 0;
    while (grid->numPalavras < targetWords && fails < 50) {
        // Escolher uma palavra existente aleatoria para cruzar
        int srcIdx = rand() % grid->numPalavras;
        Palavra* srcP = &grid->palavras[srcIdx];
        
        // Escolher ponto de interseccao aleatorio nesta palavra
        int interOffset = rand() % srcP->tamanho;
        char pivotChar = srcP->resposta[interOffset];
        int pivotR = srcP->inicio.linha + ((srcP->direcao == DIRECAO_VERTICAL) ? interOffset : 0);
        int pivotC = srcP->inicio.coluna + ((srcP->direcao == DIRECAO_HORIZONTAL) ? interOffset : 0);
        
        // Detalhes da nova palavra
        Direcao newDir = (srcP->direcao == DIRECAO_HORIZONTAL) ? DIRECAO_VERTICAL : DIRECAO_HORIZONTAL;
        int newLen = 4 + (rand() % 4); // 4 a 7
        
        // Encontrar palavra de newLen que tenha pivotChar em alguma posicao 'j'
        char** searchList = NULL;
        int count = 0;
        dict_search_by_size(newLen, &searchList, &count);
        
        bool placed = false;
        if (count > 0) {
            // Tentar 10 candidatos aleatorios
            for(int attempt=0; attempt<10; attempt++) {
                char* cand = searchList[rand() % count];
                
                // Encontrar ocorrencias de pivotChar em cand
                for(int j=0; j<newLen; j++) {
                    if (cand[j] == pivotChar) {
                         // Colocacao potencial
                         // Posicao inicial da nova palavra
                         int startR = pivotR - ((newDir == DIRECAO_VERTICAL) ? j : 0);
                         int startC = pivotC - ((newDir == DIRECAO_HORIZONTAL) ? j : 0);
                         
                         // Verificar Duplicata
                         bool dup = false;
                         for(int w=0; w<grid->numPalavras; w++) {
                             if (strcmp(grid->palavras[w].resposta, cand) == 0) dup = true;
                         }
                         if (dup) continue;

                         if (IsPlacementValid(grid, startR, startC, newLen, newDir, cand)) {
                             // Colocar!
                             Palavra* newP = &grid->palavras[grid->numPalavras];
                             newP->direcao = newDir;
                             newP->tamanho = newLen;
                             newP->inicio.linha = startR;
                             newP->inicio.coluna = startC;
                             strcpy(newP->resposta, cand);
                             strcpy(newP->textoAtual, "");
                             newP->estaCompleta = false;
                             dict_check_word(newP->resposta, newP->dica, TAMANHO_MAX_DICA);
                             if (strlen(newP->dica) == 0) strcpy(newP->dica, "Sem Dica");

                             // Preencher Grid
                             for(int k=0; k<newLen; k++) {
                                 int rr = (newDir==DIRECAO_VERTICAL) ? startR+k : startR;
                                 int cc = (newDir==DIRECAO_HORIZONTAL) ? startC+k : startC;
                                 grid->celulas[rr][cc].letra = cand[k];
                                 grid->celulas[rr][cc].tipo = CELULA_PREENCHIDA;
                             }
                             
                             grid->numPalavras++;
                             placed = true;
                             break;
                         }
                    }
                }
                if (placed) break;
            }
        }
        
        if (searchList) {
            for(int i=0; i<count; i++) free(searchList[i]);
            free(searchList);
        }
        
        if (!placed) fails++; else fails = 0;
    }
    
    // 5. Finalizar: Define todas as celulas preenchidas para status VAZIA (esconder letras) logica?
    // Espere, a logica do jogo requer `CELULA_VAZIA` (Caixa branca vazia) para digitar.
    // `CELULA_PREENCHIDA` implica que TEM uma letra. 
    // IsPlacementValid usou PREENCHIDA para checar colisoes.
    // Agora devemos converter o "Grid Solucao" gerado em um "Grid Jogavel".
    
    // Realmente, `inicializarGrid` define TIPO para BLOQUEADA (Azul).
    // Nosso gerador definiu TIPO para PREENCHIDA (para logica) e LETRA para Resposta.
    // Para o jogo comecar, normalmente queremos as letras ESCONDIDAS mas celulas BRANCAS (VAZIA).
    
    /*
    typedef struct {
        TipoCelula tipo;    // Tipo da célula (vazia, bloqueada, preenchida)
        char letra;         // Letra atual ('\0' se vazia) ... ISTO E ENTRADA DO USUARIO
    } Celula;
    */
    
    for(int r=0; r<grid->linhas; r++) {
        for(int c=0; c<grid->colunas; c++) {
            if (grid->celulas[r][c].tipo == CELULA_PREENCHIDA) {
                grid->celulas[r][c].tipo = CELULA_VAZIA; // Tornar caixa de entrada branca
                grid->celulas[r][c].letra = '\0';        // Limpar resposta (esta armazenada na lista Palavra)
            }
            // Bloqueada permanece Bloqueada.
        }
    }
    
    // Tambem Recalcular Numeros
    // Precisamos da logica RecalculateNumbers que geralmente esta em `main` ou interface?
    // Na verdade `interface.c` ou logica geralmente atribui numeros.
    // Vamos implementar numeracao simples aqui.
    int n = 1;
    for(int i=0; i<grid->numPalavras; i++) {
        Palavra* p = &grid->palavras[i];
        if (grid->celulas[p->inicio.linha][p->inicio.coluna].numero == 0) {
             grid->celulas[p->inicio.linha][p->inicio.coluna].numero = n++;
        }
    }

    return true;
}
