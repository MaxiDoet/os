section .text
extern gdtDescriptor
global gdt_install

gdt_install:
    cli
    lgdt [gdtDescriptor]
    jmp 08h:reload_cs

reload_cs:
    mov eax, 10h
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov ss, eax
    ret