#include "type.h"
#include "const.h"
#include "protect.h"
#include "process.h"
#include "global.h"
#include "proto.h"


// iota 原理， 传入数值未经处理，不是字符码，需要进行转换
PUBLIC char* iota(char* info, int number)
{
	int i = 0;
	info[i] = '0';
	i += 1;
	info[i] = 'x';
	i += 1;
	info[i] = '0';
	i += 1;
	// 0x

	int temp = number;
	for(int j=28;j>=0;j-=4)
	{
		temp = (temp>>j)&0x0000000F;
		if(temp <= 9)
		{
			temp += '0';
			info[i] = temp;
		}
		else
		{
			temp += 'A';
			temp -= 10;
			info[i] = temp;
		}
		i += 1;
		temp = number;
	}
	info[i] = 0;
	return info;
}

PUBLIC void disp_int(int number)
{
	char numBuffer[32];
	char* intInfo = iota(numBuffer, number);
	disp_str(intInfo);
}

PUBLIC void delay(int num){
    for(int i=0;i <= num;i++)
    {
        for(int j=0;j<100;j++)
        {
            for(int k=0;k<1000;k++)
            {
                
            }
        }
    }
}
