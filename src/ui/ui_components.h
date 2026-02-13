#ifndef UI_COMPONENTS_H
#define UI_COMPONENTS_H

#include <raylib.h>
#include <stdbool.h>

// Modern Color Palette
#define UI_COLOR_BG           CLITERAL(Color){ 236, 240, 241, 255 } // Clouds
#define UI_COLOR_PRIMARY      CLITERAL(Color){ 52, 152, 219, 255 }  // Peter River
#define UI_COLOR_ACCENT       CLITERAL(Color){ 230, 126, 34, 255 }  // Carrot
#define UI_COLOR_TEXT         CLITERAL(Color){ 44, 62, 80, 255 }    // Midnight Blue
#define UI_COLOR_BUTTON       CLITERAL(Color){ 189, 195, 199, 255 } // Silver
#define UI_COLOR_BUTTON_HOVER CLITERAL(Color){ 149, 165, 166, 255 } // Concrete
#define UI_COLOR_GRID_BLACK   CLITERAL(Color){ 44, 62, 80, 255 }
#define UI_COLOR_SELECTION    CLITERAL(Color){ 130, 204, 221, 150 } // Semi-transparent Blue

// Draw a styled button with rounded corners and hover effect
bool GuiButton(Rectangle rect, const char* text);

// Draw a panel with shadow and rounded corners
void GuiPanel(Rectangle rect);

// Helper to draw centered text
void DrawTextCentered(const char* text, int cx, int cy, int fontSize, Color color);

#endif // UI_COMPONENTS_H
