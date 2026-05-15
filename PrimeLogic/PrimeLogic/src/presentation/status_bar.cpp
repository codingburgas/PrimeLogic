#include "presentation/status_bar.h"
#include "presentation/raylib_ui.h"

#include "raylib.h"

void renderStatusSection(const AppState* state, int x, int y, int width)
{
    DrawRectangle(x, y, width, 30, Color{ 230, 235, 240, 255 });
    DrawRectangleLines(x, y, width, 30, GRAY);

    const char* message = state->statusMessage[0] == '\0' ? "Ready." : state->statusMessage;
    drawUiText(x + 12, y + 6, message, 18, DARKBLUE);
}