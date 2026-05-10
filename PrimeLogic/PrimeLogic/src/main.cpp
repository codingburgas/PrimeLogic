#include "raylib.h"

int main()
{
    InitWindow(1280, 860, "PrimeLogic-Sports Score Manager");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color{ 245, 247, 250, 255 });
        DrawText("PrimeLogic-Sports Score Manager", 24, 24, 28, DARKBLUE);
        DrawText("...", 24, 70, 18, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
