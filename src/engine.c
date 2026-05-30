#include "engine.h"
#include "config.h"
#include "mesh.h"
#include "raylib.h"
#include "vec.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

static float angle = 0.0f;
static MyMesh *cat_mesh = NULL;
static Mat4 tz_mat;
static Mat4 ty_mat;

static void display_FPS(void)
{
    static char buf[32];
    snprintf(buf, sizeof(buf), "FPS: %d", GetFPS());
    DrawText(buf, 0, 0, 1, GREEN);
}

static void draw_cat(void)
{
    angle += PI / 4 * GetFrameTime();

    Mat4 rot_mat = Mat4_rotateXZ(angle);
    Mat4 trans_mat = Mat4_multiply(&tz_mat, &ty_mat);
    Mat4 mat = Mat4_multiply(&rot_mat, &trans_mat);

    for (size_t i = 0; i < cat_mesh->f_count; ++i) {
        Face f = cat_mesh->fs[i];

        for (size_t j = 0; j < f.count; ++j) {
            uint32_t a_idx = cat_mesh->v_idx_buf[f.start + j] - 1;
            uint32_t b_idx = cat_mesh->v_idx_buf[f.start + (j + 1) % f.count] - 1;

            if (a_idx >= cat_mesh->v_count || b_idx >= cat_mesh->v_count) {
                printf("bad index: a=%u b=%u v_count=%zu\n", a_idx, b_idx, cat_mesh->v_count);
                continue;
            }

            Vec4 trans = Vec4_transform(cat_mesh->vs[a_idx], &mat);
            Vec2 proj = Vec4_to_ndc(trans);
            Vec2 screen_a = Vec2_screen(proj);

            trans = Vec4_transform(cat_mesh->vs[b_idx], &mat);
            proj = Vec4_to_ndc(trans);
            Vec2 screen_b = Vec2_screen(proj);

            DrawLine((int)screen_a.x, (int)screen_a.y, (int)screen_b.x, (int)screen_b.y, GREEN);
        }
    }
}

void Engine_setup(void)
{
    SetTargetFPS(TARGET_FPS);

    tz_mat = Mat4_translate_z(700);
    ty_mat = Mat4_translate_y(-200);

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

