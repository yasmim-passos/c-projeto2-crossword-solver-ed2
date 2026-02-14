#ifndef PUZZLE_GENERATOR_H
#define PUZZLE_GENERATOR_H

#include "../data/structs.h"
#include <stdbool.h>

// Initialize generator resources (seed random)
void Generator_Init(void);

// Generates a new random level into the provided grid
// Uses hardcoded templates for structure and Solver to fill content
bool Generator_GenerateLevel(Grid* grid, int levelIndex);

#endif // PUZZLE_GENERATOR_H
