#include "ui_components.h"
#include <string.h>
#include <stdio.h>

bool GuiButton(Rectangle rect, const char* text) {
    Vector2 mousePoint = GetMousePosition();
    bool isHover = CheckCollisionPointRec(mousePoint, rect);
    bool isClicked = false;
    bool isPressed = false;

    if (isHover) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            isPressed = true;
        }
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            isClicked = true;
        }
    }

    Color color = isHover ? UI_COLOR_BUTTON_HOVER : UI_COLOR_BUTTON;
    if (isPressed) color = UI_COLOR_PRIMARY;

    Rectangle drawRect = rect;
    if (isPressed) {
        drawRect.y += 2; // Press effect
    }

    // Shadow
    if (!isPressed) {
        DrawRectangleRounded((Rectangle){rect.x, rect.y + 4, rect.width, rect.height}, 0.2f, 10, Fade(GRAY, 0.5f));
    }

    DrawRectangleRounded(drawRect, 0.2f, 10, color);
    DrawRectangleRoundedLines(drawRect, 0.2f, 10, WHITE);
    
    int fontSize = 20;
    int textWidth = MeasureText(text, fontSize);
    
    Color textColor = isPressed ? WHITE : UI_COLOR_TEXT;
    DrawText(text, (int)(drawRect.x + (drawRect.width - textWidth) / 2), (int)(drawRect.y + (drawRect.height - fontSize) / 2), fontSize, textColor);

    return isClicked;
}

void GuiPanel(Rectangle rect) {
    // Shadow
    DrawRectangleRounded((Rectangle){rect.x + 4, rect.y + 4, rect.width, rect.height}, 0.05f, 10, Fade(GRAY, 0.3f));
    // Main Body
    DrawRectangleRounded(rect, 0.05f, 10, WHITE);
}

void DrawTextCentered(const char* text, int cx, int cy, int fontSize, Color color) {
    int textWidth = MeasureText(text, fontSize);
    DrawText(text, cx - textWidth / 2, cy - fontSize / 2, fontSize, color);
}

void DrawTextWrapped(const char *text, Rectangle rec, int fontSize, Color color) {
    // Basic word wrapping implementation
    if (!text || strlen(text) == 0) return;

    // int textLen = strlen(text); // Removed unused variable
    int currentLineY = (int)rec.y;
    int currentLineX = (int)rec.x;
    int lineHeight = fontSize + 2;
    
    char buffer[1024]; // Safe buffer for typical definition length
    strncpy(buffer, text, 1023);
    buffer[1023] = '\0';
    
    char* word = strtok(buffer, " ");
    char lineBuffer[256] = "";
    
    while (word != NULL) {
        char testLine[256];
        strcpy(testLine, lineBuffer);
        if (strlen(lineBuffer) > 0) strcat(testLine, " ");
        strcat(testLine, word);
        
        int width = MeasureText(testLine, fontSize);
        
        if (width > rec.width) {
            // Draw current line and move down
            DrawText(lineBuffer, currentLineX, currentLineY, fontSize, color);
            currentLineY += lineHeight;
            
            // Start new line with current word
            strcpy(lineBuffer, word);
        } else {
            // Check if word contains newline (from file reading if not cleaned)
            // But strtok splits by space. If definitions have \n, usage should handle it.
            // For now assuming definitions are single line strings from dictionary.c logic.
            strcpy(lineBuffer, testLine);
        }
        
        word = strtok(NULL, " ");
    }
    // Draw last line
    DrawText(lineBuffer, currentLineX, currentLineY, fontSize, color);
}
