#include "project.h"

/* Define The Queue Parameters */
#define RECEIVE_BUF_PRIORITY				3
#define RECEIVE_BUF_Bytes				1024
xQueueHandle RECEIVE_Queue_Handler;

/* receive buf */
uint8_t USART_RX_BUF[USART_REC_LEN] ; 

uint16_t USART_RX_STA = 0;  // flag of receive state 
uint32_t USART_RX_CNT = 0;  // received bytes
uint8_t CodeUpdateFlag = 0; // update flag of app code 

void usart_config(void)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOB);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART2);

    /* connect port to USART2 Tx */
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* connect port to USART2 Rx */
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);

    /* USART configure */
    usart_deinit(USART2);
    usart_baudrate_set(USART2, 115200);
    usart_word_length_set(USART2, USART_WL_8BIT);
    usart_stop_bit_set(USART2, USART_STB_1BIT);
    usart_parity_config(USART2, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART2, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART2, USART_CTS_DISABLE);
    usart_receive_config(USART2, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);
    usart_enable(USART2);
    /* enable usart2 receive interrupt */
    usart_interrupt_enable(USART2, USART_INT_RBNE);
    /* config NVIC interrupt priority and enable interrupt */
    nvic_irq_enable(USART2_IRQn, RECEIVE_BUF_PRIORITY, RECEIVE_BUF_PRIORITY);

	RECEIVE_Queue_Handler = xQueueCreate(RECEIVE_BUF_Bytes, sizeof(uint8_t));
	if(RECEIVE_Queue_Handler == NULL)
	{
		printf("receive queue handler create error \r\n");
		while(1)
		{
		}
	}
	else
	{
		printf("receive queue handler create right \r\n");
	}
    xQueueReset(RECEIVE_Queue_Handler);
    usart_data_receive(USART2);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART2, (uint8_t)ch);
    while (RESET == usart_flag_get(USART2, USART_FLAG_TBE))
        ;
    return ch;
}

int fgetc(FILE *f)
{
    uint8_t ch = 0;
    ch = usart_data_receive(USART2);
    return ch;
}
uint8_t Finish_Flag = 0;
// USART2 interrupt function
void USART2_IRQHandler(void)
{
    uint8_t Res;   
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	
    if (usart_interrupt_flag_get(USART2, USART_INT_FLAG_RBNE) != RESET)
    {
		
		/* clear flag of interrupt */
		usart_interrupt_flag_clear(USART2, USART_INT_FLAG_RBNE);
        Res = usart_data_receive(USART2);
        xQueueSendToBackFromISR(RECEIVE_Queue_Handler, &Res, &xHigherPriorityTaskWoken );
        if(xHigherPriorityTaskWoken == pdTRUE)
        {
			taskYIELD();
        }
    }
}
