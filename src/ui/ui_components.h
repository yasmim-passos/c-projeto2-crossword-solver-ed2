#ifndef UI_COMPONENTS_H
#define UI_COMPONENTS_H

#include <raylib.h>
#include <stdbool.h>

// Modern Color Palette
// COLORS - Blue & Yellow Theme
#define UI_COLOR_PRIMARY      (Color){ 70, 160, 240, 255 }   // Lighter Sky Blue (Buttons, Headers)
#define UI_COLOR_SECONDARY    (Color){ 255, 204, 0, 255 }    // Yellow (Accents)
#define UI_COLOR_ACCENT       (Color){ 255, 240, 100, 255 }  // Light Yellow (Selection)
#define UI_COLOR_BG           (Color){ 245, 250, 255, 255 }  // Very pale blue/white (Background)
#define UI_COLOR_TEXT         (Color){ 20, 30, 60, 255 }     // Dark Blue/Black (Text)
#define UI_COLOR_BUTTON       (Color){ 70, 160, 240, 255 }   // Lighter Sky Blue
#define UI_COLOR_BUTTON_HOVER (Color){ 100, 180, 250, 255 }  // Even Lighter Blue
#define UI_COLOR_GRID_LINE    (Color){ 40, 100, 180, 255 }   // Mid Blue Lines
#define UI_COLOR_BLOCK        (Color){ 70, 160, 240, 255 }   // Lighter Sky Blue Blocks
#define UI_COLOR_CORRECT      (Color){ 46, 204, 113, 255 }   // Green (Correct)

// Draw a styled button with rounded corners and hover effect
bool GuiButton(Rectangle rect, const char* text);

// Draw a panel with shadow and rounded corners
void GuiPanel(Rectangle rect);

// Helper to draw centered text
void DrawTextCentered(const char* text, int cx, int cy, int fontSize, Color color);

// Helper to draw text wrapped within a rectangle
void DrawTextWrapped(const char *text, Rectangle rec, int fontSize, Color color);

#endif // UI_COMPONENTS_H
