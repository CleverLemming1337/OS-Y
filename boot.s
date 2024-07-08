.section .text
.global _start

_start:
    mov $0x2BADB002, %eax
    jmp kernel_main