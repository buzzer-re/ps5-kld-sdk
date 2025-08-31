#pragma once

#define kernel_base_addr 0xFFFFFFFF80210000 

#define OFFSET(x) x - kernel_base_addr

#define Xfast_syscall_403 0x294218
#define kprintf_offset_403 0x28da78
#define apic_ops_offset_403 OFFSET(0xFFFFFFFF81B44AC8)
#define kernel_pmap_offset_403 OFFSET(0xFFFFFFFF84067A78)
