section .multiboot
    dd 0x1badb002
    dd 3
    dd -(0x1badb002 + 3)

section .bootstrap_stack nobits
stack_bottom:
    resb 16384 ; 16 KiB
stack_top:

section .text

extern kmain

global _start

_start:
    cli

    ; Pass the multiboot info to the kernel
    push ebx
    ; Pass the magic value
    push eax

    call kmain

    cli
    hlt
.lhang:
    jmp .lhang