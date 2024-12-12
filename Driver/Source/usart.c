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


void init_com0(usart_struc_def  com_struct)
{
    com_struct->baudval			= 115200;				/* bound rate */
    com_struct->usart_periph	= USART0;				/* periph usart */
	com_struct->wlen			= USART_WL_8BIT;			/* data length */
	com_struct->nvic_irq			= USART0_IRQn;			/* nvic_irq */
	com_struct->stblen			= USART_STB_1BIT;		/* stop bits */
	com_struct->paritycfg		= USART_PM_NONE;			/* parity check bit */
	com_struct->ctsconfig		= USART_CTS_DISABLE;		/* hardware flow cts control*/
	com_struct->rtsconfig		= USART_RTS_DISABLE;		/* hardware flow rts control*/
	com_struct->interrupt  		= USART_INT_RBNE;		/* Receive Buffer Not Empty flag */
	
	/* Hardware pin initialization */
	/* enable GPIO clock */
	rcu_periph_clock_enable(RCU_GPIOA);
	/* enable USART clock */
	rcu_periph_clock_enable(RCU_USART0);
	/* connect port to USART0 Tx */
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	/* connect port to USART0 Rx */
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
	
	
}

void init_com2(usart_struc_def  com_struct)
{
	com_struct->baudval			= 115200;
	com_struct->usart_periph		= USART2;
	com_struct->wlen			= USART_WL_8BIT;
	com_struct->nvic_irq			= USART2_IRQn;
	com_struct->stblen			= USART_STB_1BIT;
	com_struct->paritycfg		= USART_PM_NONE;
	com_struct->ctsconfig		= USART_CTS_DISABLE;
	com_struct->rtsconfig		= USART_RTS_DISABLE;
	com_struct->interrupt  		= USART_INT_RBNE;
	
	
	/* Hardware pin initialization */
	/* enable GPIO clock */
	rcu_periph_clock_enable(RCU_GPIOB);
	/* enable USART clock */
	rcu_periph_clock_enable(RCU_USART2);
	/* connect port to USART0 Tx */
	gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
	/* connect port to USART0 Rx */
	gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
}

void usart_config( usart_struc_def  com_struct)
{
    /* USART configure */
    usart_deinit(com_struct->usart_periph);
    usart_baudrate_set(com_struct->usart_periph, com_struct->baudval);
    usart_word_length_set(com_struct->usart_periph, com_struct->wlen);
    usart_stop_bit_set(com_struct->usart_periph, com_struct->stblen);
    usart_parity_config(com_struct->usart_periph, com_struct->paritycfg);
    usart_hardware_flow_rts_config(com_struct->usart_periph, com_struct->rtsconfig);
    usart_hardware_flow_cts_config(com_struct->usart_periph,com_struct->ctsconfig );
    usart_receive_config(com_struct->usart_periph, USART_RECEIVE_ENABLE);
    usart_transmit_config(com_struct->usart_periph, USART_TRANSMIT_ENABLE);
    usart_enable(com_struct->usart_periph); 
    /* enable usart2 receive interrupt */
    usart_interrupt_enable(com_struct->usart_periph, com_struct->interrupt);
    /* config NVIC interrupt priority and enable interrupt */
    nvic_irq_enable(com_struct->nvic_irq, RECEIVE_BUF_PRIORITY, RECEIVE_BUF_PRIORITY);
}

void init_receive_queue(void)
{
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
    //usart_data_receive(USART0);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t)ch);
    while (RESET == usart_flag_get(USART0, USART_FLAG_TBE))
        ;
    return ch;
}

int fgetc(FILE *f)
{
    uint8_t ch = 0;
    ch = usart_data_receive(USART0);
    return ch;
}

// USART0 interrupt function
void USART0_IRQHandler(void)
{
    uint8_t Res;   
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	
    if (usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE) != RESET)
    {
		
		/* clear flag of interrupt */
		usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);
        Res = usart_data_receive(USART0);
        xQueueSendToBackFromISR(RECEIVE_Queue_Handler, &Res, &xHigherPriorityTaskWoken );
        if(xHigherPriorityTaskWoken == pdTRUE)
        {
			taskYIELD();
        }
    }
}

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
