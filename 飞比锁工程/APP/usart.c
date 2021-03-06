#include "usart.h"
#include "global.h"
#include "eeprom.h"

<<<<<<< HEAD
=======
/***********
** 函数名称：wake_up_in()
**功能描述：唤醒模块函数
**入口参数 ：cmd 
SET,为高电平
RESET为低电平
**返回值 
***********/
void wake_up_in(u8 cmd)
{
  if(cmd ==SET)
   GPIO_WriteBit(GPIOC , GPIO_Pin_4 ,SET);  //设置PB0为高电平，唤醒模块
  else
    GPIO_WriteBit(GPIOC , GPIO_Pin_4 ,RESET);  //设置PB0为高电平，唤醒模块
}

>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e

/**********************
** 函数描述: 串口初始化
**参数， baudrate 波特率
**********************/
void UART1_Init(unsigned int baudrate)
{ 
  GPIO_Init(GPIOC, GPIO_Pin_5, GPIO_Mode_Out_PP_High_Fast); //TXD
  GPIO_Init(GPIOC, GPIO_Pin_6, GPIO_Mode_In_PU_No_IT);      //RXD
  
  SYSCFG_REMAPDeInit();
  SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortC , ENABLE); //把USART1串口模块默认的PC5(TX) , PC6(RX)引脚，重映射到PA2(TX) ,PA3(RX)
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1 , ENABLE); //使能USART1时钟
  USART_Init(USART1,                //设置USART1
            baudrate,               //波特率设置
            USART_WordLength_8b,    //数据长度设为8位
            USART_StopBits_1,       //1位停止位
            USART_Parity_No,        //无校验
            (USART_Mode_Rx | USART_Mode_Tx));  //设置为发送接收双模式
  
  USART_ITConfig(USART1, USART_IT_RXNE , ENABLE);
  USART_Cmd(USART1 , ENABLE);
}

/**********************
** 函数描述: 串口重新映射
**参数， baudrate 波特率
**********************/
void UART1_RemapInit(unsigned int baudrate)
{ 
  GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast); //TXD
  GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_Mode_In_PU_No_IT);      //RXD
  
  SYSCFG_REMAPDeInit();
  SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortA , ENABLE); //把USART1串口模块默认的PC5(TX) , PC6(RX)引脚，重映射到PA2(TX) ,PA3(RX)
  
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1 , ENABLE); //使能USART1时钟

  USART_Init(USART1,                //设置USART1
            baudrate,               //波特率设置
            USART_WordLength_8b,    //数据长度设为8位
            USART_StopBits_1,       //1位停止位
            USART_Parity_No,        //无校验
            (USART_Mode_Rx | USART_Mode_Tx));  //设置为发送接收双模式
  
  USART_ITConfig(USART1, USART_IT_RXNE , ENABLE);
   
  USART_Cmd(USART1 , ENABLE);
  
}


/**********************
** 函数描述: 串口发送数据
**参数， char *p 发送的数组, u8 len 发送的字节数
**********************/
<<<<<<< HEAD
void send_hex(u8 *p,u8 len)
{
  u8 i;
  for(i=0;i<len;i++){
    while( (USART1->SR & (uint8_t)USART_FLAG_TXE) == 0);       
    USART_SendData8(USART1 , (u8)p[i]);             //向发送寄存器写入数据     
  }
  while( (USART1->SR & (uint8_t)USART_FLAG_TXE) == 0);   //等待数据发送完成
=======
void send_hex(char *p,u8 len)
{
  u8 i;
  for(i=0;i<len;i++){
    while(USART_GetFlagStatus(USART1 , USART_FLAG_TXE) == 0);       
    USART_SendData8(USART1 , (u8)p[i]);             //向发送寄存器写入数据     
  }
  while(USART_GetFlagStatus(USART1 , USART_FLAG_TXE) == 0);   //等待数据发送完成
>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e
}