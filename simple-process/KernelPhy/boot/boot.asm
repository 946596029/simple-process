	org 07c00h

	BaseOfStack equ 07c00h		

	jmp short LABEL_START
	nop
	
	%include "fat12hdr.inc"
	%include "load.inc"

LABEL_START:
	mov ax,cs
	mov ds,ax
	mov ss,ax
	mov es,ax	
	mov sp,BaseOfStack
	
	; clear screen
	mov ax,0600h
	mov bx,0700h
	mov cx,0
	mov dx,0184fh
	int 10h

	mov dh,0
	call DispStr
	
	xor ah,ah
	xor dl,dl
	int 13h

	mov word [wSectorNo],SectorNoOfRootDirectory
	mov word [wRootDirSizeForLoop],RootDirSectors
LABEL_SEARCH_IN_ROOT_DIR_BEGIN:
	cmp word [wRootDirSizeForLoop],0
	jz LABEL_NO_LOADERBIN
	dec word [wRootDirSizeForLoop]
	mov ax,BaseOfLoader
	mov es,ax
	mov bx,OffsetOfLoader
	mov ax,[wSectorNo]
	mov cl,1
	call ReadSector ; es:bx is ready , ax is SectorNo	
	
	mov si,LoaderFileName
	mov di,OffsetOfLoader ; the content is ready to cmp is same or different
	cld 
	; change di grow direction
	mov dx,10h
LABEL_SEARCH_FOR_LOADERBIN:
	cmp dx,0
	jz LABEL_GOTO_NEXT_SECTOR_IN_ROOT_DIR
	dec dx
	mov cx,11
LABEL_CMP_FILENAME:
	cmp cx,0
	jz LABEL_FILENAME_FOUND
	dec cx
	lodsb
	cmp al,byte [es:di]
	jz LABEL_GO_ON
	jmp LABEL_DIFFERENT
LABEL_GO_ON:
	inc di
	jmp LABEL_CMP_FILENAME
LABEL_DIFFERENT:
	and di,0FFE0h
	add di,20h
	mov si,LoaderFileName
	jmp LABEL_SEARCH_FOR_LOADERBIN 
LABEL_GOTO_NEXT_SECTOR_IN_ROOT_DIR:
	add word [wSectorNo],1
	jmp LABEL_SEARCH_IN_ROOT_DIR_BEGIN
LABEL_NO_LOADERBIN:
	mov dh,2
	call DispStr
	; if not find loader.bin, will shut down there
	jmp $

LABEL_FILENAME_FOUND:
	mov ax,RootDirSectors
	and di,0FFE0h ; point to this item start position
	add di,01Ah ; offset to the first cluster of this item file
	mov cx,word[es:di]
	push cx
	add cx,ax
	add cx,DeltaSectorNo
	mov ax,BaseOfLoader
	mov es,ax
	mov bx,OffsetOfLoader
	mov ax,cx
LABEL_GOON_LOADING_FILE:
	push ax
	push bx
	mov ah,0Eh ; let the cursor move forward a foot
	mov al,'.' ; the content this position should display
	mov bl,0Fh ; set the background
	int 10h
	pop bx
	pop ax

	mov cl,1
	call ReadSector
	pop ax
	call GetFATEntry
	cmp ax,0FFFh

	jz LABEL_FILE_LOADED
	push ax
	mov dx,RootDirSectors
	add ax,dx
	add ax,DeltaSectorNo
	add bx,[BPB_BytsPerSec]
	jmp LABEL_GOON_LOADING_FILE
LABEL_FILE_LOADED:
	mov dh,1
	call DispStr

	jmp BaseOfLoader:OffsetOfLoader
	





wRootDirSizeForLoop	dw 0
wSectorNo	dw 0
b0dd	db 0
LoaderFileName	db "LOADER  BIN",0
MessageLength	equ 9
BootMessage	db "Booting  "
Message1	db "Ready.   "
Message2	db "No LOADER"


DispStr:
	mov ax,MessageLength
	mul dh
	add ax,BootMessage
	mov bp,ax
	mov ax,ds
	mov es,ax
	mov cx,MessageLength
	mov ax,01301h
	mov bx,0007h
	mov dl,0
	int 10h
	ret

ReadSector:
	push bp
	mov bp,sp
	sub esp,2
	
	mov byte [bp-2],cl
	push bx
	mov bl,[BPB_SecPerTrk]
	div bl
	inc ah
	mov cl,ah
	mov dh,al
	shr al,1
	mov ch,al
	and dh,1
	pop bx
	mov dl,[BS_DrvNum]
.GoOnReading:
	mov ah,2
	mov al,byte [bp-2]
	int 13h
	jc .GoOnReading

	add esp,2
	pop bp
	
	ret
	
GetFATEntry:
	push es
	push bx
	push ax
	mov ax,BaseOfLoader
	sub ax,0100h
	mov es,ax
	pop ax
	mov byte [b0dd],0
	mov bx,3
	mul bx
	mov bx,2
	div bx
	cmp dx,0
	jz LABEL_EVEN	
	mov byte [b0dd],1
LABEL_EVEN:
	xor dx,dx
	mov bx,[BPB_BytsPerSec]
	div bx
	
	push dx
	mov bx,0
	add ax,SectorNoOfFAT1
	mov cl,2
	call ReadSector

	pop dx
	add bx,dx
	mov ax,[es:bx]
	cmp byte [b0dd],1
	jnz LABEL_EVEN_2
	shr ax,4
LABEL_EVEN_2:
	and ax,0FFFh
LABEL_GET_FAT_ENRY_OK:	
	pop bx
	pop es
	ret

times 510 - ($ - $$) db 0
dw 0xaa55

