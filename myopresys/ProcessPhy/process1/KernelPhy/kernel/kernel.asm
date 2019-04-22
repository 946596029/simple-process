SELECTOR_KERNEL_CS equ 8

%include "sconst.inc"

; phy function
	extern exception_handler
	extern irq_action
	extern clock_handler
	extern delay
; function
	extern cstart
	extern kernel_main
; value
	extern idt_ptr
	extern gdt_ptr
	extern disp_pos
	extern proc_ready_ptr
	extern tss
	extern irq_table
	extern k_reenter
	extern sys_call_table

bits 32
[section .data]
clock_int_msg db "M",0

[section .bss]
StackSpace resb 2*1024
StackTop:

[section .text]

	global _start
	global restart
	global sys_call

	global divide_fault
	global debug_fault_trap
	global breakpoint_trap
	global overflow_trap
	global out_of_range_fault
	global vaild_opcode_fault
	global equipment_fault
	global double_abort
	global coprocessor_out_of_segment_fault
	global vaild_TSS_fault
	global segment_not_present_fault
	global stack_segment_fault
	global routine_protect_fault
	global page_fault
	global x87fpu_float_fault
	global align_check_fault
	global machine_check_abort
	global simd_float_fault

	global hwint00
	global hwint01
	global hwint02
	global hwint03
	global hwint04
	global hwint05
	global hwint06
	global hwint07
	global hwint08
	global hwint09
	global hwint10
	global hwint11
	global hwint12
	global hwint13
	global hwint14
	global hwint15

_start:
	mov esp,StackTop
	mov dword [disp_pos],0

	sgdt[gdt_ptr]
	call cstart
	lgdt[gdt_ptr]

	lidt[idt_ptr]

	jmp SELECTOR_KERNEL_CS:cinit
cinit:
	;ud2

	push eax
	xor eax,eax
	mov ax,SelectorTSS
	ltr ax
	pop eax

	jmp kernel_main
;==========================================
;  exception handlers
;==========================================
divide_fault:
	push 0xffffffff	
	push 0
	jmp exception
debug_fault_trap:
	push 0xffffffff
	push 1
	jmp exception
breakpoint_trap:
	push 0xffffffff
	push 3
	jmp exception
overflow_trap:
	push 0xffffffff
	push 4
	jmp exception
out_of_range_fault:
	push 0xffffffff
	push 5
	jmp exception
vaild_opcode_fault:
	push 0xffffffff
	push 6
	jmp exception
equipment_fault:
	push 0xffffffff
	push 7
	jmp exception
double_abort:
	push 8
	jmp exception
coprocessor_out_of_segment_fault:
	push 0xffffffff
	push 9
	jmp exception
vaild_TSS_fault:
	push 10
	jmp exception
segment_not_present_fault:
	push 11
	jmp exception
stack_segment_fault:
	push 12
	jmp exception
routine_protect_fault:
	; 常规保护错误
	push 13
	jmp exception
page_fault:
	push 14
	jmp exception
x87fpu_float_fault:
	push 0xffffffff
	push 16
	jmp exception
align_check_fault:
	push 17
	jmp exception
machine_check_abort:
	push 0xffffffff
	push 18
	jmp exception
simd_float_fault:
	push 0xffffffff
	push 19
	jmp exception
;==================================
; hardware interrupt (15)
;==================================
%macro hwint_master 1
	call save
	in al,INT_M_CTLMASK
	or al,(1<<%1)
	out INT_M_CTLMASK,al
	mov al,EOI
	out INT_M_CTL,al
	sti
	push %1
	call [irq_table + 4 * %1]
	pop ecx
	cli
	in al,INT_M_CTLMASK
	and al,~(1<<%1)
	out INT_M_CTLMASK,al
	ret
%endmacro
ALIGN 16
hwint00:
	hwint_master 0
ALIGN 16
hwint01:
	hwint_master 1
ALIGN 16
hwint02:
	hwint_master 2
ALIGN 16
hwint03:
	hwint_master 3
ALIGN 16
hwint04:
	hwint_master 4
ALIGN 16
hwint05:
	hwint_master 5
ALIGN 16
hwint06:
	hwint_master 6
ALIGN 16
hwint07:
	hwint_master 7
hwint08:
	push 0x28
	jmp hard_exception
hwint09:
	push 0x29
	jmp hard_exception
hwint10:
	push 0x2A
	jmp hard_exception
hwint11:
	push 0x2B
	jmp hard_exception
hwint12:
	push 0x2C
	jmp hard_exception
hwint13:
	push 0x2D
	jmp hard_exception
hwint14:
	push 0x2E
	jmp hard_exception
hwint15:
	push 0x2F
	jmp hard_exception

exception:
	call exception_handler
	add esp,8
	jmp $
	iretd

hard_exception:
	call irq_action
	add esp,4
	jmp $
save:
	;sub esp,4 // this memory block is change to next command of call save
	pushad
	push ds
	push es
	push fs
	push gs
	mov ax,ss
	mov ds,ax
	mov es,ax

	mov eax,[esp + stackframe_eax - stackframe_base]
	mov esi,esp
	; i think use process stack to transport value is better than regsiter

	inc dword [k_reenter]
	cmp dword [k_reenter],0
	jne .1

	mov esp,StackTop

	push restart
	jmp [esi + stackframe_retaddr - stackframe_base]
.1:
	; why this do not change esp,beacause it interrupt in kernel stack, not in
	; PCB stack
	push restart_re_enter
	jmp [esi + stackframe_retaddr - stackframe_base]

restart:
	; set ldtr
	mov esp,[proc_ready_ptr]
	lldt [esp + proc_ldt_sel]
	; set tss , this i am not think it
	lea esi,[esp + stackframe_top]
	mov dword [tss + tss_esp0],esi
restart_re_enter:
	dec dword [k_reenter]
	pop gs
	pop fs
	pop es
	pop ds
	popad
	add esp,4
	iretd

	;call save
	;inc byte [gs:0]
	;in al,INT_M_CTLMASK
	;or al,1
	;out INT_M_CTLMASK,al
	;mov al,EOI
	;out INT_M_CTL,al
	;sti ; and this is right
	;push 0
	;call clock_handler
	;add esp,4
	;cli
	;in al,INT_M_CTLMASK
	;and al,0xFE
	;out INT_M_CTLMASK,al
	;ret

sys_call:
	call save

	sti
	call [sys_call_table + 4 * eax]  ; eax value is change ,how it change
	; this i do not think it, 
	; this deal to the gcc will set return value to eax regsiter
	; and the clock inte may be occupy in this time
	mov [esi + stackframe_eax - stackframe_base],eax
	cli

	ret