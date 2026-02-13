#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include "../ui/interface.h"
#include "../data/dictionary.h"
#include "../ui/ui_components.h"
#include "../data/structs.h"
#include "../external/api_client.h"

int main() {
    dict_init();
    
    InitWindow(900, 700, "Crossword Puzzle - Polished");
    SetTargetFPS(60);
    
    InitInterface();

    EstadoJogo estado;
    estado.modo = MODO_USUARIO;
    estado.estaCompleto = false;
    estado.estaResolvido = false;
    
    Grid* g = &estado.grid;
    g->linhas = 10;
    g->colunas = 10;
    
    for (int y = 0; y < TAMANHO_MAX_GRID; y++) {
        for (int x = 0; x < TAMANHO_MAX_GRID; x++) {
            g->celulas[y][x].tipo = CELULA_VAZIA;
            g->celulas[y][x].letra = '\0';
            g->celulas[y][x].eFixa = false;
            g->celulas[y][x].numero = 0;
            
            if (x < 10 && y < 10) {
                if ((x + y) % 5 == 0) g->celulas[y][x].tipo = CELULA_BLOQUEADA;
            } else {
                g->celulas[y][x].tipo = CELULA_BLOQUEADA; 
            }
        }
    }
    
    g->celulas[0][0].numero = 1;

    char def[TAMANHO_MAX_DICA] = "Search a word...";
    char currentWord[64] = "Hello";

    while (!WindowShouldClose()) {
        UpdateInterface(g, &estado);
        
        BeginDrawing();
        ClearBackground(UI_COLOR_BG);
        
        // Main Panel
        GuiPanel((Rectangle){ 20, 20, 860, 660 });
        
        DrawText("Crossword Puzzle", 40, 40, 30, UI_COLOR_PRIMARY);
        DrawText("Polished Version", 40, 70, 20, GRAY);
        
        DrawCrosswordGrid(g, 50, 120, 45);
        
        // Side Panel for API
        int sideX = 550;
        DrawText("Dictionary API", sideX, 120, 24, UI_COLOR_ACCENT);
        DrawText("Word: 'hello'", sideX, 160, 20, UI_COLOR_TEXT);
        
        if (GuiButton((Rectangle){ (float)sideX, 200, 150, 40 }, "Check API")) {
             if (dict_check_word("hello", def, sizeof(def))) {
                 // def populated
             } else {
                 strcpy(def, "Not found.");
             }
        }
        
        // Definition Box
        DrawRectangleRounded((Rectangle){ (float)sideX, 260, 280, 200 }, 0.1f, 5, Fade(LIGHTGRAY, 0.3f));
        DrawTextWrapped(def, (Rectangle){ (float)sideX + 10, 270, 260, 180 }, 20, UI_COLOR_TEXT);

        EndDrawing();
    }

    CloseWindow();
    api_cleanup();

    return 0;
}
