#include "config.h"
#include "engine.h"
#include "mesh.h"
#include "raylib.h"

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    Mesh_read_model();

    while (!WindowShouldClose()) {
        Engine_render();
    }

    CloseWindow();
    return 0;
}

