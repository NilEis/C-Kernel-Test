CC = i686-elf-gcc.exe
AS = i686-elf-gcc.exe
LD = i686-elf-gcc.exe
AS_FLAGS = -std=gnu99 -ffreestanding -g -c
LD_FLAGS = -ffreestanding -nostdlib -g -Tsrc/linker.ld -lgcc
CC_FLAGS = -std=gnu99 -ffreestanding -g -c

SOURCE_C := $(wildcard src/*.c) $(wildcard src/*/*.c)
SOURCE_ASM := $(wildcard src/*.s) $(wildcard src/*/*.s)
OBJS := $(SOURCE_C:.c=.o) $(SOURCE_ASM:.s=.o)
KERNEL_BIN := bin/kernel.elf

VM = qemu-system-i386.exe
VM_FLAGS = -vga virtio

ifeq ($(OS),Windows_NT)
	SHELL=cmd
	RM = del /S
	DEL_OBJS = *.o *.elf *.out
else
	RM = rm -f
	DEL_OBJS = $(OBJS) $(KERNEL_BIN)
endif

all: $(KERNEL_BIN)

.PHONY: clean run rebuild
clean:
	$(RM) $(DEL_OBJS)

$(KERNEL_BIN): $(OBJS)
	$(LD) $^ $(LD_FLAGS) -o $@

%.o: %.c
	$(CC) $< $(CC_FLAGS) -o $@

%.o: %.s
	$(AS) $< $(AS_FLAGS) -o $@

run: all
	$(VM) $(VM_FLAGS) -kernel $(KERNEL_BIN)

rebuild: clean $(KERNEL_BIN)