#ifndef MESH_H
#define MESH_H

#include "vec.h"

typedef struct {
    Vec4 v[2000];
    int count;
} My_Mesh;

My_Mesh Mesh_read_model(void);

#endif
