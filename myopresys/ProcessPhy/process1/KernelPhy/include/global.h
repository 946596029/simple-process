#ifdef ORANGES_GLOBAL_HERE
#undef EXTERN
#define EXTERN
#endif

EXTERN int disp_pos;
EXTERN u8 gdt_ptr[6];
EXTERN DESCRIPTOR gdt[GDT_SIZE];
EXTERN u8 idt_ptr[6];
EXTERN GATE idt[IDT_SIZE];

EXTERN u32 k_reenter;
EXTERN int ticks;

EXTERN PROCESS proc_table[3];
EXTERN PROCESS* proc_ready_ptr;
EXTERN TSS tss;
EXTERN char proc_stack[10240];
EXTERN irq_hander irq_table[16];

EXTERN sys_call_func sys_call_table[16];