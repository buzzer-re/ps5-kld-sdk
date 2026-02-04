# Detect OS and set LLVM paths accordingly
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    # macOS with Homebrew LLVM 18
    LLVM_PREFIX := /opt/homebrew/opt/llvm@18/bin
else
    # Linux with LLVM 18 from apt repository
    LLVM_PREFIX := /usr/lib/llvm-18/bin
endif

CC      := $(LLVM_PREFIX)/clang
OBJCOPY := $(LLVM_PREFIX)/llvm-objcopy
ODIR    := build
SDIR    := src

MAPFILE := $(shell basename "$(CURDIR)").map
CFLAGS  := --target=x86_64-unknown-none-elf \
           -I./freebsd-headers/ \
           -I./include/ \
           $(IDIRS) \
           -Os -std=c11 \
           -ffunction-sections -fdata-sections \
           -fno-builtin -nostdlib -ffreestanding \
           -Wall -Wextra \
           -march=btver2 -mtune=btver2 \
           -m64 -mcmodel=small \
           -fPIC

CFILES  := $(wildcard $(SDIR)/*.c)
SFILES  := $(wildcard $(SDIR)/*.s)
OBJS    := $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(CFILES)) $(patsubst $(SDIR)/%.s, $(ODIR)/%.o, $(SFILES))

all: $(ODIR) $(OBJS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.s
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR):
	@mkdir -p build/

.PHONY: clean

clean:
	rm -rf "$(TARGET)" "$(MAPFILE)" $(ODIR)
