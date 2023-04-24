
#include "common.h"
#include <nuttx/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <tflmrt/runtime.h>
#include <unistd.h>

#define TF_ARENA_SIZE (1000000) // 1M

int main(int argc, FAR char *argv[])
{
    int ret = EXIT_SUCCESS;
    void *model = NULL;
    const char *model_path = argv[1];
    tflm_config_t tf_config = {.cpu_num = 1};
    tflm_runtime_t tf_runtime;

    if (argc != 2)
    {
        fprintf(stderr, "Argument expected\n");
        ret = EXIT_FAILURE;
        goto _return;
    }

    errno = 0;
    if (!(model = load_file_into_memory(model_path)))
    {
        fprintf(stderr, "Failed to load model from %s into memory: %s", model_path, strerror(errno));
        ret = errno;
        goto _free;
    }
    printf("Loaded model into memory\n");

    if ((ret = tflm_initialize(&tf_config)))
    {
        fprintf(stderr, "tflm_initialize() failed: %d\n", ret);
        goto _tflm_finalize;
    }
    printf("Initialized Tensorflow lite micro\n");

    if ((ret = tflm_runtime_initialize(&tf_runtime, model, TF_ARENA_SIZE)))
    {
        fprintf(stderr, "Failed to initialize runtime: %d\n", ret);
        goto _tflm_runtime_finalize;
    }
    printf("Tensorflow arena size: %d\n", tflm_runtime_actual_arenasize(&tf_runtime));

_tflm_runtime_finalize:
    tflm_runtime_finalize(&tf_runtime);
_tflm_finalize:
    tflm_finalize();
_free:
    free(model);
_return:
    return ret;
}
