#ifndef __BSP_H
#define __BSP_H
#include "stm8l15x.h"

#define Nan_Jing

#define ZIGBEE_BAUD 19200


void wake_up_in(u8 GPIO_BitVal);
void config_wake_up_out(u8 GPIO_Mode);
u8 get_wakeup_out_pin_state();   //��ȡwakeup_out pin��״̬
u8 get_lock_rx_pin();  //��ȡ��ģ��rx����
u8 config_lock_rx_pin(u8 GPIO_Mode);  //��ȡ��ģ��rx����
void config_lock_tx_pin(u8 GPIO_Mode);
void set_lock_tx_pin(u8 GPIO_BitVal);
void TIM2_Init(u16 prescaler);
void IOInit();
void init_pin_interrupt(void);
u8 zigbee_usart_send(void);
void LED2(u8 GPIO_BitVal);


#endif 