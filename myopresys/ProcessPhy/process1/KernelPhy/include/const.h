#ifndef ORANGES_CONST_H_
#define ORANGES_CONST_H_

#define PUBLIC
#define PRIVATE static

#define EXTERN extern

#define GDT_SIZE 128
#define IDT_SIZE 256
#define LDT_SIZE 2

#define INT_M_CTL 0x20
#define INT_M_CTLMASK 0x21
#define INT_S_CTL 0xA0
#define INT_S_CTLMASK 0xA1

//===========================================
// privilege class define here
//===========================================
#define DESC_DPL_KERNEL		0
#define DESC_DPL_TASK   	1
#define DESC_DPL_USER		3

#endif
