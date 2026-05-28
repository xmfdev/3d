#include "engine.h"
#include "config.h"
#include "mesh.h"
#include "raylib.h"
#include "vec.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static float angle = 0.0f;
static MyMesh *cat_mesh = NULL;

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

static void draw_cat(void)
{
    angle += PI / 4 * GetFrameTime();

    Mat4 rot_mat = Mat4_rotateXZ(angle);
    Mat4 tz_mat = Mat4_translate_z(700);
    Mat4 ty_mat = Mat4_translate_y(-200);
    Mat4 trans_mat = Mat4_multiply(&tz_mat, &ty_mat);
    Mat4 mat = Mat4_multiply(&rot_mat, &trans_mat);

    for (size_t i = 0; i < cat_mesh->v_count; ++i) {
        Vec4 trans = Vec4_transform(cat_mesh->vs[i], &mat);
        Vec2 proj = Vec4_to_ndc(trans);
        Vec2 screen = Vec2_screen(proj);

        DrawPixel(screen.x, screen.y, GREEN);
    }
}

void Engine_setup(void)
{
    SetTargetFPS(TARGET_FPS);

    cat_mesh = Mesh_load_from_file("assets/cat.obj");

    // TODO: Logger file.
    if (cat_mesh != NULL) {
        printf("loaded cat mesh successfully\n");
    } else {
        printf("failed to load cat mesh");
    }
}

// TODO: Limit FPS.
void Engine_render(void)
{
    BeginDrawing();
    ClearBackground(BLACK);
    display_FPS();
    // draw_cube();
    draw_cat();
    EndDrawing();
}

