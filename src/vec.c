#include "vec.h"
#include "config.h"
#include <math.h>

Vec4 Vec4_make(float x, float y, float z)
{
    return (Vec4){
        .x = x,
        .y = y,
        .z = z,
        .w = 1,
    };
}

Vec4 Vec4_transform(Vec4 v, Mat4 *m)
{
    return (Vec4){
        .x = v.x * m->m[0][0] + v.y * m->m[1][0] + v.z * m->m[2][0] + v.w * m->m[3][0],
        .y = v.x * m->m[0][1] + v.y * m->m[1][1] + v.z * m->m[2][1] + v.w * m->m[3][1],
        .z = v.x * m->m[0][2] + v.y * m->m[1][2] + v.z * m->m[2][2] + v.w * m->m[3][2],
        .w = v.x * m->m[0][3] + v.y * m->m[1][3] + v.z * m->m[2][3] + v.w * m->m[3][3],
    };
}

Vec4 Vec4_rotateXZ(Vec4 v, float rad)
{
    return (Vec4){
        .x = v.x * cosf(rad) + v.z * -sinf(rad),
        .y = v.y,
        .z = v.x * sinf(rad) + v.z * cosf(rad),
        .w = v.w,
    };
}

Vec4 Vec4_translate_z(Vec4 v, float delta) {
    return (Vec4){
        .x = v.x,
        .y = v.y,
        .z = v.z + delta,
        .w = v.w,
    };
}

// Projects from 3D to 2D via perspective divide, returning NDC coordinates in [-1, 1].
// TODO: If `v.z` is 0 or negative, the point is on or behind the camera - do not call this.
Vec2 Vec4_project(Vec4 v)
{
    return (Vec2){
        .x = v.x / v.z,
        .y = v.y / v.z,
    };
}

// Converts NDC coordinates [-1, 1] to screen pixel coordinates.
Vec2 Vec2_screen(Vec2 v)
{
    return (Vec2){
        .x = (SCREEN_WIDTH * v.x + SCREEN_WIDTH) / 2,
        .y = (SCREEN_HEIGHT - SCREEN_HEIGHT * v.y) / 2,
    };
}

