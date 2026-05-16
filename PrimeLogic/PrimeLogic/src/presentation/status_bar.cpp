#include "presentation/status_bar.h"
#include "presentation/raylib_ui.h"

#include "raylib.h"

void renderStatusSection(const AppState* state, int x, int y, int width)
{
    /*
    Какво: Рисува лента със статус съобщение в долната част на прозореца.
    Как: Сив фон с текст; ако няма съобщение, показва "Ready.".
    Защо: Потребителят винаги вижда обратна връзка след действие.
    */
    DrawRectangle(x, y, width, 30, Color{ 230, 235, 240, 255 });
    DrawRectangleLines(x, y, width, 30, GRAY);

    const char* message = state->statusMessage[0] == '\0' ? "Ready." : state->statusMessage;
    drawUiText(x + 12, y + 6, message, 18, DARKBLUE);
}
