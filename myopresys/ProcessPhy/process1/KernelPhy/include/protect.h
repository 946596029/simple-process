#ifndef ORANGES_PROTECT_H_
#define ORANGES_PROTECT_H_

typedef struct s_descriptor{
	u16 limit_low;
	u16 base_low;
	u8  base_mid;
	u8  attr1; //P(1) DPL(2) DT(1) TYPE(4)
	u8  limit_high_attr2; //G(1) D(1) 0(1) AVL(1) LimitHigh(4)
	u8  base_high;
} DESCRIPTOR;

typedef struct s_gate{
	u16 offset_low;
	u16 selector_gate;
	u8  dcount; // 0 0 0 paramcount(5) 用于声明需要复制的参数个数
	u8  attribute; // P(1) DPL(2) S(1) TYPE(4)
	u16 offset_high;
} GATE;

/* GDT */
/* 描述符索引 */
#define	INDEX_DUMMY		0	// ┓
#define	INDEX_FLAT_C		1	// ┣ LOADER 里面已经确定了的.
#define	INDEX_FLAT_RW		2	// ┃
#define	INDEX_VIDEO		3	// ┛
/* 选择子 */
#define	SELECTOR_DUMMY		   0		// ┓
#define	SELECTOR_FLAT_C		0x08		// ┣ LOADER 里面已经确定了的.
#define	SELECTOR_FLAT_RW	0x10		// ┃
#define	SELECTOR_VIDEO		(0x18+3)	// ┛<-- RPL=3

#define	SELECTOR_KERNEL_CS	SELECTOR_FLAT_C
#define	SELECTOR_KERNEL_DS	SELECTOR_FLAT_RW


/* 描述符类型值说明 */
#define	DA_32			0x4000	/* 32 位段				*/
#define	DA_LIMIT_4K		0x8000	/* 段界限粒度为 4K 字节			*/
#define	DA_DPL0			0x00	/* DPL = 0				*/
#define	DA_DPL1			0x20	/* DPL = 1				*/
#define	DA_DPL2			0x40	/* DPL = 2				*/
#define	DA_DPL3			0x60	/* DPL = 3				*/
/* 存储段描述符类型值说明 */
#define	DA_DR			0x90	/* 存在的只读数据段类型值		*/
#define	DA_DRW			0x92	/* 存在的可读写数据段属性值		*/
#define	DA_DRWA			0x93	/* 存在的已访问可读写数据段类型值	*/
#define	DA_C			0x98	/* 存在的只执行代码段属性值		*/
#define	DA_CR			0x9A	/* 存在的可执行可读代码段属性值		*/
#define	DA_CCO			0x9C	/* 存在的只执行一致代码段属性值		*/
#define	DA_CCOR			0x9E	/* 存在的可执行可读一致代码段属性值	*/
/* 系统段描述符类型值说明 */
#define	DA_LDT			0x82	/* 局部描述符表段类型值			*/
#define	DA_TaskGate		0x85	/* 任务门类型值				*/
#define	DA_386TSS		0x89	/* 可用 386 任务状态段类型值		*/
#define	DA_386CGate		0x8C	/* 386 调用门类型值			*/
#define	DA_386IGate		0x8E	/* 386 中断门类型值			*/
#define	DA_386TGate		0x8F	/* 386 陷阱门类型值			*/


#define INT_VECTOR_DIVIDE	0x00
#define INT_VECTOR_DEBUG	0x01
#define INT_VECTOR_BREAKPOINT	0x03
#define INT_VECTOR_OVERFLOW	0x04
#define INT_VECTOR_BOUNDS	0x05
#define INT_VECTOR_VAILDOPCODE	0x06
#define INT_VECTOR_EQUIPMENT	0x07
#define INT_VECTOR_DOUBLEFAULT  0x08
#define INT_VECTOR_CPBOUNDS	0x09
#define INT_VECTOR_VAILDTSS	0x0A
#define INT_VECTOR_SEGNOTEXIST	0x0B
#define INT_VECTOR_STACKSEGFAULT	0x0C
#define INT_VECTOR_ROUTINEPROTECT	0x0D
#define INT_VECTOR_PAGEFAULT	0x0E

#define INT_VECTOR_X87FPUFLOAT	0x10
#define INT_VECTOR_ALIGNCHECK	0x11
#define INT_VECTOR_MACHINECHECK 0x12
#define INT_VECTOR_SIMDFLOAT	0x13

//=========================================
// int vector define here
//=========================================

#define INT_VECTOR_IRQ0	0x20
#define INT_VECTOR_IRQ8 0x28

#define INT_VECTOR_IRQ00 0x20
#define INT_VECTOR_IRQ01 0x21
#define INT_VECTOR_IRQ02 0x22
#define INT_VECTOR_IRQ03 0x23
#define INT_VECTOR_IRQ04 0x24
#define INT_VECTOR_IRQ05 0x25
#define INT_VECTOR_IRQ06 0x26
#define INT_VECTOR_IRQ07 0x27

#define INT_VECTOR_IRQ08 0x28
#define INT_VECTOR_IRQ09 0x29
#define INT_VECTOR_IRQ10 0x2A
#define INT_VECTOR_IRQ11 0x2B
#define INT_VECTOR_IRQ12 0x2C
#define INT_VECTOR_IRQ13 0x2D
#define INT_VECTOR_IRQ14 0x2E
#define INT_VECTOR_IRQ15 0x2F
//===========================================
// 0x14 --> 0x19 intel reserve
//===========================================
// 0x20 --> 0x30 8259A inteprrute used
// 0x30 --> 0x100 no use ,can set by yourself
//===========================================
#define INT_SYS_CALL_FUNC 0x80

#endif
