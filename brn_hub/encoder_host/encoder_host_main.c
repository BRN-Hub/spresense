
#include "common.h"
#include <malloc.h>
#include <nuttx/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <tflmrt/runtime.h>
#include <unistd.h>

#define TF_ARENA_SIZE (1500000) // 100k

int main(int argc, FAR char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Argument expected\n");
        return EXIT_FAILURE;
    }

    int ret = EXIT_SUCCESS;
    void *model = NULL;
    const char *model_path = argv[1];
    tflm_config_t tf_config = {.cpu_num = 1};
    tflm_runtime_t tf_runtime;

    errno = 0;
    if (!(model = load_file_into_memory(model_path)))
    {
        fprintf(stderr, "Failed to load model from %s into memory: %s", model_path, strerror(errno));
        ret = errno;
        goto _return;
    }
    size_t model_size_memory = malloc_usable_size(model);
    printf("Loaded model into memory, %d bytes\n", model_size_memory);

    if ((ret = tflm_initialize(&tf_config)))
    {
        fprintf(stderr, "tflm_initialize() failed: %d\n", ret);
        goto _free;
    }
    printf("Initialized Tensorflow lite micro\n");

    if ((ret = tflm_runtime_initialize(&tf_runtime, model, TF_ARENA_SIZE)))
    {
        fprintf(stderr, "Failed to initialize runtime: %d\n", ret);
        goto _tflm_finalize;
    }
    printf("foo\n");
    printf("Tensorflow arena size: %d\n", tflm_runtime_actual_arenasize(&tf_runtime));

    tflm_runtime_finalize(&tf_runtime);
_tflm_finalize:
    tflm_finalize();
_free:
    free(model);
_return:
    return ret;
}
