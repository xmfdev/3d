#ifndef MESH_H
#define MESH_H

#include "vec.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint32_t start;
    size_t count;
} Face;

typedef struct {
    Vec4 *vs;
    Face *fs;
    uint32_t *v_idx_buf;

    size_t v_count;
    size_t f_count;
    size_t v_idx_count;
} MyMesh;

MyMesh *Mesh_load_from_file(char *path);

#endif
