; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                              string.asm
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                                                       Forrest Yu, 2005
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

[SECTION .text]

; 导出函数
global	memorycpy
global 	memoryset

; ------------------------------------------------------------------------
; void* memcpy(void* es:pDest, void* ds:pSrc, int iSize);
; ------------------------------------------------------------------------
memorycpy:
	push	ebp
	mov	ebp, esp

	push	esi
	push	edi
	push	ecx

	mov	edi, [ebp + 8]	; Destination
	mov	esi, [ebp + 12]	; Source
	mov	ecx, [ebp + 16]	; Counter
.1:
	cmp	ecx, 0		; 判断计数器
	jz	.2		; 计数器为零时跳出

	mov	al, [ds:esi]		; ┓
	inc	esi			; ┃
					; ┣ 逐字节移动
	mov	byte [es:edi], al	; ┃
	inc	edi			; ┛

	dec	ecx		; 计数器减一
	jmp	.1		; 循环
.2:
	mov	eax, [ebp + 8]	; 返回值

	pop	ecx
	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp

	ret			; 函数结束，返回
; memcpy 结束-------------------------------------------------------------

;=============================================================
; memset( char* pDst, char ch, int size)
;=============================================================
memoryset:
	push ebp;
	mov ebp,esp;// store esp value ,in case of the func change the value of esp
	push ecx
	push eax
	push edi

	mov edi,[ebp+8]
	mov  al,[ebp+12]
	mov ecx,[ebp+16]
.1:
	cmp ecx,0
	jz .2
	mov [ds:edi],al
	inc edi
	dec ecx
	jmp .1
.2:
	pop edi
	pop eax
	pop ecx
	mov esp,ebp
	pop ebp
	ret
; memset finish
