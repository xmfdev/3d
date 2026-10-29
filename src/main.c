#include "config.h"
#include "engine.h"
#include "raylib.h"

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_FULLSCREEN_MODE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    Engine_setup();

    while (!WindowShouldClose()) {
        Engine_render();
    }

    CloseWindow();
    return 0;
}

