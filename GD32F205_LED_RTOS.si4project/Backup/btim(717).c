#include "project.h"

timer_parameter_struct g_tim1_handle;		/* 定时器1句柄 */

uint32_t FreeRTOSRunTimeTicks;			/* FreeRTOS 时间统计的计数器 */
	 
void configureTimeForRunTimeStats(void)
{

	FreeRTOSRunTimeTicks = 0;
	/* init tim1 */
	
}


void 