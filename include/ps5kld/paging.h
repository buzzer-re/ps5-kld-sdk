#pragma once

#include <ps5kld/dmap.h>


int bind_page_pte(uint64_t vaddr1, uint64_t vaddr2, uint64_t pml);
