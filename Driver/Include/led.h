#ifndef __LED_H
#define __LED_H

#include "project.h"


void led_task(void *pvParameters);
void beep_task(void *pvParameters);
void runtime_task(void* pvParameters);
void interrupt_task(void *pvParameters);
void Timer1Callback(TimerHandle_t xTimer);
void led_toggle(void);


#endif

