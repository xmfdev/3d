#include "mesh.h"
#include "vec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

My_Mesh Mesh_read_model(void)
{
    char buf[100] = {0};
    FILE *foo = fopen("assets/cat.obj", "r");
    My_Mesh mesh = {0};

    if (foo == NULL) {
        abort();
    }

    int index_v = 0;
    while (1) {
        char *line = fgets(buf, sizeof(buf), foo);

        if (line == NULL) {
            abort();
        }

        if (line[0] != 'v') {
            continue;
        } else if (line[1] == 't') {
            break;
            ;
        }

        // Load vertices.
        // Mesh mesh = {0};

        char strs[4][15] = {0};

        int sp_found = 0;
        int c_found = 0;

        for (int i = 0; i < strlen(line); ++i) {
            if (line[i] == '\n') {
                break;
            } else if (line[i] == ' ') {
                sp_found++;
                c_found = 0;
                continue;
            }

            strs[sp_found][c_found] = line[i];
            c_found++;
        }

        Vec4 vec = Vec4_make(0, 0, 0);

        char *end;
        for (int i = 1; i < sizeof(strs) / sizeof(strs[0]); ++i) {
            float val = strtof(strs[i], &end);

            if (i == 1)
                vec.x = val;
            else if (i == 2)
                vec.y = val;
            else if (i == 3)
                vec.z = val;
        }

        // printf("(%f, %f, %f, %f)\n", vec.x, vec.y, vec.z, vec.w);
        mesh.v[index_v] = vec;

        // printf("\n");
        index_v++;
    }

    mesh.count = index_v;
    fclose(foo);

    return mesh;
}

