#include "engine.h"
#include "raylib.h"
#include "vec.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static float angle = 0.0;

// clang-format off
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
// clang-format on

static char *make_string(void)
{
    size_t len = snprintf(NULL, 0, "FPS: %d", GetFPS()) + 1;
    char *buf = malloc(len);

    if (!buf) {
        return NULL;
    }

    snprintf(buf, len, "FPS: %d", GetFPS());
    return buf;
}

static void display_FPS(void)
{
    DrawText(make_string(), 0, 0, 1, GREEN);
}

static void draw_cube(void)
{
    angle += PI / 2 * GetFrameTime();

    Mat4 rot_mat = Mat4_rotateXZ(angle);
    Mat4 trans_mat = Mat4_translate_z(1);
    Mat4 mat = Mat4_multiply(&rot_mat, &trans_mat);

    for (size_t i = 0; i < sizeof(vertices) / sizeof(Vec4); ++i) {
        Vec4 trans = Vec4_transform(vertices[i], &mat);
        Vec2 proj = Vec4_project(trans);
        Vec2 screen = Vec2_screen(proj);

        DrawPixel(screen.x, screen.y, GREEN);
    }

    size_t faces_len = sizeof(faces) / sizeof(faces[0]);
    size_t i_face_len = sizeof(faces[0]) / sizeof(int);

    for (size_t i = 0; i < faces_len; ++i) {
        for (size_t j = 0; j < i_face_len; ++j) {
            if (faces[i][j] == -1) {
                break;
            }

            int next = faces[i][(j + 1) % i_face_len];

            if (next == -1) {
                break;
            }

            Vec4 a = vertices[faces[i][j]];
            Vec4 b = vertices[next];

            Vec4 trans = Vec4_transform(a, &mat);
            Vec2 proj = Vec4_project(trans);
            Vec2 screen_a = Vec2_screen(proj);

            trans = Vec4_transform(b, &mat);
            proj = Vec4_project(trans);
            Vec2 screen_b = Vec2_screen(proj);

            DrawLine(screen_a.x, screen_a.y, screen_b.x, screen_b.y, GREEN);
        }
    }
}

void Engine_render(void)
{
    BeginDrawing();
    ClearBackground(BLACK);
    display_FPS();
    draw_cube();
    EndDrawing();
}

