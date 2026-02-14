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

// --- HELPER FUNCTIONS ---

// Check if a word placement is valid (bounds and neighbors)
// Returns true if valid.
static bool IsPlacementValid(Grid* g, int row, int col, int len, Direcao dir, const char* word) {
    int r = row;
    int c = col;
    
    // 1. Basic Bounds
    if (r < 0 || c < 0) return false;
    if (dir == DIRECAO_HORIZONTAL) {
        if (c + len > g->colunas) return false;
    } else {
        if (r + len > g->linhas) return false;
    }

    // 2. Collision & Neighbor Check
    // We iterate over the cells of the new word
    for (int i = 0; i < len; i++) {
        // Current cell position
        int curR = (dir == DIRECAO_HORIZONTAL) ? r : r + i;
        int curC = (dir == DIRECAO_HORIZONTAL) ? c + i : c;
        char letter = word[i];

        // Check content of this cell
        Celula* cell = &g->celulas[curR][curC];
        
        // If cell is already filled
        if (cell->tipo == CELULA_PREENCHIDA) {
            // Must match exact letter
            if (cell->letra != letter) return false;
        } 
        else if (cell->tipo == CELULA_BLOQUEADA || cell->tipo == CELULA_VAZIA) {
             // If empty/blocked, we must ensure we are not touching other words "sideways" 
             // (creating 2-letter non-words), UNLESS it's the crossing point.
             // This logic is complex. 
             // Simplified: standard crossword rules require that any adjacent cell 
             // perpendicular to flow must be empty/blocked, unless it's a valid crossing.
             
             // BUT, since we are building *constructively*, we can simplify:
             // Only allow placement if the cell is EMPTY/BLOQUEADA.
        }
    }
    
    // 3. Verify Word Limits (Start/End)
    // Cell "before" word must be empty/blocked/boundary
    int preR = (dir == DIRECAO_HORIZONTAL) ? r : r - 1;
    int preC = (dir == DIRECAO_HORIZONTAL) ? c - 1 : c;
    if (preR >= 0 && preC >= 0 && preR < g->linhas && preC < g->colunas) {
        if (g->celulas[preR][preC].tipo == CELULA_PREENCHIDA) return false;
    }
    
    // Cell "after" word must be empty/blocked/boundary
    int postR = (dir == DIRECAO_HORIZONTAL) ? r : r + len;
    int postC = (dir == DIRECAO_HORIZONTAL) ? c + len : c;
    if (postR < g->linhas && postC < g->colunas) {
        if (g->celulas[postR][postC].tipo == CELULA_PREENCHIDA) return false;
    }

    // 4. "Parallel" check (prevent side-by-side words)
    // Iterate again, checking strictly perpendicular neighbors
    for (int i = 0; i < len; i++) {
        int curR = (dir == DIRECAO_HORIZONTAL) ? r : r + i;
        int curC = (dir == DIRECAO_HORIZONTAL) ? c + i : c;
        
        // If this cell was already filled, it's a crossing, so neighbors are OK (they belong to crossing word)
        if (g->celulas[curR][curC].tipo == CELULA_PREENCHIDA) continue; 

        // Check Perpendicular Neighbors
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

    // 1. Reset Grid (All Blocked/Blue)
    inicializarGrid(grid, TAMANHO_MAX_GRID, TAMANHO_MAX_GRID);
    grid->numPalavras = 0;

    // 2. Determine Difficulty
    int targetWords = 6 + (levelIndex * 2); 
    if (targetWords > MAX_PALAVRAS) targetWords = MAX_PALAVRAS;
    
    // 3. Place First Word (Center)
    // Get random candidate 5-8 chars
    int len = 5 + (rand() % 4);
    char** candidates = NULL;
    int numCandidates = 0;
    dict_search_by_size(len, &candidates, &numCandidates);
    
    if (numCandidates > 0) {
        char* startWord = candidates[rand() % numCandidates];
        
        Palavra* p = &grid->palavras[grid->numPalavras];
        p->direcao = DIRECAO_HORIZONTAL;
        p->tamanho = len;
        p->inicio.linha = 7; // Center Y
        p->inicio.coluna = 7 - (len/2); // Center X
        strcpy(p->resposta, startWord);
        strcpy(p->textoAtual, "");
        p->estaCompleta = false;
        // Fetch definition
        dict_check_word(p->resposta, p->dica, TAMANHO_MAX_DICA);
        if (strlen(p->dica) == 0) strcpy(p->dica, "Sem Dica");

        // Place on Grid
        for(int k=0; k<len; k++) {
            grid->celulas[p->inicio.linha][p->inicio.coluna + k].letra = p->resposta[k];
            grid->celulas[p->inicio.linha][p->inicio.coluna + k].tipo = CELULA_PREENCHIDA;
        }
        grid->numPalavras++;
    }
    // Cleanup first candidates
    if (candidates) {
        for(int i=0; i<numCandidates; i++) free(candidates[i]);
        free(candidates);
    }
    
    // 4. Grow Loop
    int fails = 0;
    while (grid->numPalavras < targetWords && fails < 50) {
        // Pick a random existing word to cross
        int srcIdx = rand() % grid->numPalavras;
        Palavra* srcP = &grid->palavras[srcIdx];
        
        // Pick a random intersection point on this word
        int interOffset = rand() % srcP->tamanho;
        char pivotChar = srcP->resposta[interOffset];
        int pivotR = srcP->inicio.linha + ((srcP->direcao == DIRECAO_VERTICAL) ? interOffset : 0);
        int pivotC = srcP->inicio.coluna + ((srcP->direcao == DIRECAO_HORIZONTAL) ? interOffset : 0);
        
        // New word details
        Direcao newDir = (srcP->direcao == DIRECAO_HORIZONTAL) ? DIRECAO_VERTICAL : DIRECAO_HORIZONTAL;
        int newLen = 4 + (rand() % 4); // 4 to 7
        
        // Find a word of newLen that has pivotChar at some position 'j'
        char** searchList = NULL;
        int count = 0;
        dict_search_by_size(newLen, &searchList, &count);
        
        bool placed = false;
        if (count > 0) {
            // Try 10 random candidates
            for(int attempt=0; attempt<10; attempt++) {
                char* cand = searchList[rand() % count];
                
                // Find occurrences of pivotChar in cand
                for(int j=0; j<newLen; j++) {
                    if (cand[j] == pivotChar) {
                         // Potential placement
                         // Start pos of new word
                         int startR = pivotR - ((newDir == DIRECAO_VERTICAL) ? j : 0);
                         int startC = pivotC - ((newDir == DIRECAO_HORIZONTAL) ? j : 0);
                         
                         // Check Duplicate
                         bool dup = false;
                         for(int w=0; w<grid->numPalavras; w++) {
                             if (strcmp(grid->palavras[w].resposta, cand) == 0) dup = true;
                         }
                         if (dup) continue;

                         if (IsPlacementValid(grid, startR, startC, newLen, newDir, cand)) {
                             // Place it!
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

                             // Fill Grid
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
    
    // 5. Finalize: Set all filled cells to EMPTY status (hide letters) logic?
    // Wait, the game logic requires `CELULA_VAZIA` (Empty white box) to type into.
    // `CELULA_PREENCHIDA` implies it HAS a letter. 
    // IsPlacementValid used PREENCHIDA to check collisions.
    // Now we must convert the generated "Solution Grid" into a "Playable Grid".
    
    // Actually, `inicializarGrid` sets TIPO to BLOQUEADA (Blue).
    // Our generator set TIPO to PREENCHIDA (for logic) and LETRA to Answer.
    // For the game to start, we typically want the letters HIDDEN but cells WHITE (VAZIA).
    // BUT, the structs say:
    // CELULA_VAZIA = 0 (White, no letter typed yet)
    // CELULA_PREENCHIDA = 2 (White, has letter)
    // We should reset TIPO to CELULA_VAZIA and keep `letra`? 
    // No, `letra` in struct `Celula` is the *user input*. The Answer is in `Palavra`.
    
    // Correct loop:
    /*
    typedef struct {
        TipoCelula tipo;    // Tipo da célula (vazia, bloqueada, preenchida)
        char letra;         // Letra atual ('\0' se vazia) ... THIS IS USER INPUT
    } Celula;
    */
    
    for(int r=0; r<grid->linhas; r++) {
        for(int c=0; c<grid->colunas; c++) {
            if (grid->celulas[r][c].tipo == CELULA_PREENCHIDA) {
                grid->celulas[r][c].tipo = CELULA_VAZIA; // Make it white input box
                grid->celulas[r][c].letra = '\0';        // Clear answer (it is stored in Palavra list)
            }
            // Blocked remains Blocked.
        }
    }
    
    // Also Recalculate Numbers
    // (We need the RecalculateNumbers logic which is usually in `main` or interface?
    // Actually `interface.c` or logic usually assigns numbers.
    // Let's implement simple numbering here.
    int n = 1;
    for(int i=0; i<grid->numPalavras; i++) {
        Palavra* p = &grid->palavras[i];
        if (grid->celulas[p->inicio.linha][p->inicio.coluna].numero == 0) {
             grid->celulas[p->inicio.linha][p->inicio.coluna].numero = n++;
        }
    }

    return true;
}
