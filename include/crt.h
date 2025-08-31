#pragma once

#include <ps5kld/kernel.h>

typedef struct __kproc_args
{
    uint64_t kdata_base;
    uint32_t fwver;
} kproc_args;

void start(kproc_args* args);
extern int module_start(kproc_args* args);