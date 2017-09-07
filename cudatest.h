//
// Created by martin on 20/08/17.
//

#ifndef CALENHAD_CUDATEST_H
#define CALENHAD_CUDATEST_H

#include <driver_types.h>

static void CheckCudaErrorAux (const char *, unsigned, const char *, cudaError_t);
float *gpuReciprocal (unsigned size);

#endif //CALENHAD_CUDATEST_H
