#include "type.h"
#include "const.h"
#include "protect.h"
#include "process.h"
#include "global.h"
#include "string.h"
#include "proto.h"

extern PUBLIC void restart();

PUBLIC void kernel_main()
{
    disp_str("-----------kernel main begin---------------\n");
    process_init();
    // 装载进程状态
    k_reenter = 0;
    ticks = 0;
    put_irq_handler(0,clock_handler);
    put_sys_call_func(0,sys_get_ticks);
    // 进入进程循环
    restart();
    // 这一句永远都执行不到
    // k_reenter = -1;
    while(1){

    }
}

PUBLIC void process_init()
{   
    /*// init the TSS segment
    memoryset(&tss_table[0],0,sizeof(TSS));

    init_descriptor(&gdt[SELECTORTSS>>3],
        SELECTORTSS,
        vir2addr(seg2base(SELECTOR_KERNEL_DS), &tss_table[0]),
        sizeof(TSS) - 1,
        DA_386TSS);
    tss_table[0].ss0 = SELECTOR_KERNEL_DS;*/

    process_state_init(0, SELECTORTESTA, TestA);

    // init the process testA ldt
    init_descriptor(&gdt[SELECTORTESTA>>3],
        SELECTORTESTA,
        vir2addr(seg2base(SELECTOR_KERNEL_DS), &proc_table[0].ldts),
        sizeof(DESCRIPTOR) * 2 - 1,
        DA_LDT);

    process_state_init(1, SELECTORTESTB, TestB);

    // init the process testB ldt
    init_descriptor(&gdt[SELECTORTESTB>>3],
        SELECTORTESTB,
        vir2addr(seg2base(SELECTOR_KERNEL_DS), &proc_table[1].ldts),
        sizeof(DESCRIPTOR) * 2 - 1,
        DA_LDT);

    process_state_init(2, SELECTORTESTC, TestC);

    init_descriptor(&gdt[SELECTORTESTC>>3],
        SELECTORTESTC,
        vir2addr(seg2base(SELECTOR_KERNEL_DS), &proc_table[2].ldts),
        sizeof(DESCRIPTOR) * 2 - 1,
        DA_LDT);
}

// only a lower function
PUBLIC void process_state_init(u16 index, u16 selector, process_body func)
{
    memorycpy(&proc_table[index].ldts[0], &gdt[INDEX_FLAT_C], sizeof(DESCRIPTOR));
    proc_table[index].ldts[0].attr1 = DA_CR  | (DESC_DPL_TASK << 5);
    memorycpy(&proc_table[index].ldts[1], &gdt[INDEX_FLAT_RW], sizeof(DESCRIPTOR));
    proc_table[index].ldts[1].attr1 = DA_DRW | (DESC_DPL_TASK << 5);
    
    proc_table[index].ldt_selector = selector;
    proc_table[index].stackFrame.gs = (SELECTORGS | SA_TIG | SA_RPL_TASK);
    proc_table[index].stackFrame.cs = (SELECTORCS | SA_TIL | SA_RPL_TASK);
    proc_table[index].stackFrame.es = (SELECTORDS | SA_TIL | SA_RPL_TASK);
    proc_table[index].stackFrame.ds = (SELECTORDS | SA_TIL | SA_RPL_TASK);
    proc_table[index].stackFrame.fs = (SELECTORDS | SA_TIL | SA_RPL_TASK);
    proc_table[index].stackFrame.ss = (SELECTORDS | SA_TIL | SA_RPL_TASK);
    proc_table[index].stackFrame.eip = (u32)(*func);
    proc_table[index].stackFrame.esp = (u32)(proc_stack) + ProcStackTop*index;// 加法至堆栈底
    proc_table[index].stackFrame.eflags = 0x1202;

    proc_ready_ptr = &proc_table[index];
}

// this function directly use physical address, so you need translate the address
PUBLIC void init_descriptor(DESCRIPTOR* desc, u16 selector, u32 base, u32 limit, u16 attribute)
{
    //u16 num = selector >> 3;// SA_RPL will be ignore , beacause it likes int type divide
    desc->limit_low = limit & 0xFFFF;
    desc->base_low = base & 0xFFFF;
    desc->base_mid = (base>>16) & 0xFF;
    desc->attr1 = attribute & 0xFF;
    desc->limit_high_attr2= (attribute >> 8) & 0xF0 | (limit >> 16) & 0x0F;
    desc->base_high = (base>>24) & 0xFF;
}

PUBLIC u32 seg2base(u16 selector)
{
    u16 index = selector >> 3;
    DESCRIPTOR* desc = &gdt[index];
    /*u32 address = address + (desc->base_low | 0x0000FFFF);
    address = address + (desc-> base_mid | 0x00FF0000);
    address = address + (desc-> base_high | 0xFF000000);*/
    // beacause the regs is 32 bits ,so the data can do this
    u32 address = ((desc->base_high<<24)|(desc->base_mid<<16)|(desc->base_low));
    return address;
}

// 这里使用的平坦模型，所以在映射这里可以可以不需要映射
PUBLIC u32 vir2addr(u32 base, void* offset)
{
    return ( base +  (u32)(offset) );
}

/*
    注释原因，动态寻找不利于调试
    PUBLIC void init_process_ldt_desc()
    {
        DESCRIPTOR* g_table = &gdt;
        for(int i=0;i<GDT_SIZE;i++)
        {
            if((g_table[i].attr1 & 0x10) != 0x10 )
            {
                init_descriptor(i,)
            }
        }
    }
*/

/*
this set irq_handler and sys_call_func
*/
PUBLIC void put_irq_handler(int irq_num, irq_hander func){
    disable_irq(irq_num);
    irq_table[irq_num]=func;
    enable_irq(irq_num);
}

PUBLIC void put_sys_call_func(int index, sys_call_func func)
{
    sys_call_table[index]=(void*)(func);
}

PUBLIC int sys_get_ticks()
{
    //disp_str("+");
    return ticks;
}

//====================process body================================
PUBLIC void TestA(){
    int i = 0;
    while(1){
        disp_str("A");
        disp_int(get_ticks());
        disp_str(".");
        delay(1);
    }
}

PUBLIC void TestB(){
    int i = 0x1000;
    while(1){
        disp_str("B");
        disp_int(get_ticks());
        disp_str(".");
        delay(1);
    }
}

PUBLIC void TestC(){
    int i = 0x2000;
    while(1){
        disp_str("C");
        disp_int(get_ticks());
        disp_str(".");
        delay(1);
    }
}



