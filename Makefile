
CC		:= gcc
OBJCOPY	:= objcopy
ODIR	:= build
SDIR	:= src

MAPFILE := $(shell basename "$(CURDIR)").map
CFLAGS += -DFIRMWARE=$(FW)
$(info $(CLFLAGS))
CFLAGS	:= $(IDIRS) -Os -std=c11 -ffunction-sections -fdata-sections -fno-builtin -nostartfiles -nostdlib -Wall -Wextra -march=btver2 -mtune=btver2 -m64 -mabi=sysv -mcmodel=small -fpie -fPIC -I ./freebsd-headers/ -I ./include/

LFLAGS	:= -Xlinker -T ./linker.x -Xlinker -Map="$(MAPFILE)" -O0 -Wl,--build-id=none -Wl,--gc-sections
CFILES	:= $(wildcard $(SDIR)/*.c)
SFIL	ES	:= $(wildcard $(SDIR)/*.s)
OBJS	:= $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(CFILES)) $(patsubst $(SDIR)/%.s, $(ODIR)/%.o, $(SFILES)) $(CFILES)

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
