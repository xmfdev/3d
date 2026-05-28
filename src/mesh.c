#include "mesh.h"
#include "config.h"
#include "vec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OBJ_L_START_IDX 2
#define OBJ_L_SIZE 256

static int str_to_v_coor(char *str, float *out)
{
    char *end;
    *out = strtof(str, &end);
    return end != str ? OPERATION_SUCCESS : OPERATION_ERROR;
}

static int set_coor_to_val(char *buf, float *x, float *y, float *z, float *w, int n_spaces)
{
    float coor = 0.0f;
    int i_res = str_to_v_coor(buf, &coor);

    if (i_res == OPERATION_ERROR) {
        return OPERATION_ERROR;
    }

    switch (n_spaces) {
    case 0:
        *x = coor;
        break;
    case 1:
        *y = coor;
        break;
    case 2:
        *z = coor;
        break;
    case 3:
        *w = coor;
        break;
    default:
        return OPERATION_ERROR;
    }

    return OPERATION_SUCCESS;
}

static Vec4 load_vertex(char *line)
{
    size_t line_blen = strlen(line) + 1;

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 1.0f;

    char buf[line_blen];
    memset(buf, 0, line_blen);

    size_t buf_filled = 0;
    size_t n_spaces = 0;

    for (int i = OBJ_L_START_IDX; i < line_blen - 1; ++i) {
        if (line[i] != ' ' && line[i] != '\n') {
            buf[buf_filled++] = line[i];
            continue;
        }

        if (set_coor_to_val(buf, &x, &y, &z, &w, n_spaces) == OPERATION_ERROR) {
            return (Vec4){0};
        }

        memset(buf, 0, line_blen);
        buf_filled = 0;
        n_spaces++;
    }

    // Edge-case: Last line of file may not end in a '\n'.
    if (buf_filled > 0) {
        if (set_coor_to_val(buf, &x, &y, &z, &w, n_spaces) == OPERATION_ERROR) {
            return (Vec4){0};
        }
    }

    return (Vec4){
        .x = x,
        .y = y,
        .z = z,
        .w = w,
    };
}

static size_t num_vertices(FILE *f)
{
    char line[OBJ_L_SIZE] = {0};
    size_t count = 0;

    while (fgets(line, sizeof(line), f)) {
        if (line[0] == 'v' && line[1] == ' ') {
            ++count;
        }
    }
    return count;
}

static MyMesh *make(void)
{
    MyMesh *mesh = malloc(sizeof(MyMesh));

    if (mesh == NULL) {
        return NULL;
    }

    mesh->vs = NULL;
    mesh->v_count = 0;

    return mesh;
}

MyMesh *Mesh_load_from_file(char *path)
{
    FILE *f = fopen(path, "r");

    if (f == NULL) {
        return NULL;
    }

    MyMesh *mesh = make();

    if (mesh == NULL) {
        fclose(f);
        return NULL;
    }

    mesh->v_count = num_vertices(f);
    mesh->vs = malloc(sizeof(Vec4) * mesh->v_count);

    if (mesh->vs == NULL) {
        fclose(f);
        free(mesh);
        return NULL;
    }

    char line[OBJ_L_SIZE] = {0};
    size_t idx = 0;

    rewind(f);
    while (fgets(line, sizeof(line), f)) {
        if (line[0] != 'v' || line[1] != ' ') {
            continue;
        }

        mesh->vs[idx++] = load_vertex(line);
    }

    fclose(f);
    return mesh;
}

