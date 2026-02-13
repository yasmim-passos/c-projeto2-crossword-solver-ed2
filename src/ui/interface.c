#include "interface.h"
#include "ui_components.h"
#include <stdio.h>
#include <raymath.h>
#include <math.h>

static int selectedX = -1;
static int selectedY = -1;

// Animation state
static float cellScales[TAMANHO_MAX_GRID][TAMANHO_MAX_GRID];
// static float timeAccumulator = 0.0f; // Removed unused variable

void InitInterface(void) {
    // Initialize animation states
    for (int y = 0; y < TAMANHO_MAX_GRID; y++) {
        for (int x = 0; x < TAMANHO_MAX_GRID; x++) {
            cellScales[y][x] = 1.0f;
        }
    }
}

// Draw the grid with Classic Newspaper Style
void DrawCrosswordGrid(Grid* grid, int offsetX, int offsetY, int cellSize) {
    if (!grid) return;

    int rows = (grid->linhas > 0) ? grid->linhas : TAMANHO_MAX_GRID;
    int cols = (grid->colunas > 0) ? grid->colunas : TAMANHO_MAX_GRID;

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            Rectangle rect = {
                (float)(offsetX + x * cellSize),
                (float)(offsetY + y * cellSize),
                (float)cellSize,
                (float)cellSize
            };

            // Determine Cell Color
            Color bgColor = UI_COLOR_BG; // Default empty cell
            
            if (grid->celulas[y][x].tipo == CELULA_BLOQUEADA) {
                bgColor = UI_COLOR_BLOCK; // Blue Block
            } else if (x == selectedX && y == selectedY) {
                bgColor = UI_COLOR_ACCENT; // Yellow Selection
            } else {
                 // Check if this cell belongs to a completed/verified word
                 // For now, simple check: is it correct?
                 // (This logic might be complex to do per cell without extra state. 
                 //  Let's just use White for normal cells)
                 bgColor = WHITE;
                 
                 // If we have a verification feedback state in grid/interface, use UI_COLOR_CORRECT
                 // Assuming logic handled in main loop or separate draw pass?
                 // Let's rely on standard drawing for now.
            }

            // Draw Cell Background
            DrawRectangleRec(rect, bgColor);
            
            // Draw Borders (Blue Lines) - Only for non-blocked cells
            if (grid->celulas[y][x].tipo != CELULA_BLOQUEADA) {
                DrawRectangleLinesEx(rect, 1.0f, UI_COLOR_GRID_LINE);
            }

            // Draw Number if exists
            if (grid->celulas[y][x].numero > 0) {
                DrawText(TextFormat("%d", grid->celulas[y][x].numero), 
                         (int)rect.x + 3, (int)rect.y + 3, 10, UI_COLOR_TEXT);
            }

            // Draw Letter
            char letra = grid->celulas[y][x].letra;
            if (letra != '\0') {
                char letterStr[2] = { letra, '\0' };
                int fontSize = (int)(cellSize * 0.6f);
                int textW = MeasureText(letterStr, fontSize);
                
                // Draw Green Shadow if Correct (Verification logic needs to set a flag or we check dynamically)
                // For simplicity: If cell is marked "FIXED" (completed), draw Green.
                // Wait, 'eFixa' was for initial fixed letters. We can repurpose/add 'eCorreta'.
                // Let's assume verifying marks them as Fixed/Correct.
                
                Color txtColor = UI_COLOR_TEXT;
                if (grid->celulas[y][x].eFixa) { // Using eFixa to mean "Verified/Correct"
                     txtColor = UI_COLOR_CORRECT;
                     // Draw subtle shadow/glow
                     DrawText(letterStr, 
                         (int)(rect.x + (cellSize - textW) / 2) + 2, 
                         (int)(rect.y + (cellSize - fontSize) / 2 + 2) + 2, 
                         fontSize, Fade(UI_COLOR_CORRECT, 0.5f));
                }
                
                // Center letter
                DrawText(letterStr, 
                         (int)(rect.x + (cellSize - textW) / 2), 
                         (int)(rect.y + (cellSize - fontSize) / 2 + 2), 
                         fontSize, txtColor);
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
    if (!grid) return;
    
    // Keyboard Navigation
    // Keyboard Navigation (Skip Blocks)
    if (IsKeyPressed(KEY_RIGHT)) {
        int nx = selectedX;
        do { nx = (nx + 1) % grid->colunas; } while(grid->celulas[selectedY][nx].tipo == CELULA_BLOQUEADA && nx != selectedX);
        selectedX = nx;
    }
    if (IsKeyPressed(KEY_LEFT)) {
        int nx = selectedX;
        do { nx = (nx - 1 + grid->colunas) % grid->colunas; } while(grid->celulas[selectedY][nx].tipo == CELULA_BLOQUEADA && nx != selectedX);
        selectedX = nx;
    }
    if (IsKeyPressed(KEY_DOWN)) {
        int ny = selectedY;
        do { ny = (ny + 1) % grid->linhas; } while(grid->celulas[ny][selectedX].tipo == CELULA_BLOQUEADA && ny != selectedY);
        selectedY = ny;
    }
    if (IsKeyPressed(KEY_UP)) {
        int ny = selectedY;
        do { ny = (ny - 1 + grid->linhas) % grid->linhas; } while(grid->celulas[ny][selectedX].tipo == CELULA_BLOQUEADA && ny != selectedY);
        selectedY = ny;
    }

    // Mouse Selection
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        int offsetX = 50; 
        int offsetY = 100; // Match main.c DrawCrosswordGrid call
        int cellSize = 40; // Match main.c DrawCrosswordGrid call

        int gx = (int)(mouse.x - offsetX) / cellSize;
        int gy = (int)(mouse.y - offsetY) / cellSize;
        
        int rows = (grid->linhas > 0) ? grid->linhas : TAMANHO_MAX_GRID;
        int cols = (grid->colunas > 0) ? grid->colunas : TAMANHO_MAX_GRID;

        if (gx >= 0 && gx < cols && gy >= 0 && gy < rows) {
             // Only select if not blocked (optional, user preference)
             // or keep logic simple
             selectedX = gx;
             selectedY = gy;
             cellScales[gy][gx] = 1.2f;   
        } else {
             selectedX = -1;
             selectedY = -1;
        }
    }

    // Keyboard Interaction (Typing)
    if (selectedX != -1 && selectedY != -1) {
        // Skip check for MODO_USUARIO if passing simple State
        // if (estado->modo == MODO_USUARIO && ... eFixa)
        if (grid->celulas[selectedY][selectedX].eFixa) {
             // Do nothing if fixed
        } else {
            int key = GetKeyPressed();
            if ((key >= 65 && key <= 90) || (key >= 97 && key <= 122)) { // A-Z
                 if (key >= 97) key -= 32; // Uppercase
                 grid->celulas[selectedY][selectedX].letra = (char)key;
                 grid->celulas[selectedY][selectedX].tipo = CELULA_PREENCHIDA;
                 
                 // Pop animation
                 cellScales[selectedY][selectedX] = 1.5f; 
                 // Move to next cell automatically? (Optional)
            } else if (key == KEY_BACKSPACE || key == KEY_DELETE) {
                 grid->celulas[selectedY][selectedX].letra = '\0';
                 grid->celulas[selectedY][selectedX].tipo = CELULA_VAZIA;
                 cellScales[selectedY][selectedX] = 0.8f; 
            }
        }
    }
}

