; multiboot header
MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
MBFLAGS  equ  MBALIGN | MEMINFO ; multiboot flags
MAGIC    equ  0x1BADB002        ; magic num
CHECKSUM equ -(MAGIC + MBFLAGS)   ; checksum of above, to prove we are multiboot

; Declare the multiboot header.
section .multiboot
align 4
	dd MAGIC
	dd MBFLAGS
	dd CHECKSUM

section .bss
align 16
stack_bottom:
	resb 16384 ; 16 KiB
stack_top:

section .text
global _start:function (_start.end - _start)
_start:
	; stack setup
	mov esp, stack_top

	extern main
	call main

	; If main returns, write to screen 'H' and hang the CPU.
	mov dword [0xb8000], 0x2f48 ; 'H' in white on green
	cli
.hang:	hlt
	jmp .hang
.end: