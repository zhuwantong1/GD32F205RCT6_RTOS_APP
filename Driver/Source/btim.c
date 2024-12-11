#include "project.h"



uint32_t FreeRTOSRunTimeTicks;			/* FreeRTOS 时间统计的计数器 */
	 
void configureTimeForRunTimeStats(void)
{

	FreeRTOSRunTimeTicks = 0;
	/* init tim1 */
	btim_tim1_int_init(10-1, 1200-1);
}

/*	定时器1的定时中断初始化
*	arr:自动重装值
*	psc:时钟预分频数
*/
void btim_tim1_int_init(uint16_t arr, uint32_t psc)
{
	timer_parameter_struct 			g_tim1_handle;		/* 定时器1句柄 */
	/* 配置tim1 */
	g_tim1_handle.prescaler 			= arr;
	g_tim1_handle.alignedmode 		= TIMER_COUNTER_EDGE;
	g_tim1_handle.counterdirection	= TIMER_COUNTER_UP;
	g_tim1_handle.period				= psc;
	g_tim1_handle.clockdivision		= TIMER_CKDIV_DIV1;
	g_tim1_handle.repetitioncounter	= 0;
	
	rcu_periph_clock_enable(RCU_TIMER1); 				/* 开启定时器时钟 */
	timer_init( TIMER1, &g_tim1_handle );			/* 初始化定时器1 */
	timer_auto_reload_shadow_enable(TIMER1);			/* 使能自动重加载 */
	timer_interrupt_enable(TIMER1, TIMER_INT_UP);		/* 使能溢出中断 */
	nvic_irq_enable(TIMER1_IRQn, 2, 0);				/* 配置中断优先级 */	
	timer_enable(TIMER1);							/* 使能定时器tim1 */
}

/* 定时器1的中断函数 */
void TIMER1_IRQHandler( void )
{
//	led_toggle();
	FreeRTOSRunTimeTicks++;
	timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);
}


