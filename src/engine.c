#include "engine.h"
#include "raylib.h"
#include "vec.h"
#include <stddef.h>
#include <stdlib.h>

static float angle = 0.0;

static Vec4 vertices[] = {
    {.x = 0.25, .y = 0.25, .z = 0.25, .w = 1},
    {.x = 0.25, .y = -0.25, .z = 0.25, .w = 1},
    {.x = -0.25, .y = -0.25, .z = 0.25, .w = 1},
    {.x = -0.25, .y = 0.25, .z = 0.25, .w = 1},

    {.x = 0.25, .y = 0.25, .z = -0.25, .w = 1},
    {.x = 0.25, .y = -0.25, .z = -0.25, .w = 1},
    {.x = -0.25, .y = -0.25, .z = -0.25, .w = 1},
    {.x = -0.25, .y = 0.25, .z = -0.25, .w = 1},
};

static int faces[][4] = {
    {0, 1, 2, 3},
    {4, 5, 6, 7},
    {0, 4, -1, -1},
    {1, 5, -1, -1},
    {2, 6, -1, -1},
    {3, 7, -1, -1}
};

void Engine_render(void)
{
    BeginDrawing();
    ClearBackground(BLACK);

    angle += PI * GetFrameTime();

    // TODO: Use a single matrix for rotating and translating.
    for (size_t i = 0; i < sizeof(vertices) / sizeof(Vec4); i++) {
        Vec4 rot = Vec4_rotateXZ(vertices[i], angle);
        Vec4 trans = Vec4_translate_z(rot, 1);
        Vec2 proj = Vec4_project(trans);
        Vec2 screen = Vec2_screen(proj);

        // DrawRectangle(screen.x, screen.y, 10, 10, GREEN);
        DrawPixel(screen.x, screen.y, GREEN);
    }

    size_t faces_len = sizeof(faces) / sizeof(faces[0]);
    size_t i_face_len = sizeof(faces[0]) / sizeof(int);

    for (size_t i = 0; i < faces_len; i++) {
        for (size_t j = 0; j < i_face_len; j++) {
            if (faces[i][j] == -1) {
                break;
            }

            int next = faces[i][(j + 1) % i_face_len];

            if (next == -1) {
                break;
            }

            Vec4 a = vertices[faces[i][j]];
            Vec4 b = vertices[next];

            Vec4 rot_a = Vec4_rotateXZ(a, angle);
            Vec4 trans_a = Vec4_translate_z(rot_a, 1);
            Vec2 proj_a = Vec4_project(trans_a);
            Vec2 screen_a = Vec2_screen(proj_a);

            Vec4 rot_b = Vec4_rotateXZ(b, angle);
            Vec4 trans_b = Vec4_translate_z(rot_b, 1);
            Vec2 proj_b = Vec4_project(trans_b);
            Vec2 screen_b = Vec2_screen(proj_b);

            DrawLine(screen_a.x, screen_a.y, screen_b.x, screen_b.y, GREEN);
        }
    }

    EndDrawing();
}

