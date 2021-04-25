# C-Kernel-Test
 ## Build Kernel
 ```bash
# Compile
i686-elf-gcc -std=gnu99 -ffreestanding -g -c src/start.s -o bin/obj/start.o
i686-elf-gcc -std=gnu99 -ffreestanding -g -c src/kernel.c -o bin/obj/kernel.o
# Link
i686-elf-gcc -ffreestanding -nostdlib -g -T src/linker.ld bin/obj/start.o bin/obj/kernel.o -o bin/kernel.elf -lgcc
 ```
## Run Kernel
```bash
qemu-system-i386 -kernel bin/kernel.elf
```