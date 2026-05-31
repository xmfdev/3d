#include "engine.h"
#include "config.h"
#include "mesh.h"
#include "raylib.h"
#include "vec.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define NEAR_CLIP_Z (float)10

static float angle = 0.0f;
static MyMesh *cat_mesh = NULL;
static Mat4 trans_mat;
static Vec4 *transformed;

static void display_FPS(void)
{
    static char buf[32];
    snprintf(buf, sizeof(buf), "FPS: %d", GetFPS());
    DrawText(buf, 0, 0, 1, GREEN);
}

static Vec4 near_clip_lerp(Vec4 *a, Vec4 *b)
{
    float t = (NEAR_CLIP_Z - a->z) / (b->z - a->z);

    return (Vec4){
        .x = a->x + t * (b->x - a->x),
        .y = a->y + t * (b->y - a->y),
        .z = NEAR_CLIP_Z,
        .w = 1.0f,
    };
}

static void draw_cat(void)
{
    angle += PI / 16 * GetFrameTime();

    Mat4 rot_mat = Mat4_rotateXZ(angle);
    Mat4 mat = Mat4_multiply(&rot_mat, &trans_mat);

    Vec2 proj;
    Vec2 screen_a;
    Vec2 screen_b;

    for (size_t i = 0; i < cat_mesh->v_count; ++i) {
        transformed[i] = Vec4_transform(cat_mesh->vs[i], &mat);
    }

    for (size_t i = 0; i < cat_mesh->f_count; ++i) {
        Face f = cat_mesh->fs[i];

        for (size_t j = 0; j < f.count; ++j) {
            uint32_t a_idx = cat_mesh->v_idx_buf[f.start + j] - 1;
            uint32_t b_idx = cat_mesh->v_idx_buf[f.start + (j + 1) % f.count] - 1;

            bool a_behind = transformed[a_idx].z < NEAR_CLIP_Z;
            bool b_behind = transformed[b_idx].z < NEAR_CLIP_Z;

            if (a_behind && b_behind) {
                continue;
            }

            if (a_behind || b_behind) {
                if (a_behind) {
                    proj = Vec4_to_ndc(near_clip_lerp(&transformed[a_idx], &transformed[b_idx]));
                    screen_a = Vec2_screen(proj);
                    proj = Vec4_to_ndc(transformed[b_idx]);
                    screen_b = Vec2_screen(proj);

                    DrawLine((int)screen_a.x, (int)screen_a.y, (int)screen_b.x, (int)screen_b.y,
                             GREEN);
                    continue;
                }

                proj = Vec4_to_ndc(near_clip_lerp(&transformed[a_idx], &transformed[b_idx]));
                screen_a = Vec2_screen(proj);
                proj = Vec4_to_ndc(transformed[a_idx]);
                screen_b = Vec2_screen(proj);

                DrawLine((int)screen_a.x, (int)screen_a.y, (int)screen_b.x, (int)screen_b.y, GREEN);
                continue;
            }

            proj = Vec4_to_ndc(transformed[a_idx]);
            screen_a = Vec2_screen(proj);
            proj = Vec4_to_ndc(transformed[b_idx]);
            screen_b = Vec2_screen(proj);

            DrawLine((int)screen_a.x, (int)screen_a.y, (int)screen_b.x, (int)screen_b.y, GREEN);
        }
    }
}

void Engine_setup(void)
{
    SetTargetFPS(TARGET_FPS);

    cat_mesh = Mesh_load_from_file("assets/cat.obj");
    transformed = malloc(sizeof(Vec4) * cat_mesh->v_count);

    // Mat4 tz_mat = Mat4_translate_z(700);
    Mat4 tz_mat = Mat4_translate_z(200);
    Mat4 ty_mat = Mat4_translate_y(-200);
    trans_mat = Mat4_multiply(&tz_mat, &ty_mat);

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

