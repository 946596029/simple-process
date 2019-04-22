;-------------------------------
; DA_ : Descriptor Attribute
; D   : Data Segment
; C   : Code Segment
; S   : System Segment
; R   : only read
; E   : only execute
; RW  : read and write
; A   : accessed
; others : as its name
;-------------------------------
; DOWN : expand below
; SAME : same code
;-------------------------------


; Descriptor class
DA_32	EQU	4000h	;32bit seg
DA_LIMIT_4K EQU 8000h	;segment 4K to 1
DA_DPL0	EQU	00h	;DPL=0
DA_DPL1 EQU	20h	;DPL=1
DA_DPL2	EQU	40h	;DPL=2
DA_DPL3 EQu	60h	;DPL=3

; Descriptor Attribute Value
; data segment
DA_DR	EQU	90h	;existed only can read
DA_DRA	EQU	91h	;existed only can read and accessed
DA_DRW	EQU	92h	;existed can read and write
DA_DRWA	EQU	93h	;existed can read and write and accessed
DA_DRDOWN	EQU	94h	;existed can read and expand below
DA_DRADOWN	EQU	95h	;existed can read and accessed and expand below
DA_DRWDOWN	EQU	96h	;existed can read and write and expand below
DA_DRWADOWN	EQU	97h	;existed can read and write and accessed and expand below

; code segment
DA_CE	EQU	98h	;existed only can execute
DA_CEA	EQU	99h	;existed only can execute and accessed
DA_CER	EQU	9Ah	;existed can execute and read
DA_CERA	EQU	9Bh	;existed can execute and read and accessed
DA_CESAME	EQU	9Ch	;existed only can execute and is same code segment
DA_CEASAME	EQU	9Dh	;existed only can execute and accessed and is same code segment
DA_CERSAME	EQU	9Eh	;existed can execute and read and is same code segment
DA_CERASAME	EQU	9Fh	;existed can execute and read and accessed and is same code segment

; system segment
;-----------------------------------------
; T286	:can use 286TSS
; F286	:busy 286TSS
; GATECALL286 	:286 call gate
; GATEINTE286 	:286 interrupter gate
; GATETRAP286 	:286 trap gate
; T386	:can use 386TSS
; F386  :busy 386TSS
; GATECALL386 	:386 call gate
; GATEINTE386 	:386 interrupter gate
; GATETRAP386	:386 trap gate
; LDT	:LDT Descriptor
; GATEMISSION	:mission gate
;-----------------------------------------

DA_ST286	EQU	81h	
DA_SF286	EQU	83h	
DA_SGATECALL286	EQU	84h	
DA_SGATEINTE286 EQU	86h	
DA_SGATETRAP286	EQU	87h	
DA_ST386	EQU	89h	
DA_SF386	EQU	8Bh	
DA_SGATECALL386	EQU	8Ch	
DA_SGATEINTE386	EQU	8Eh
DA_SGATETRAP386	EQU	8Fh
DA_LDT	EQU	82h
DA_GATEMISSION	EQU	85h

;----------------------------------------
; Page Attribute Note
; PG_  bits of page 
; P	:present
; RWR	:R/W attribute value, can read and execute
; RWW	:R/W attribute value, can read and execute and write
; USS	:U/S attribute value, system rank
; USU	;U/S attribute value, user rank
;----------------------------------------
PG_P	EQU 1
PG_RWR	EQU 0
PG_RWW	EQU 2
PG_USS	EQU 0
PG_USU	EQU 4

;----------------------------------------
;Selector picture
; 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16
; |        num                             | TI| RPL|
; SA_	:Selector Attribute
; RPLx	:request privilieage level x
; TIL 	:LDT Selector
; TIG	:GDT Selector
;----------------------------------------

SA_TIG	EQU	0
SA_TIL	EQU	4

SA_RPL0	EQU	0
SA_RPL1	EQU	1
SA_RPL2	EQU	2
SA_RPL3	EQU	3

;---------------------------------------
; Descriptor  three arguments
; usage: Descriptor Base Limit Attr
; Base: dd
; Limit: dd lower 20 bit is available
; Attr: dw higer 4 bits of lower 8 bits is zero
;--------------------------------------

%macro Descriptor 3
	dw %2 & 0FFFFh
	dw %1 & 0FFFFh
	db (%1 >> 16) & 0FFh
	dw (%3 & 0F0FFh) | ((%2 >> 8) & 0F00h)
	db (%1 >> 24) & 0FFh
%endmacro ;8 bytes

;--------------------------------------------
; Gate
; usage: Gate Selector, Offset, DCount, Attr
; Selector: dw
; Offset: dd
; DCount: db
; Attr: db
;--------------------------------------------
%macro Gate 4
	dw %2 & 0FFFFh
	dw %1 & 0FFFFh
	dw (%3 & 1Fh) | ((%4 << 8) & 0FF00h)
	dw (%2 >> 16) & 0FFFFh
%endmacro ;8 bytes



