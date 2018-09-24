#ifndef __USART_H
#define __USART_H
#include "stm8l15x.h"
#include "global.h"
<<<<<<< HEAD
void UART1_Init(unsigned int baudrate);
void UART1_RemapInit(unsigned int baudrate);
void send_hex(u8 *p,u8 len);
 
 
=======
extern u8 key[];
extern u8 password[6];
extern u8 npass[6];
extern u8 data_buff[50];
 void wake_up_in(u8 cmd);
 void encryption(void);
 u8 crc_8n(unsigned char *ptr,unsigned char n);

>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e
#endif