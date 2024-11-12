%macro ISR_ERRCODE 1
global isr%1
isr%1:
    cli
    push long 0
    push long %1
    jmp isr_common_stub
%endmacro

%macro ISR_NO_ERRCODE 1
global isr%1
isr%1:
    cli
    push long 0
    push long 0
    push long %1
    jmp isr_common_stub
%endmacro

%macro IRQ 2
global irq%1
irq%1:
    cli
    push long 0
    push long %2
    jmp irq_common_stub
%endmacro

ISR_NO_ERRCODE 0
ISR_NO_ERRCODE 1
ISR_NO_ERRCODE 2
ISR_NO_ERRCODE 3
ISR_NO_ERRCODE 4
ISR_NO_ERRCODE 5
ISR_NO_ERRCODE 6
ISR_NO_ERRCODE 7

ISR_ERRCODE 8
ISR_NO_ERRCODE 9 
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14

ISR_NO_ERRCODE 15
ISR_NO_ERRCODE 16
ISR_NO_ERRCODE 17
ISR_NO_ERRCODE 18
ISR_NO_ERRCODE 19
ISR_NO_ERRCODE 20
ISR_NO_ERRCODE 21
ISR_NO_ERRCODE 22
ISR_NO_ERRCODE 23
ISR_NO_ERRCODE 24
ISR_NO_ERRCODE 25
ISR_NO_ERRCODE 26
ISR_NO_ERRCODE 27
ISR_NO_ERRCODE 28
ISR_NO_ERRCODE 29
ISR_NO_ERRCODE 30
ISR_NO_ERRCODE 31

; Master PIC
IRQ   0,    32
IRQ   1,    33
IRQ   2,    34
IRQ   3,    35
IRQ   4,    36
IRQ   5,    37
IRQ   6,    38
IRQ   7,    39

; Slave PIC
IRQ   8,    40
IRQ   9,    41
IRQ  10,    42
IRQ  11,    43
IRQ  12,    44
IRQ  13,    45
IRQ  14,    46
IRQ  15,    47

extern exception_handler
isr_common_stub:
    ; Push GP registers to stack
    pusha
    push ds
    push es
    push fs
    push gs

    ; Load kernel data GDT descriptor
    mov ax, 0x08
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Push up stack
    mov eax, esp
    push eax
    mov eax, exception_handler
    call eax       ; Special call, preserves the 'eip' register
    pop eax

    ; Restore CPU state and return to main code
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret

extern irq_handler
irq_common_stub:
    ; Push GP registers to stack
    pusha
    push ds
    push es
    push fs
    push gs

    ; Load kernel data GDT descriptor
    mov ax, 0x08
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Push up stack
    mov eax, esp
    push eax
    mov eax, irq_handler
    call eax       ; Special call, preserves the 'eip' register
    pop eax

    ; Restore CPU state and return to main code
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret


global isr_stub_table
isr_stub_table:
%assign i 0 
%rep    32 
    dd isr%+i ; use DQ instead if targeting 64-bit
%assign i i+1 
%endrep

global irq_stub_table
irq_stub_table:
%assign j 0
%rep    16
    dd irq%+j
%assign j j+1
%endrep

global idt_load
extern idtp
idt_load:
    lidt [idtp]
    ret