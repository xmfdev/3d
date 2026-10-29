#include "mesh.h"
#include "config.h"
#include "vec.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OBJ_L_START_IDX 2
#define OBJ_L_SIZE 256

static int str_to_int(char *str, uint32_t *out)
{
    char *end;
    *out = (uint32_t)strtoul(str, &end, 10);
    return end != str ? OPERATION_SUCCESS : OPERATION_ERROR;
}

static int str_to_f(char *str, float *out)
{
    char *end;
    *out = strtof(str, &end);
    return end != str ? OPERATION_SUCCESS : OPERATION_ERROR;
}

static int set_coor_to_val(char *buf, float *x, float *y, float *z, float *w, uint64_t n_spaces)
{
    float coor = 0.0f;
    int i_res = str_to_f(buf, &coor);

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

    for (size_t i = OBJ_L_START_IDX; i < line_blen - 1; ++i) {
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

static size_t get_v_idx_count(char *line)
{
    size_t num_spaces = 0;
    size_t idx = 2;

    while (1) {
        if (line[idx] == ' ') {
            ++num_spaces;
        } else if (line[idx] == '\n' || line[idx] == '\0') {
            break;
        }

        ++idx;
    }

    return num_spaces;
}

static int find_next_ch(size_t cur_idx, char target, char *line)
{
    for (size_t i = cur_idx + 1; (int)i < (int)strlen(line); ++i) {
        if (line[i] == target) {
            return (int)i;
        }
    }

    return -1;
}

static int store_v_idx(char *buf, size_t len, size_t *buf_filled, MyMesh *mesh)
{
    uint32_t conversion;
    int res = str_to_int(buf, &conversion);
    if (res == -1) {
        return -1;
    }
    memset(buf, '\0', len);
    *buf_filled = 0;
    mesh->v_idx_buf[mesh->v_idx_count++] = conversion;
    return 0;
}

static int load_v_indices(char *line, MyMesh *mesh)
{
    size_t line_blen = strlen(line) + 1;
    char buf[30] = {'\0'};
    size_t buf_filled = 0;
    int loaded_total = 0;

    for (size_t i = OBJ_L_START_IDX; i < line_blen; ++i) {
        if (line[i] == '\n') {
            if (buf_filled > 0) {

            store_v_idx(buf, sizeof(buf), &buf_filled, mesh);
            ++loaded_total;
            }
            break;
        } else if (line[i] == '/') {
            int nidx = find_next_ch(i, ' ', line);

            if (nidx != -1) {
                i = (size_t)nidx;
                store_v_idx(buf, sizeof(buf), &buf_filled, mesh);
                ++loaded_total;
                continue;
            }

            store_v_idx(buf, sizeof(buf), &buf_filled, mesh);
            ++loaded_total;
            break;
        } else if (line[i] == ' ') {
            if (buf_filled > 0) {
                store_v_idx(buf, sizeof(buf), &buf_filled, mesh);
                ++loaded_total;
            }
            continue;
        }

        buf[buf_filled++] = line[i];
    }

    return loaded_total;
}

static void set_mesh_data(MyMesh *mesh, FILE *f)
{
    char line[OBJ_L_SIZE] = {0};

    size_t v_count = 0;
    size_t f_count = 0;
    size_t v_idx_count = 0;

    while (fgets(line, sizeof(line), f)) {
        if (line[0] == 'v' && line[1] == ' ') {
            ++v_count;
        } else if (line[0] == 'f' && line[1] == ' ') {
            ++f_count;
            v_idx_count += get_v_idx_count(line);
        }
    }

    mesh->v_count = v_count;
    mesh->f_count = f_count;
    mesh->v_idx_count = v_idx_count;

    printf("v_count: %zu\n", mesh->v_count);
    printf("f_count: %zu\n", mesh->f_count);
    printf("v_idx_count: %zu\n", mesh->v_idx_count);
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

    set_mesh_data(mesh, f);
    mesh->vs = malloc(sizeof(Vec4) * mesh->v_count);
    mesh->fs = malloc(sizeof(Face) * mesh->f_count);
    mesh->v_idx_buf = malloc(sizeof(uint32_t) * mesh->v_idx_count);

    if (mesh->vs == NULL || mesh->fs == NULL || mesh->v_idx_buf == NULL) {
        fclose(f);
        free(mesh->vs);
        free(mesh->fs);
        free(mesh->v_idx_buf);
        free(mesh);
        return NULL;
    }

    char line[OBJ_L_SIZE] = {0};
    size_t idx = 0;
    int loaded_indices = 0;
    int loaded_indices_total = 0;
    int loaded_faces = 0;

    mesh->v_idx_count = 0;
    rewind(f);
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == 'v' && line[1] == ' ') {
            mesh->vs[idx++] = load_vertex(line);
            continue;
        }

        if (line[0] == 'f' && line[1] == ' ') {
            loaded_indices = load_v_indices(line, mesh);
            Face face = {.start = (uint32_t)loaded_indices_total,
                         .count = (uint32_t)loaded_indices};
            // printf("loaded face (start = %d, count = %d)\n", loaded_indices_total, loaded_indices);
            loaded_indices_total += loaded_indices;
            mesh->fs[loaded_faces++] = face;
            continue;
        }

        continue;
    }

    fclose(f);
    return mesh;
}

