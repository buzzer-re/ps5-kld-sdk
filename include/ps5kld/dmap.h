#pragma once

#include <ps5kld/kernel.h>
#define PHYSTODMAP(phys_addr) (get_dmap() + phys_addr)

uint64_t get_dmap();
