#include "project.h"

extern xQueueHandle RECEIVE_Queue_Handler;

/* judge the received message */
void judge_message(void)
{
	uint8_t chr;
	uint16_t items;
	items = uxQueueMessagesWaiting( RECEIVE_Queue_Handler );
	while( items != 0 )
	{
		printf("items = %d\r\n",items);
		xQueueReceive( RECEIVE_Queue_Handler, &chr, 0 );
		items--;
		if(  chr == 0xAA )
		{
			printf("correct message \r\n");
		}
		else
		{
			printf("error message \r\n");
		}

	}	
	//xQueueReset(RECEIVE_Queue_Handler);
	//memset(USART_RX_BUF,0,20);
}
