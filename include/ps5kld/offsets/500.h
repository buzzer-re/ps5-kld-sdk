#pragma once

#define kernel_base_addr 0xFFFFFFFF80210000 
#define Xfast_syscall 0x29d310

#define OFFSET(x) x - kernel_base_addr

#define kprintf_offset OFFSET(0xFFFFFFFF804A74B0)
#define kernel_pmap_offset OFFSET(0xFFFFFFFF841E8A88)
