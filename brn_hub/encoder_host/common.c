#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

void *load_file_into_memory(const char *path)
{
    if (NULL == path)
    {
        return NULL;
    }

    FILE *f = NULL;

    if (!(f = fopen(path, "r")))
    {
        return NULL;
    }

    fseek(f, 0L, SEEK_END);
    ulong f_size = ftell(f);
    fseek(f, 0L, SEEK_SET);

    void *buf = malloc(f_size);
    if (NULL == buf)
    {
        fclose(f);
        return NULL;
    }

    ulong n_written = fread(buf, 1, f_size, f);
    if (n_written != f_size)
    {
        free(buf);
        fclose(f);
        return NULL;
    }

    fclose(f);
    return buf;
}