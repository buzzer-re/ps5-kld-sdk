LLVM_PREFIX := /opt/homebrew/opt/llvm@18/bin
CC      := $(LLVM_PREFIX)/clang
LD      := $(LLVM_PREFIX)/ld.lld
OBJCOPY := $(LLVM_PREFIX)/llvm-objcopy
ODIR    := build
SDIR    := src

MAPFILE := $(shell basename "$(CURDIR)").map
CFLAGS  := --target=x86_64-unknown-none-elf \
           -I/opt/ps5-kld-sdk/include/ \
           -I/opt/ps5-kld-sdk/include/freebsd-headers/ \
           -I./include/ \
           $(IDIRS) \
           -Os -std=c11 \
           -ffunction-sections -fdata-sections \
           -fno-builtin -nostdlib -ffreestanding \
           -Wall -Wextra \
           -march=btver2 -mtune=btver2 \
           -m64 -mcmodel=small \
           -fPIC

LFLAGS  := -T /opt/ps5-kld-sdk/ld/linker.x \
           -Map=$(MAPFILE) \
           --gc-sections \
           --build-id=none

CFILES  := $(wildcard $(SDIR)/*.c)
SFILES  := $(wildcard $(SDIR)/*.s)
OBJS    := $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(CFILES)) $(patsubst $(SDIR)/%.s, $(ODIR)/%.o, $(SFILES))
LIBS    := /opt/ps5-kld-sdk/lib/*.o
TARGET  := $(shell basename "$(CURDIR)").bin
ELF     := $(shell basename "$(CURDIR)").elf

$(TARGET): $(ODIR) $(OBJS)
	$(LD) $(ODIR)/*.o $(LIBS) -o $(ELF) $(LFLAGS)
	$(OBJCOPY) -S -O binary $(ELF) "$(TARGET)"
	mv $(ELF) build/

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.s
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR):
	@mkdir -p build/

.PHONY: clean

clean:
	rm -rf "$(TARGET)" "$(MAPFILE)" $(ODIR)
