.PHONY: all clean dist build check testdrivers todolist build-x86 build-x86-kernel-bin echo

# asm src
ASM_SRC := $(wildcard src/asm/*.asm)
ASM_OBJ := $(patsubst src/asm/%.asm, build/x86/%.o, $(ASM_SRC))

# c src
C_SRC := $(shell find src/kernel -type f -name '*.c')
C_HEADERS := $(shell find src/kernel -type f -name '*.h')
C_LIBS := $(wildcard src/kernel/lib/*.c)
C_LIBS_OBJ := $(patsubst src/kernel/lib/%.c, build/x86/%.o, $(C_LIBS))
C_OBJ := $(patsubst src/kernel/%.c, build/x86/%.o, $(C_SRC))

$(ASM_OBJ): build/x86/%.o : src/asm/%.asm
	mkdir -p $(dir $@) && \
	nasm -f elf32 $< -o $@

$(C_LIBS_OBJ): build/x86/%.o : src/kernel/lib/%.c
	mkdir -p $(dir $@) && \
	i686-elf-gcc -std=gnu99 -ffreestanding -O2 -Wall -Wextra -c $< -o $@

$(C_OBJ): build/x86/%.o : src/kernel/%.c
	mkdir -p $(dir $@) && \
	i686-elf-gcc -std=gnu99 -ffreestanding -O2 -Wall -Wextra -c $< -o $@

# other stuff
build-x86-kernel-bin: $(ASM_OBJ) $(C_OBJ) $(C_LIBS_OBJ)
	i686-elf-gcc -T linker.ld -o build/x86/nzux-kernel.bin -ffreestanding -O2 -nostdlib $(ASM_OBJ) $(C_OBJ)

# build iso
build-x86: build/x86/nzux-kernel.bin
	cp -r build/target/x86_iso -t build/x86/ && \
	cp build/x86/nzux-kernel.bin -t build/x86/x86_iso/boot/ && \
	grub-mkrescue -o dist/i686/nzux.iso build/x86/x86_iso

all: build-x86-kernel-bin build-x86
	echo ASM: $(ASM_SRC) ASM_OBJ: $(ASM_OBJ) C: $(C_SRC) C_OBJ: $(C_OBJ) H: $(C_HEADERS)

echo:
	echo ASM: $(ASM_SRC) ASM_OBJ: $(ASM_OBJ) C: $(C_SRC) C_OBJ: $(C_OBJ) H: $(C_HEADERS)