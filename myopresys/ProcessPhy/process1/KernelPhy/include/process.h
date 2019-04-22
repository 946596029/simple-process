#ifndef ORANGE_PROTECT_H_
#define ORANGE_PROTECT_H_

typedef struct s_tss{
    u32 backlink;
    u32 esp0;
    u32 ss0;
    u32 esp1;
    u32 ss1;
    u32 esp2;
    u32 ss2;
    u32 gr3;
    u32 eip;
    u32 eflags;
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 ebx;
    u32 esp;
    u32 ebp;
    u32 esi;
    u32 edi;
    u32 es;
    u32 cs;
    u32 ss;
    u32 ds;
    u32 fs;
    u32 gs;
    u32 LDTSelector;
    u16 trap;
    u16 io_base;
} TSS;

typedef struct PROCStack{
    u32 gs;
    u32 fs;
    u32 es;
    u32 ds;
    u32 edi;
    u32 esi;
    u32 ebp;
    u32 esp_kernel;
    u32 ebx;
    u32 edx;
    u32 ecx;
    u32 eax;
    u32 retaddr; // how to use it
    u32 eip;
    u32 cs;
    u32 eflags;
    u32 esp;
    u32 ss;
} s_stackframe;

typedef struct PCB{
    s_stackframe stackFrame;
    u16 ldt_selector;
    DESCRIPTOR ldts[LDT_SIZE];
    u32 pid;
    char p_name[16];
} PROCESS;

#define SELECTORTSS     0x20 
#define SELECTORTESTA   0x28 
#define SELECTORTESTB   0x30
#define SELECTORTESTC   0x38

#define SELECTORCS      0x00
#define SELECTORDS      0x08
#define SELECTORGS      0x18

#define SA_TIL          0x04
#define SA_TIG          0x00

#define SA_RPL_MASK     0x03
#define SA_RPL_KERNEL   0x00
#define SA_RPL_TASK     0x01
#define SA_RPL_USER     0x03

#define ProcStackTop    1024

#endif


