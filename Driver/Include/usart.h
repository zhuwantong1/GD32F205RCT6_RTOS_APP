#ifndef USART_H
#define USART_H

#include "project.h"
#define USART_REC_LEN  			200         //received bytes max



extern uint8_t  USART_RX_BUF[USART_REC_LEN]; //received buf  
extern uint16_t USART_RX_STA;         		//received state flag
extern uint32_t USART_RX_CNT;				//received bytes count
extern uint8_t  CodeUpdateFlag;



typedef struct usart_struct_para{
    uint32_t usart_periph;          /* comx:x = 1,2,3 */
    uint32_t baudval;
    uint32_t wlen;
	uint8_t  nvic_irq;
	uint32_t stblen;
	uint32_t paritycfg;
	uint32_t rtsconfig;
	uint32_t ctsconfig;
	usart_interrupt_enum interrupt;
	
}usart_struct;


typedef usart_struct* usart_struc_def;

void init_receive_queue(void);
void init_com0(usart_struc_def  com_struct);
void init_com2(usart_struc_def  com_struct);
void usart_config(usart_struc_def  com_struct);
#endif

