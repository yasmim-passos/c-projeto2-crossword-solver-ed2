#include "ui_components.h"

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
    DrawRectangleRoundedLines(drawRect, 0.2f, 10, 2, WHITE);
    
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
