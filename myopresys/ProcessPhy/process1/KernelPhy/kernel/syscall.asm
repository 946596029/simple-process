[section .bss]
Sys_call_get_ticks  equ 0x80
NR_get_ticks        equ 0x00
%include "sconst.inc"

[section .text]

global get_ticks

get_ticks:
    mov eax,NR_get_ticks
    int Sys_call_get_ticks
    ret

