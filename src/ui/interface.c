#include "interface.h"
#include "ui_components.h"
#include <stdio.h>
#include <math.h>

static int selectedX = -1;
static int selectedY = -1;

// Animation state
static float cellScales[TAMANHO_MAX_GRID][TAMANHO_MAX_GRID];
static float timeAccumulator = 0.0f;

void InitInterface(void) {
    // Initialize animation states
    for (int y = 0; y < TAMANHO_MAX_GRID; y++) {
        for (int x = 0; x < TAMANHO_MAX_GRID; x++) {
            cellScales[y][x] = 1.0f;
        }
    }
}

void DrawCrosswordGrid(Grid* grid, int offsetX, int offsetY, int cellSize) {
    int rows = (grid->linhas > 0) ? grid->linhas : TAMANHO_MAX_GRID;
    int cols = (grid->colunas > 0) ? grid->colunas : TAMANHO_MAX_GRID;

    timeAccumulator += GetFrameTime();

    // Draw Grid Background Shadow/Container
    Rectangle gridRect = { 
        (float)offsetX - 5, 
        (float)offsetY - 5, 
        (float)(cols * cellSize + 10), 
        (float)(rows * cellSize + 10) 
    };
    DrawRectangleRounded(gridRect, 0.02f, 10, Fade(GRAY, 0.2f));

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            // Animation logic: decay scale back to 1.0
            if (cellScales[y][x] > 1.0f) {
                cellScales[y][x] = Lerp(cellScales[y][x], 1.0f, 0.1f);
            }

            float scale = cellScales[y][x];
            float size = cellSize * scale;
            float offset = (size - cellSize) / 2.0f;

            Rectangle rect = {
                (float)(offsetX + x * cellSize) - offset,
                (float)(offsetY + y * cellSize) - offset,
                (float)size,
                (float)size
            };

            Celula* cv = &grid->celulas[y][x];
            
            // Draw Cell Background
            if (cv->tipo == CELULA_BLOQUEADA) {
                DrawRectangleRounded(rect, 0.1f, 5, UI_COLOR_GRID_BLACK);
            } else {
                Color bgColor = WHITE;
                
                // Selection Pulse Calculation
                if (x == selectedX && y == selectedY) {
                    float pulse = (sinf(timeAccumulator * 5.0f) + 1.0f) * 0.5f; // 0 to 1
                    bgColor = ColorAlphaBlend(WHITE, UI_COLOR_SELECTION, Fade(WHITE, 0.2f + 0.3f * pulse));
                }
                
                DrawRectangleRounded(rect, 0.1f, 5, bgColor);
                DrawRectangleRoundedLines(rect, 0.1f, 5, 2, LIGHTGRAY);

                // Draw Number if exists
                if (cv->numero > 0) {
                    char numStr[4];
                    sprintf(numStr, "%d", cv->numero);
                    DrawText(numStr, (int)rect.x + 4, (int)rect.y + 2, 10, GRAY);
                }

                // Draw Letter
                if (cv->letra != '\0' && cv->letra != ' ') {
                    char charStr[2] = { cv->letra, '\0' };
                    Color textColor = cv->eFixa ? UI_COLOR_PRIMARY : UI_COLOR_TEXT;
                    DrawTextCentered(charStr, (int)(rect.x + rect.width/2), (int)(rect.y + rect.height/2), (int)(24 * scale), textColor);
                }
            }
        }
    }
}

void DrawSolverStatus(bool is_running, int steps) {
    if (is_running) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(WHITE, 0.8f));
        DrawText("Solver Running...", GetScreenWidth()/2 - 100, GetScreenHeight()/2 - 20, 30, UI_COLOR_ACCENT);
        
        char stepStr[32];
        sprintf(stepStr, "Steps: %d", steps);
        DrawText(stepStr, GetScreenWidth()/2 - 50, GetScreenHeight()/2 + 20, 20, GRAY);
    }
}

void UpdateInterface(Grid* grid, EstadoJogo* estado) {
    // Mouse Interaction for Selection
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        // Constants matching DrawGrid (should be shared config ideally)
        int offsetX = 50; 
        int offsetY = 100;
        int cellSize = 40;

        int gx = (int)(mouse.x - offsetX) / cellSize;
        int gy = (int)(mouse.y - offsetY) / cellSize;
        
        int rows = (grid->linhas > 0) ? grid->linhas : TAMANHO_MAX_GRID;
        int cols = (grid->colunas > 0) ? grid->colunas : TAMANHO_MAX_GRID;

        if (gx >= 0 && gx < cols && gy >= 0 && gy < rows) {
            if (grid->celulas[gy][gx].tipo != CELULA_BLOQUEADA) {
                selectedX = gx;
                selectedY = gy;
                cellScales[gy][gx] = 1.2f; // Slight bump on selection
            }
        } else {
            selectedX = -1;
            selectedY = -1;
        }
    }

    // Keyboard Interaction
    if (selectedX != -1 && selectedY != -1) {
        if (estado->modo == MODO_USUARIO && grid->celulas[selectedY][selectedX].eFixa) {
            return;
        }

        int key = GetKeyPressed();
        if ((key >= 65 && key <= 90) || (key >= 97 && key <= 122)) { // A-Z
             if (key >= 97) key -= 32; // Uppercase
             grid->celulas[selectedY][selectedX].letra = (char)key;
             grid->celulas[selectedY][selectedX].tipo = CELULA_PREENCHIDA;
             
             // Pop animation
             cellScales[selectedY][selectedX] = 1.5f; 
        } else if (key == KEY_BACKSPACE || key == KEY_DELETE) {
             grid->celulas[selectedY][selectedX].letra = '\0';
             grid->celulas[selectedY][selectedX].tipo = CELULA_VAZIA;
             cellScales[selectedY][selectedX] = 0.8f; // Shrink slightly on delete
        }
    }
}
