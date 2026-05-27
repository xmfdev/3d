#include "utils.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

char *Utils_make_fps_str(void)
{
    int len = snprintf(NULL, 0, "FPS: %d", GetFPS()) + 1;
    char *buf = malloc(len);

    if (!buf) {
        return NULL;
    }

    snprintf(buf, len, "FPS: %d", GetFPS());
    return buf;
}

