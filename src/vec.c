#include "vec.h"
#include "config.h"
#include <math.h>
#include <stdlib.h>

// Returns a zero matrix.
Mat4 Mat4_make_zero()
{
    return (Mat4){
        .m = {{0}, {0}, {0}, {0}},
    };
}

// Returns an identity matrix.
Mat4 Mat4_make_id()
{
    return (Mat4){
        .m = {{1}, {0, 1}, {0, 0, 1}, {0, 0, 0, 1}},
    };
}

// Returns two multiplied matrices.
//
// Note: `a` is the rightmost matrix.
Mat4 Mat4_multiply(Mat4 *a, Mat4 *b)
{
    Mat4 result = Mat4_make_zero();

    size_t n_cols = sizeof(a->m) / sizeof(a->m[0]);
    size_t n_rows = sizeof(a->m[0]) / sizeof(a->m[0][0]);

    for (size_t i = 0; i < n_cols; ++i) {
        for (size_t j = 0; j < n_rows; ++j) {
            for (size_t k = 0; k < n_cols; ++k) {
                result.m[i][j] += a->m[i][k] * b->m[k][j];
            }
        }
    }

    return result;
}

Vec4 Vec4_make(float x, float y, float z)
{
    return (Vec4){
        .x = x,
        .y = y,
        .z = z,
        .w = 1,
    };
}

Vec4 Vec4_transform(Vec4 v, const Mat4 *m)
{
    return (Vec4){
        .x = v.x * m->m[0][0] + v.y * m->m[1][0] + v.z * m->m[2][0] + v.w * m->m[3][0],
        .y = v.x * m->m[0][1] + v.y * m->m[1][1] + v.z * m->m[2][1] + v.w * m->m[3][1],
        .z = v.x * m->m[0][2] + v.y * m->m[1][2] + v.z * m->m[2][2] + v.w * m->m[3][2],
        .w = v.x * m->m[0][3] + v.y * m->m[1][3] + v.z * m->m[2][3] + v.w * m->m[3][3],
    };
}

// Returns a matrix that when multiplied by a vector, rotates it around the Y axis.
Mat4 Mat4_rotateXZ(float rad)
{
    Mat4 mat = Mat4_make_id();

    mat.m[0][0] = cosf(rad);
    mat.m[0][2] = sinf(rad);
    mat.m[2][0] = -sinf(rad);
    mat.m[2][2] = cosf(rad);

    return mat;
}

// Returns a matrix that when multiplied by a vector, translates it on the Y axis.
Mat4 Mat4_translate_y(float delta)
{
    Mat4 mat = Mat4_make_id();
    mat.m[3][1] = delta;
    return mat;
}

// Returns a matrix that when multiplied by a vector, translates it on the Z axis.
Mat4 Mat4_translate_z(float delta)
{
    Mat4 mat = Mat4_make_id();
    mat.m[3][2] = delta;
    return mat;
}

// Projects from 3D to 2D via perspective divide, returning NDC coordinates in [-1, 1].
// TODO: This should be a matrix.
// TODO: Aspect ratio should be moved to a separate function.
Vec2 Vec4_to_ndc(Vec4 v)
{
    return (Vec2){
        .x = v.x / v.z / ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT),
        .y = v.y / v.z / ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT),
    };
}

// Converts NDC coordinates [-1, 1] to screen pixel coordinates.
// See math notes for more info on how to derive the formula.
Vec2 Vec2_screen(Vec2 v)
{
    return (Vec2){
        .x = (SCREEN_WIDTH * v.x + SCREEN_WIDTH) / 2,
        .y = (SCREEN_HEIGHT - SCREEN_HEIGHT * v.y) / 2,
    };
}

