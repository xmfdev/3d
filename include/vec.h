#ifndef VEC_H
#define VEC_H

typedef struct {
    float x, y;
} Vec2;

typedef struct {
    float x, y, z, w;
} Vec4;

// Column-major order, [col][row].
typedef struct {
    float m[4][4];
} Mat4;

// According to the System V AMD64 ABI:
//
// Structs greater than 16 bytes are passed on the stack (not registers).
//
// `Mat4` is 64 bytes, so it's passed by pointer to avoid copying.
// `Vec4` is 16 bytes and `Vec2`/`Vec3` are under, so they're passed by value (registers).
//
// TL;DR: For small structs, there's no need for indirection as it is less efficient.

Mat4 Mat4_make_zero();
Mat4 Mat4_make_id();
Mat4 Mat4_multiply(Mat4 *a, Mat4 *b);
Mat4 Mat4_rotateXZ(float rad);
Mat4 Mat4_translate_y(float delta);
Mat4 Mat4_translate_z(float delta);

Vec4 Vec4_make(float x, float y, float z);
Vec4 Vec4_transform(Vec4 v, const Mat4 *m);
Vec2 Vec4_to_ndc(Vec4 v);

Vec2 Vec2_screen(Vec2 v);

#endif

