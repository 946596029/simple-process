
; proc process control block offset
stackframe_top          equ 72
stackframe_base         equ 0

stackframe_gs           equ 0
stackframe_fs           equ 4
stackframe_es           equ 8
stackframe_ds           equ 12
stackframe_edi          equ 16
stackframe_esi          equ 20
stackframe_ebp          equ 24
stackframe_esp_kernel   equ 28
stackframe_ebx          equ 32
stackframe_edx          equ 36
stackframe_ecx          equ 40
stackframe_eax          equ 44
stackframe_retaddr      equ 48

stackframe_eip          equ 52
stackframe_cs           equ 56
stackframe_eflags       equ 60
stackframe_esp          equ 64
stackframe_ss           equ 68

proc_ldt_sel            equ 72
proc_ldt_desc1          equ 74
proc_ldt_desc2          equ 82
proc_pid                equ 90
proc_p_name             equ 94

; tss attribute offset
tss_backlink    equ 0;
tss_esp0        equ 4
tss_ss0         equ 8
tss_esp1        equ 12
tss_ss1         equ 16
tss_esp2        equ 20
tss_ss2         equ 24
tss_gr3         equ 28
tss_eip         equ 32
tss_eflags      equ 36
; other attributes do not use now ,so i will write when i need it.

SelectorTSS     equ 0x20

; reenter         equ -1
INT_M_CTL       equ 0x20 ; master 8259a core 
INT_M_CTLMASK   equ 0x21 ; master 9259a mask code
INT_S_CTL       equ 0xA0 ; slave 8259a core 
INT_S_CTLMASK   equ 0xA1 ; slave 8259a mask code

EOI             equ 0x20 ; let this in , the next inte can change