#include "type.h"
#include "const.h"
#include "protect.h"
#include "process.h"
#include "global.h"
#include "string.h"
#include "proto.h"

PUBLIC void clock_handler(int irq)
{
    disp_str("^");
    ticks++;
    if(k_reenter!=0)
    {
        disp_str("!");
        return;
    }
    proc_ready_ptr ++;
    if(proc_ready_ptr >= (proc_table + 3))
    {
        proc_ready_ptr = proc_table;
    }
}