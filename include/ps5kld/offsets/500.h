#pragma once

#define kernel_base_addr 0xFFFFFFFF80210000 

#define OFFSET(x) x - kernel_base_addr

#define Xfast_syscall_500 0x29d310

#define kprintf_offset_500 OFFSET(0xFFFFFFFF804A74B0)
#define apic_ops_offset_500 OFFSET(0xFFFFFFFF81C74130)
#define cpu_apic_ids_offset_500 OFFSET(0xFFFFFFFF83D338C0)

#define kernel_pmap_offset_500 OFFSET(0xFFFFFFFF841E8A88)
#define malloc_offset_500 OFFSET(0xFFFFFFFF80D9B210)
#define malloc_MTEMP_offset_500 OFFSET(0xFFFFFFFF822855A0)

