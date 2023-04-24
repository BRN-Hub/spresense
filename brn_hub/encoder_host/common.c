#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

void *load_file_into_memory(const char *path)
{
    void *buf = NULL;

    if (NULL == path)
    {
        goto _return;
    }

    FILE *f = NULL;

    if (!(f = fopen(path, "r")))
    {
        goto _return;
    }

    fseek(f, 0L, SEEK_END);
    ulong f_size = ftell(f);
    fseek(f, 0L, SEEK_SET);

    buf = malloc(f_size);
    if (NULL == buf)
    {
        goto _fclose;
    }

    ulong n_written = fread(buf, 1, f_size, f);
    if (n_written != f_size)
    {
        free(buf);
        buf = NULL;
    }

_fclose:
    fclose(f);
_return:
    return buf;
}
