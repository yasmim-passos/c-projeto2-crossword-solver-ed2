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
    if (!text || strlen(text) == 0) return;

    int currentLineY = (int)rec.y;
    int currentLineX = (int)rec.x;
    int lineHeight = fontSize + 4; // Add some spacing
    
    // Copy text to modify or parse
    // We will parse character by character to handle \n and spaces
    const char* ptr = text;
    char wordBuffer[256];
    int wordLen = 0;
    
    char lineBuffer[1024] = "";
    
    while (*ptr != '\0') {
        if (*ptr == '\n') {
            // Force line break
            // Draw current line
            if (strlen(lineBuffer) > 0) {
                 DrawText(lineBuffer, currentLineX, currentLineY, fontSize, color);
            }
            currentLineY += lineHeight;
            strcpy(lineBuffer, ""); // Reset line
            ptr++; // Skip \n
            continue;
        }
        
        // Read word
        wordLen = 0;
        // const char* startWord = ptr; // Unused
        while (*ptr != ' ' && *ptr != '\n' && *ptr != '\0') {
             wordBuffer[wordLen++] = *ptr++;
        }
        wordBuffer[wordLen] = '\0';
        
        // Check if word fits
        const char* space = (strlen(lineBuffer) > 0) ? " " : "";
        int currentW = MeasureText(lineBuffer, fontSize);
        int wordW = MeasureText(wordBuffer, fontSize);
        int spaceW = MeasureText(" ", fontSize);
        
        int totalW = currentW + (strlen(space) > 0 ? spaceW : 0) + wordW;
        
        if (totalW > rec.width) {
            // Draw current line and clear
            DrawText(lineBuffer, currentLineX, currentLineY, fontSize, color);
            currentLineY += lineHeight;
            strcpy(lineBuffer, wordBuffer);
        } else {
            if (strlen(lineBuffer) > 0) strcat(lineBuffer, " ");
            strcat(lineBuffer, wordBuffer);
        }
        
        if (*ptr == ' ') ptr++; // Skip space
    }
    
    // Draw last line
    if (strlen(lineBuffer) > 0) {
        DrawText(lineBuffer, currentLineX, currentLineY, fontSize, color);
    }
}
