.PHONY: all clean dist build check testdrivers todolist build-x86 build-x86-kernel-bin echo

CFLAGS := -I src/kernel/lib/include -I src/kernel/lib/extra -I src/kernel/lib/include/util -std=gnu99 -ffreestanding -O2 -Wall -Wextra

# asm src
ASM_SRC := $(shell find src/asm -type f -name '*.asm')
ASM_OBJ := $(patsubst src/asm/%.asm, build/x86/asm/%_asm_src.o, $(ASM_SRC))

# c src
C_SRC := $(shell find src/kernel -type f -name '*.c')
C_HEADERS := $(shell find src/kernel -type f -name '*.h')
C_LIBS := $(wildcard src/kernel/lib/*.c)
C_LIBS_OBJ := $(patsubst src/kernel/lib/%.c, build/x86/lib/%_lib_src.o, $(C_LIBS))
C_OBJ := $(patsubst src/kernel/%.c, build/x86/%_src.o, $(C_SRC))

build/x86/asm/%_asm_src.o: src/asm/%.asm
	mkdir -p $(dir $@) && \
	nasm -f elf32 $< -o $@

build/x86/lib/%_lib_src.o: src/kernel/lib/%.c
	mkdir -p $(dir $@) && \
	i686-elf-gcc $(CFLAGS) -c $< -o $@

build/x86/%_src.o: src/kernel/%.c
	mkdir -p $(dir $@) && \
	i686-elf-gcc $(CFLAGS) -c $< -o $@

# other stuff
build-x86-kernel-bin: $(ASM_OBJ) $(C_OBJ) $(C_LIBS_OBJ)
	i686-elf-gcc -T linker.ld -o build/x86/nzux-kernel.bin -ffreestanding -O2 -nostdlib $(ASM_OBJ) $(C_OBJ)

# build iso
build-x86: build/x86/nzux-kernel.bin
	mkdir -p build/x86/x86_iso/ && \
	cp -r build/target/x86_iso -t build/x86/ && \
	cp build/x86/nzux-kernel.bin -t build/x86/x86_iso/boot/ && \
	grub-mkrescue -o dist/i686/nzux.iso build/x86/x86_iso

all: build-x86-kernel-bin build-x86
	@echo "Successfully built nzux"
	@echo "Build Statistics:"
	@echo "Number of Header files: $(words $(C_HEADERS))"
	@echo "Number of ASM object files: $(words $(ASM_OBJ))"
	@echo "Number of C object files: $(words $(C_OBJ))"
	@echo "Number of C library object files: $(words $(C_LIBS_OBJ))"

echo:
	@echo -e "ASM: $(ASM_SRC) \n ASM_OBJ: $(ASM_OBJ) \n C: $(C_SRC) \n C_OBJ: $(C_OBJ) \n H: $(C_HEADERS) \n CLIB: $(C_LIBS)"

clean:
	rm -rf build/x86/*