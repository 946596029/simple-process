[section .data]
	extern disp_pos
	INT_M_CTL		equ 0x20
	INT_M_CTLMASK	equ	0x21
	INT_S_CTL		equ 0xA0
	INT_S_CTLMASK	equ 0xA1
[section .text]
	global disp_str
	global disp_str_color
	global out_byte
	global in_byte
	global disable_irq
	global enable_irq
;---------------------------------
;void disp_str(char* point)
;---------------------------------

disp_str:
	push ebp
	mov ebp,esp
	push ebx
	push esi
	push edi
	
	mov esi,[ebp+8]
	mov edi,[disp_pos]
	mov ah,0Fh
.1:
	lodsb
	test al,al
	jz .2
	cmp al,0Ah ; is space 
	jnz .3
	push eax
	mov eax,edi
	mov bl,160
	div bl
	and eax,0FFh
	inc eax
	mov bl,160
	mul bl
	mov edi,eax
	pop eax
	jmp .1	
.3:
	mov [gs:edi],ax
	add edi,2
	jmp .1
.2:
	mov [disp_pos],edi	

	pop edi
	pop esi
	pop ebx
	pop ebp
	ret
;===================================
;void out_byte(u16 port, u8 value)
;===================================
out_byte:
	mov edx,[esp+4] ; port
	mov al,[esp+8] ; value
	out dx,al
	nop
	nop
	ret
; end

;==================================
;u8 in_byte(u16 port)
;==================================
in_byte:
	mov edx,[esp+4]
	xor eax,eax
	in al,dx
	nop
	nop
	ret
; end

;===========================================
; void disp_str_color(char* info, int color)
;===========================================

disp_str_color:
	push ebp
	mov ebp,esp
	push eax
	push ebx
	push esi
	push edi
	
	mov esi,[ebp + 8] ; because of eip in the stack
	mov edi,[disp_pos]
	
	xor eax,eax
	mov ah,[ebp + 12]
.1:
	lodsb
	test al,al
	jz .3
	cmp al,0Ah
	jnz .2
	push eax
	mov eax,edi
	mov bl,160
	div bl
	and eax,0FFh
	inc eax
	mov bl,160
	mul bl
	mov edi,eax
	pop eax
	jmp .1
.2:
	mov [gs:edi],ax
	add edi,2
	jmp .1
.3:
	mov [disp_pos],edi

	pop edi
	pop esi
	pop ebx
	pop eax
	mov esp,ebp
	pop ebp
	ret
; end

;========================================
; void disable_irq(u32 irq_num)
;========================================

disable_irq:
	mov ecx,[esp+4]
	pushf ;store eflags regs,so do not need sti command to set IF equal to one
	cli
	mov ah,1
	rol ah,cl
	cmp cl,8
	jae disable_8
disable_0:
	in al,INT_M_CTLMASK
	test al,ah
	jnz disable_already
	or al,ah
	out INT_M_CTLMASK,al
	popf
	mov eax,1
	ret
disable_8:
	in al,INT_S_CTLMASK
	test al,ah
	jnz disable_already
	or al,ah
	out INT_S_CTLMASK,al
	popf
	mov eax,1
	ret
disable_already:
	popf
	xor eax,eax
	ret
;end

;=======================================
; void enable_irq(u32 irq_num)
;=======================================
enable_irq:
	mov ecx,[esp+4]
	pushf
	cli
	mov ah,0xfe
	rol ah,cl
	cmp cl,8
	jae disable_8
enable_0:
	in al,INT_M_CTLMASK
	and al,ah
	out INT_M_CTLMASK,al
	popf
	ret
enable_8:
	in al,INT_S_CTLMASK
	and al,ah
	out INT_S_CTLMASK,al
	popf
	ret
; end