#include "type.h"
#include "const.h"
#include "protect.h"
#include "process.h"
#include "proto.h"
#include "global.h"

void Init8259A()
{
	// ICW1 master
	out_byte(INT_M_CTL,0x11);
	// ICW1 slave
	out_byte(INT_S_CTL,0x11);
	// ICW2 master
	out_byte(INT_M_CTLMASK,INT_VECTOR_IRQ0);
	// ICW2 slave
	out_byte(INT_S_CTLMASK,INT_VECTOR_IRQ8);
	// same as top
	// ICW3
	out_byte(INT_M_CTLMASK,0x04);
	out_byte(INT_S_CTLMASK,0x02);
	// ICW4
	out_byte(INT_M_CTLMASK,0x01);
	out_byte(INT_S_CTLMASK,0x01);
	// OCW1
	out_byte(INT_M_CTLMASK,0xff);
	out_byte(INT_S_CTLMASK,0xff);
	// set irq_table value
	for(int i=0;i<16;i++)
	{
		irq_table[i]=(irq_hander)irq_action;
	}
}

PUBLIC void irq_action(u8 hard_vector, u32 cs, u32 eip, u32 eflags)
{
	disp_pos = 0;
	disp_str("keyboard response only!");
	disp_str(" vector_number: ");
	disp_int(hard_vector);
}



