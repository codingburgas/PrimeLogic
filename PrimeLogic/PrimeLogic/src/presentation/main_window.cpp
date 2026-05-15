#include "presentation/main_window.h"

#include "raylib.h"

void renderApp(AppState* state)
{
    (void)state;
    ClearBackground(Color{ 245, 247, 250, 255 });
    DrawText("Sports Score Manager", 24, 18, 28, DARKBLUE);
    DrawText("...", 24, 64, 16, DARKGRAY);
}