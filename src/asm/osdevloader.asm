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
	dd 0, 0, 0, 0, 0

	dd 0 ; linear graphic mode
	dd 800; width
	dd 600 ; height
	dd 32 ; depth

section .bss
align 16
stack_bottom:
	resb 16384 ; 16 KiB
stack_top:

; section .text
; global _start:function (_start.end - _start)
; _start:
; 	; stack setup
; 	mov esp, stack_top

; 	extern main
; 	call main

; 	; If main returns, write to screen 'H' and hang the CPU.
; 	mov dword [0xb8000], 0x2f48 ; 'H' in white on green
; 	cli
; .hang:	hlt
; 	jmp .hang
; .end:

section .boot
global _start
_start:
	; load page dir
	mov eax, (boot_page_dir - 0xC0000000)
	mov cr3, eax

	; enable PSE
	mov ecx, cr4
	or ecx, 0x10
	mov cr4, ecx

	; enable paging
	mov ecx, cr0
	or ecx, 0x80000000
	mov cr0, ecx

	jmp _init_kernel

section .text
global _init_kernel
_init_kernel:
	mov esp, stack_top ; set up stack
	push ebx ; push magic num to stack
	xor ebp, ebp
	extern main
	call main

	; If main returns, write to screen 'H' and hang the CPU.
	mov dword [0xb8000], 0x2f48 ; 'H' in white on green
	cli
.hang:	hlt
	jmp .hang

section .data
; paging
align 4096

global boot_page_dir
boot_page_dir:
	dd 10000011b

	times 768-1 dd 0

	dd (0 << 22) | 10000011b
	dd (1 << 22) | 10000011b
	dd (2 << 22) | 10000011b
	dd (3 << 22) | 10000011b
	times 256-4 dd 0
