//======== kliba.asm ===============================
PUBLIC void disp_str(char* info);
PUBLIC void disp_str_color(char* info, int color);
PUBLIC void out_byte(u16 port, u8 value);
PUBLIC u8   in_byte(u16 port);
PUBLIC void disable_irq(int irq_num);
PUBLIC void enable_irq(int irq_num);
//======== i8259.c =================================
PUBLIC void Init8259A();
PUBLIC void irq_action(u8 hard_vector, u32 cs, u32 eip, u32 eflags);
//======== klib.c ==================================
PUBLIC char* iota(char* info, int number);
PUBLIC void disp_int(int number);
PUBLIC void delay(int num);
//======== protect.c ===============================
PUBLIC void exception_handler(int vectorNum, int errorCode, int eip, int cs, int eflags);
PUBLIC void interrupt_init();
//======== main.c ==================================
PUBLIC void TestA();
PUBLIC void TestB();
PUBLIC void TestC();
PUBLIC void kernel_main();
PUBLIC u32 seg2base(u16 selector);
PUBLIC u32 vir2addr(u32 base, void* offset);
PUBLIC int sys_get_ticks();
PUBLIC void put_irq_handler(int irq_num, irq_hander func);
PUBLIC void put_sys_call_func(int index, sys_call_func func);
PUBLIC void process_init();
PUBLIC void process_state_init(u16 index, u16 selector, process_body func);
PUBLIC void init_descriptor(DESCRIPTOR* table, u16 selector, u32 base, u32 limit, u16 attribute);
//======== clock.c =================================
PUBLIC void clock_handler(int irq);
//======== syscall.asm ===============================
PUBLIC int get_ticks();
//======== kernel.asm ==============================
PUBLIC void sys_call();