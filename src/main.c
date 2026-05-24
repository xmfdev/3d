#include "config.h"
#include "engine.h"
#include "raylib.h"

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    while (!WindowShouldClose()) {
        Engine_render();
    }

    CloseWindow();
    return 0;
}

