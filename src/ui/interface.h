#ifndef INTERFACE_H
#define INTERFACE_H

#include "../data/structs.h"

// Initialize Interface resources
void InitInterface(void);

// Main render function for the grid
void DrawCrosswordGrid(Grid* grid, int offsetX, int offsetY, int cellSize);

// Draw solver status overlay
void DrawSolverStatus(bool is_running, int steps);

// Update logic for UI interactions
void UpdateInterface(Grid* grid, EstadoJogo* estado);

#endif // INTERFACE_H
