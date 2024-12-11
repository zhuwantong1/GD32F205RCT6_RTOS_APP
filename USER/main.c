#include "project.h"

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/

/* start task parameters */
#define START_TASK_PRIORITY			1						
#define START_STACK_SIZE				128
TaskHandle_t 						StartTask_Handler;
void start_task(void *pvParameters);


/* led task parameters */
#define LED_TASK_PRIORITY			3
#define LED_STACK_SIZE				4*256
TaskHandle_t 						LED_TaskHandler;


/* beep task parameters */
#define BEEP_TASK_PRIORITY			3
#define BEEP_STACK_SIZE				4*256
TaskHandle_t							BEEP_TaskHandler;


/* received interrupt task */
#define INTERRUPT_TASK_PRIORITY		3
#define INTERRUPT_STACK_SIZE			4*256
TaskHandle_t							INTERRUPT_TaskHandler;


/* run time statistic */
#define RUN_TIME_TASK_PRIORITY		4
#define RUN_TIME_STACK_SIZE  		4*256                 
TaskHandle_t            				RUNTIME_TaskHandler;  


/* led timer */
static uint32_t RuningLEDTimerInterval = pdMS_TO_TICKS(500);
TimerHandle_t 	Timer1Timer_Handler;		/* define timer handle */
void Timer1Callback(TimerHandle_t xTimer);


void start_task(void *pvParameters)
{
	
	taskENTER_CRITICAL(); 
	BaseType_t ret;
	/* creat timer_task before creat task_task */
	Timer1Timer_Handler = xTimerCreate((const char*	)"Timer1",									/* timer name */
									  (TickType_t	)RuningLEDTimerInterval,					/* timer timeout period */
									  (UBaseType_t	)pdTRUE,									/* cycle timer */
									  (void*			)1,											/* timer id */
								      (TimerCallbackFunction_t)Timer1Callback);					/* timer callback function */
	if(Timer1Timer_Handler == NULL)
	{
		printf("Failed to create timer1_task \r\n");
	}
	else
	{
		printf("succeed to create timer1_task \r\n");
	}
	ret = xTaskCreate((TaskFunction_t	)led_task,
						(const char*		)"led",
						(uint16_t 		)LED_STACK_SIZE,
						(void*			)NULL,
						(UBaseType_t		)LED_TASK_PRIORITY,
						(TaskHandle_t*	)&LED_TaskHandler);
	if(ret == NULL)
	{
		printf("Failed to create led task \r\n");
	}
	else
	{
		printf("succeed to create led task \r\n");
	}
	ret = xTaskCreate((TaskFunction_t	)beep_task,
						(const char*		)"beep",
						(uint16_t		)BEEP_STACK_SIZE,
						(void*			)NULL,
						(UBaseType_t		)LED_TASK_PRIORITY,
						(TaskHandle_t*	)&BEEP_TaskHandler);			
	if(ret == NULL)
	{
		printf("Failed to create beep_task \r\n");
	}
	else
	{
		printf("succeed to create beep_task \r\n");
	}
	ret = xTaskCreate((TaskFunction_t )interrupt_task,
					(const char*		)"interrupt",
					(uint16_t		)INTERRUPT_STACK_SIZE,
					(void*			)NULL,
					(UBaseType_t		)INTERRUPT_TASK_PRIORITY,
					(TaskHandle_t*	)&INTERRUPT_TaskHandler);
	if(ret == NULL)
	{
		printf("Failed to create interrupt_task \r\n");
	}
	else
	{
		printf("succeed to create interrupt_task \r\n");
	}
	ret = xTaskCreate((TaskFunction_t )runtime_task,
					(const char*		)"runtime",
					(uint16_t		)RUN_TIME_STACK_SIZE,
					(void*			)NULL,
					(UBaseType_t		)RUN_TIME_TASK_PRIORITY,
					(TaskHandle_t*	)&RUNTIME_TaskHandler);
	if(ret == NULL)
	{
		printf("Failed to create runtime_task \r\n");
	}
	else
	{
		printf("succeed to create runtime_task \r\n");
	}
	
	xTimerStart((TimerHandle_t	)Timer1Timer_Handler,
				(TickType_t		)portMAX_DELAY);
	vTaskDelete(StartTask_Handler);											/* delete start_task */
	taskEXIT_CRITICAL(); 
	
}

int main(void)
{
	BaseType_t result = NULL;			
	SCB->VTOR = FLASH_BASE | 0x8000;
	/* NVIC */
    nvic_priority_group_set( NVIC_PRIGROUP_PRE4_SUB0 );
    /* configure systick */
	systick_config();
	gpio_config();
	usart_config();
	printf("start_config!!!\r\n");
	/* creat task of start */
	result = xTaskCreate((TaskFunction_t)start_task,
						(const char*		)"start",
						(uint16_t 		)START_STACK_SIZE,
						(void*			)NULL,
						(UBaseType_t		)START_TASK_PRIORITY,
						(TaskHandle_t*	)&StartTask_Handler);
	if(result == NULL)
	{
		printf("Failed to create Start task\r\n");
	}
	else
	{
		printf("succeed to create Start task\r\n");
	}
	vTaskStartScheduler();
}
