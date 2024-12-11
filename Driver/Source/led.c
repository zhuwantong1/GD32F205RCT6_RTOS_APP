#include "project.h"

//LED Task


void led_task(void *pvParameters)
{
	while(1) 
	{
        taskENTER_CRITICAL();

		printf("led on\r\n");
		/*turn on led*/
		//gpio_bit_set(GPIOD, GPIO_PIN_2);
		//gpio_bit_reset(GPIOC, GPIO_PIN_14);
		vTaskDelay(pdMS_TO_TICKS(500));
		taskEXIT_CRITICAL();
    }
}

void beep_task(void *pvParameters)
{
	while(1)
	{
		taskENTER_CRITICAL();
		/* turn off LED */
		printf("led off\r\n");
		//gpio_bit_set(GPIOC, GPIO_PIN_14);
		//gpio_bit_reset(GPIOD, GPIO_PIN_2);
		vTaskDelay(pdMS_TO_TICKS(500));
		taskEXIT_CRITICAL();
	}
	
}


void interrupt_task(void *pvParameters)
{
	while(1)
	{
		taskENTER_CRITICAL();
		judge_message();
		vTaskDelay(pdMS_TO_TICKS(500));
		taskEXIT_CRITICAL();
	}
	
}


void Timer1Callback(TimerHandle_t xTimer)
{
	led_toggle();
}


static FlagStatus flag = SET;
void led_toggle(void)
{
	
	//flag = gpio_output_bit_get(GPIOC,GPIO_PIN_14);
	if(flag == SET)
	{
		printf("timer led set \r\n");
		gpio_bit_set(GPIOC, GPIO_PIN_14);
		flag = RESET;
	}
	else
	{
		printf("timer led reset \r\n");
		gpio_bit_reset(GPIOC, GPIO_PIN_14);
		flag = SET;
	}
}


/* run time statistic */
extern xQueueHandle RECEIVE_Queue_Handler;
void runtime_task(void* pvParameters)
{
		
//	while(1)
//	{
//		taskENTER_CRITICAL();
//		printf("run time items++++++++++++++++++++++++++++++=\r\n");
//		vTaskDelay(pdMS_TO_TICKS(500));
//		taskEXIT_CRITICAL();
//	}	
    while ( 1 )
    {
		uint8_t chr;
		uint16_t items;
		static char runtime_info[10240] ;
		taskENTER_CRITICAL();
		items = uxQueueMessagesWaiting( RECEIVE_Queue_Handler );
		while( items != 0 )
		{
			printf("run time items = %d\r\n",items);
			xQueueReceive( RECEIVE_Queue_Handler, &chr, 0 );
			items--;
			if(  chr == 0xBB )
			{
				//runtime_info = mymalloc(SRAMIN, 100);
				vTaskGetRunTimeStats(runtime_info);             /* get task runing time information */
				printf("task_name\truning_time\tDuty_cycle\r\n");
				printf("%s\t\r\n", runtime_info);
				//myfree(SRAMIN, runtime_info);
				break;
			}
			else
			{
				printf("Unable to track operational status \r\n");
			}	
		}
		vTaskDelay(pdMS_TO_TICKS(500));
		taskEXIT_CRITICAL();
    }
	
}

