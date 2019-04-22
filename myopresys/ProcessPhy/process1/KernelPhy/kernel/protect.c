#include "type.h"
#include "const.h"
#include "protect.h"
#include "process.h"
#include "global.h"
#include "proto.h"
#include "string.h"

PRIVATE void init_desc_gate(u16 vectorNum, int_handler func, u8 desc_type, u8 privilege_run );

void divide_fault();
void debug_fault_trap();
void breakpoint_trap();
void overflow_trap();
void out_of_range_fault();
void vaild_opcode_fault();
void equipment_fault();
void double_abort();
void coprocessor_out_of_segment_fault();
void vaild_TSS_fault();
void segment_not_present_fault();
void stack_segment_fault();
void routine_protect_fault();
void page_fault();
void x87fpu_float_fault();
void align_check_fault();
void machine_check_abort();
void simd_float_fault();
// hardware interrupt
void hwint00();
void hwint01();
void hwint02();
void hwint03();
void hwint04();
void hwint05();
void hwint06();
void hwint07();
void hwint08();
void hwint09();
void hwint10();
void hwint11();
void hwint12();
void hwint13();
void hwint14();
void hwint15();

PUBLIC void interrupt_init()
{
	Init8259A();
	init_desc_gate(INT_VECTOR_DIVIDE, divide_fault, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_DEBUG, debug_fault_trap, DA_386IGate, DESC_DPL_KERNEL);
	
	init_desc_gate(INT_VECTOR_BREAKPOINT, breakpoint_trap, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_OVERFLOW, overflow_trap, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_BOUNDS, out_of_range_fault, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_VAILDOPCODE, vaild_opcode_fault, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_EQUIPMENT, equipment_fault, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_DOUBLEFAULT, double_abort, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_CPBOUNDS, coprocessor_out_of_segment_fault, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_VAILDTSS, vaild_TSS_fault, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_SEGNOTEXIST, segment_not_present_fault, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_STACKSEGFAULT, stack_segment_fault, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_ROUTINEPROTECT, routine_protect_fault, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_PAGEFAULT, page_fault, DA_386IGate, DESC_DPL_KERNEL);
	
	init_desc_gate(INT_VECTOR_X87FPUFLOAT, x87fpu_float_fault, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_ALIGNCHECK, align_check_fault, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_MACHINECHECK, machine_check_abort, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_SIMDFLOAT, simd_float_fault, DA_386IGate, DESC_DPL_KERNEL);

	init_desc_gate(INT_VECTOR_IRQ00, hwint00, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_IRQ01, hwint01, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_IRQ02, hwint02, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_IRQ03, hwint03, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_IRQ04, hwint04, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_IRQ05, hwint05, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_IRQ06, hwint06, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_IRQ07, hwint07, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_IRQ08, hwint08, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_IRQ09, hwint09, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_IRQ10, hwint10, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_IRQ11, hwint11, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_IRQ12, hwint12, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_IRQ13, hwint13, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_IRQ14, hwint14, DA_386IGate, DESC_DPL_KERNEL);
	init_desc_gate(INT_VECTOR_IRQ15, hwint15, DA_386IGate, DESC_DPL_KERNEL);
	
	// init system call soft interrupt
	init_desc_gate(INT_SYS_CALL_FUNC, sys_call, DA_386IGate, DESC_DPL_USER);

	// init the TSS segment
    memoryset(&tss,0,sizeof(TSS));

    init_descriptor(&gdt[SELECTORTSS>>3],
        SELECTORTSS,
        vir2addr(seg2base(SELECTOR_KERNEL_DS), &tss),
        sizeof(TSS) - 1,
        DA_386TSS);
	tss.io_base = sizeof(TSS);
    tss.ss0 = SELECTOR_KERNEL_DS;
	tss.cs = SELECTOR_KERNEL_CS;
}	

PRIVATE void init_desc_gate(u16 vectorNum, int_handler func, u8 desc_type, u8 privilege_run )
{
	//u32* p_idt_base = (u32*)(&idt_table_ptr[2]);
	GATE* p_idt = &idt[vectorNum];
	// u16 beacuse the limit of gate descriptor is 16 bits
	u32 intOffset = (u32)(*func);
	p_idt->offset_low = intOffset & 0xFFFF;
	p_idt->selector_gate = SELECTOR_KERNEL_CS;
	p_idt->attribute = desc_type | (privilege_run<<5);//this change ,error change one
	p_idt->offset_high = (intOffset>>16) & 0xFFFF;
}

PUBLIC void exception_handler(int vectorNum, int errorCode, int eip, int cs, int eflags)
{
	//this function not deal the interrupte , but it can display the error message, so you can by it debug the kernel.
	// stack_chk_fail_local is you defined temporay variable in functions
	// clk 20 lines to display the exception message
	char enter = '\n'; //由于是字符串赋值，所以最后的结束符号，会被认为成字母t
	int color = 0x74;
	disp_pos = 0;
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<80;j++)
		{
			disp_str(" ");
		}
	}
	// display the exception message
	disp_pos = 0;
	char * err_msg[] = {"#DE Divide Error",
			    "#DB RESERVED",
			    "—  NMI Interrupt",
			    "#BP Breakpoint",
			    "#OF Overflow",
			    "#BR BOUND Range Exceeded",
			    "#UD Invalid Opcode (Undefined Opcode)",
			    "#NM Device Not Available (No Math Coprocessor)",
			    "#DF Double Fault",
			    "    Coprocessor Segment Overrun (reserved)",
			    "#TS Invalid TSS",
			    "#NP Segment Not Present",
			    "#SS Stack-Segment Fault",
			    "#GP General Protection",
			    "#PF Page Fault",
			    "—  (Intel reserved. Do not use.)",
			    "#MF x87 FPU Floating-Point Error (Math Fault)",
			    "#AC Alignment Check",
			    "#MC Machine Check",
			    "#XF SIMD Floating-Point Exception"
	};
	//disp_str_color(err_msg[vectorNum],color);
	disp_str_color("fuck you ! GCC", color);
	disp_int(vectorNum);
	disp_str(&enter);// Enter

	disp_str_color(" ErrorCode: ",color);// display the error code
	disp_int(errorCode);// disp_int error in func
	
	disp_str_color(" EIP: ",color);// display the eip
	disp_int(eip);

	disp_str_color(" CS: ",color);// display the cs
	disp_int(cs);

	disp_str_color(" EFLAGS: ",color);// display the eflags
	disp_int(eflags);
	
	disp_str(&enter);// Enter 
}


