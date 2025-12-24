# PlayStation 5 Kernel SDK 


This an Kernel SDK is designed to support research on PlayStation 5 kernels when using loaders such as [kldload](https://github.com/buzzer-re/PS5_kldload). 

I've wrote this SDK to eliminate the need to rewrite the same kernel initialization code each time I started a new project or simple tests. It also encapsulates useful functions and includes its own CRT for more complex initializations when needed.

It currently includes offsets for functions and kernel variables for the following firmware versions:

- 4.03
- 5.0

More firmware versions can be added. The [ps5-kldload](https://github.com/buzzer-re/PS5_kldload/tree/dev) supports firmware up to version 6.50.


## Installing

First, install the dependencies (assuming Ubuntu).

```
$ sudo apt install build-essential git # gcc, make and etc
```

Clone the repository and install it

```
$ git clone https://github.com/buzzer-re/ps5-kld-sdk.git
$ cd ps5-kld-sdk
$ ./install.sh
```

The installation builds the files and moves them to `/opt/ps5-kld-sdk`.

## Example

#### CR and IDT manipulation
Here's an example using the SDK to read some registers values

```c
#include <ps5kld/kernel.h>
#include <ps5kld/intrin.h>
#include <ps5kld/dmap.h>
#include <ps5kld/machine/idt.h>

int module_start(kproc_args* args)
{
    kprintf("Hello from kernel SDK, running on fw: %x\n", args->fwver);
    kprintf("Kernel base: %#02lx\n", args->kdata_base);

    uint8_t idt[10];
    __sidt(idt);

    IDTR* idtr = (IDTR*) idt;

    kprintf("CR3: %x\nCR0: %x\nIDT base: %#02lx\n",
        __readcr3(),
        __readcr0(),
        idtr->base
    );


    return 0;
}
```

#### Physical memory playground

You can easily play around with the physical memory with already created wrappers:

```c
#include <ps5kld/kernel.h>
#include <ps5kld/dmap.h>
#include <ps5kld/intrin.h>

int module_start(void* kproc_args)
{
    uint64_t x = 10;
    kprintf("vaddr: %#02lx\nvalue: %d\n", &x, x);

    uint64_t phys =  virt2phys(__readcr3(), &x); // or any other pml4 entry value (like usermode processes)
    kprintf("paddr: %#02lx\n", phys);
    
    // write using the dmap mem space
    uint64_t* paddr_dmap =  PHYSTODMAP(phys);
    *paddr_dmap = 30;

    kprintf("new value: %d\n", x);

    return 0;
}
```

## Using

Check the `sample` folder. To create a new project, you can copy the sample from the SDK installation directory to a new location.

```
$ mkdir myproj
$ cp /opt/ps5-kld-sdk/sample/hello_world/* myproj/
```

The payloads are built with the `.bin` extension. You can send them to any kernel module loader, such as `kldload`, or embed them in your own loader.

Since the SDK includes freebsd-headers, you can use FreeBSD definitions in your project when needed, such as `<sys/cpuset.h>`. Depending on your IDE, you may want to add the following path for code completion:

- /opt/ps5-kld-sdk/include
- /oot/ps5-kld-sdk/include/freebsd-headers

## Porting

To add new offsets and function definitions, check the `include/ps5kld/offsets` directory and update the entries in `src/kernel.c`, specifically within the init_kernel function.