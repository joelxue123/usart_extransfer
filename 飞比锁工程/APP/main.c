#include "stm8l15x.h"
#include "global.h"
#include "eeprom.h"
#include "usart.h"
#include "protocol.h"
<<<<<<< HEAD
#include "command.h"
#include "bsp.h"


u8 G_receive_data[USART_LEN]={0};
u8 G_send_data[60]={0};
=======

u8 key[]={0x46,0x45,0x49,0x42,0x47};
u8 password[6]={1,2,3,4,5,6};
u8 npass[6];
u8 data_buff[50]={0};

u8 G_receive_data[100]={0xAA ,0x0A, 0x40, 00, 00, 00, 0x01, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 0xE1, 0x55};
u8 G_send_data[100]={0xAA ,0x0A, 0x40, 00, 00, 00, 0x01, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 0xE1, 0x55};
>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e

BFCT_protocol BFCT_protocol_Zigbee;
BFCT_protocol BFCT_protocol_Lock;

u8 continue_usart_data=0;  /* ������־λ */
<<<<<<< HEAD
u16 data3_tc;
=======
u8 data3_tc;
>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e
u8 timeout_flag;
u8 t_1ms;

u8 zigbee_send_cmd;

u8 lock_interrupt =0;
u8 zigbee_interrupt =0;

u8 Zigbee_process_done =1;
u8 Lock_process_done =1;

u8 Zigbee_processing_flag =0;
u8 Lock_processing_flag =0;

<<<<<<< HEAD
u8 lock_state =0;
u8 zigbee_state =0;

u8 zigbee_erro=0;
u8 lock_erro = 0;

/**********************
** ��������: ���
**********************/
void led(void)
{
#if 1
   static u8 led_on=0;
   static  u16 led_timer=0;
   if(t_1ms)
   {
    led_timer++;
    
    if(led_timer == 1000){
      led_on=1-led_on;
      led_timer=0;
    }
    if(led_on)
       LED2(RESET); //���� LED
    else
       LED2(SET);   //�ر�LED
   }
#endif 
}


/**********
**�������� �� ʱ�������,1ms����ж�
**********/
void os_time(void) 
{
    if( ( TIM2_GetFlagStatus(TIM2_FLAG_Update) != RESET) ) //�ȴ������Ƿ�ﵽ1����
    {
      t_1ms=1;
      TIM2_ClearFlag(TIM2_FLAG_Update);
    }else t_1ms=0;
}
=======
u8 Lock_process_timeout=0;
u8 Zigbee_process_timeout=0;
u8 lock_state =0;
u8 zigbee_state =0;
u8 zigbee_moni_state=0;
u8 lock_moni_state=0;

/**********
**�������� �� �Ͼ�ģ�� �ظ� ACK ����
**********/
u8 zigbee_usart_send(void)
{
    static u16 delay=0;
    if(t_1ms){
      delay++;
      if(delay ==2)
          wake_up_in(SET);
      if(delay ==20)   
          wake_up_in(RESET);
      if(delay ==40)
      {
        enableInterrupts();    //�ƺ�ûɶ�ã�������Ϊɶ�ŵ�������
        UART1_Init(ZIGBEE_BAUD); //PC5,PC6Ϊ����,��zigbeeģ��ͨѶ
        wake_up_in(SET);
        send_hex(BFCT_protocol_Zigbee.send_data,BFCT_protocol_Zigbee.send_len); // ��������

      } 
      if(delay ==42)
      {
        delay =0;
        BFCT_protocol_Zigbee.receive_enable =1; //ʹ��zigbeeģ�� ���չ���
        BFCT_protocol_Lock.receive_enable =0;   //ʧ��lock���ڽ��� 
        BFCT_protocol_Zigbee.receive_len =0;
        BFCT_protocol_Zigbee.receive_flag =0;
        return 1;
      }
      if(delay > 100)
      {
        delay =0;
      }
    }  
   return 0;
}  
>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e

/**********************
*�����ͺ���
*
************************/
u8 lock_usart_send(void)
{
   static u16 delay=0;

   
    if(t_1ms){
       delay++;
       if(1==delay)
       {
          USART_Cmd(USART1 , DISABLE);
<<<<<<< HEAD
          GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast); //TXD
          GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_Mode_In_PU_No_IT);      //RXD
          
            GPIOA->ODR &= (uint8_t)(~GPIO_Pin_2);//���ô���Ϊ�͵�ƽ
=======
          SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortC , DISABLE); //��USART1����ģ��Ĭ�ϵ�PC5(TX) , PC6(RX)����
          SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortA , DISABLE); //��USART1����ģ��Ĭ�ϵ�PC5(TX) , PC6(RX)���ţ���ӳ�䵽PA2(TX) ,PA3(RX)
          CLK_PeripheralClockConfig(CLK_Peripheral_USART1 , DISABLE); //ʹ��USART1ʱ��
          GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast); //TXD
          GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_Mode_In_PU_No_IT);      //RXD
          
          GPIO_WriteBit(GPIOA , GPIO_Pin_2 ,RESET);  //����PA2,������ͨѶ
>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e
       }
       
       if(20 ==delay)
       {
<<<<<<< HEAD
          GPIOA->ODR |= (uint8_t)GPIO_Pin_2;//���ô���Ϊ�ߵ�ƽ
=======
          GPIO_WriteBit(GPIOA , GPIO_Pin_2 ,SET);  //����PA2,������ͨѶ
>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e
       } 
       if(delay < 1000 && delay > 20)
       {
          if(  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3) )
          {
              UART1_RemapInit(9600);//������ ���ó� 9600
              send_hex(BFCT_protocol_Lock.send_data,BFCT_protocol_Lock.send_len); // ��������
              BFCT_protocol_Lock.receive_enable =1;
              delay =0;
              for(delay=0;delay<1000;delay++);
<<<<<<< HEAD
              
              USART1->CR2 &= (uint8_t)~(USART_CR2_TEN);
              GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast); //TXD
=======
>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e
              delay=0;  
              return 1;
          }
       }
       if(delay > 1000)
       {
          delay=0;
          return 2;
       }
    }
       return 0;
}

/**********
**�������� �� ��zigbee�յ�������Э�� ת�г� ��Э��
**********/
u8 zigbee_convert2lock(void)
{
<<<<<<< HEAD
    u16 data_len;
    u8 ret;
    
=======
    u8 i;
    u16 data_len;
    u8 lock_cmd=0��
    static u16 delay=0;
    static u8 erro_num=0;
    static u8 check_timeout=0;
>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e
    /************
    **�Ͼ�ģ��,Э��Ӧ���ʽ

    ***************/
    /****
    ��zigbee����������ݣ���䵽lock����������
    ****/
  
  switch(zigbee_state)
  {
    case 0:
<<<<<<< HEAD
        BFCT_protocol_Zigbee.receive_flag =0;
        BFCT_protocol_Lock.receive_enable =0;  //�ر������ڽ��ܺ���
        BFCT_protocol_Zigbee.receive_enable =0; //�ر�zigbee���ڽ��ܺ���
        
        BFCT_protocol_Zigbee.command=BFCT_protocol_Zigbee.receive_data[ZIGBEE_CMD_ADDR]; //��ȡ��������
        if( check_code(BFCT_protocol_Zigbee.receive_data) )
        { /**********����У����� ************/
          data_len =0 ;
#if  DEBUG
          zigbee_erro =  zigbee_erro | sum_check_BIT;
#endif 
          zigbee_state =10;
        }else {
         zigbee_state ++;
        }
=======
        BFCT_protocol_Lock.receive_enable =0;  //�ر������ڽ��ܺ���
        BFCT_protocol_Zigbee.receive_enable =0; //�ر�zigbee���ڽ��ܺ���
        
        data_len = (BFCT_protocol_Zigbee.receive_data[2] << 8) + BFCT_protocol_Zigbee.receive_data[3];
        BFCT_protocol_Zigbee.command=BFCT_protocol_Zigbee.receive_data[ZIGBEE_CMD_ADDR]; //��ȡ��������
        if(BFCT_protocol_Zigbee.receive_data[data_len +3] != sum_check(BFCT_protocol_Zigbee.receive_data,data_len +3) )
        { /**********����У����� ************/
          data_len =0 ;
          zigbee_erro =  zigbee_erro | sum_check_BIT;
          return 1;
        }
        zigbee_state ++;
>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e
    break;
    case 1:
        ret = exe_zigbee_cmd(BFCT_protocol_Zigbee.command);
        if(ret)
        {
<<<<<<< HEAD
            zigbee_state =10;
        }
    break;

  case 10:
       Zigbee_process_done =1;
       zigbee_state =0;
=======
          if(zigbee_erro ==0 && lock_erro==0 )
          {
            zigbee_state =10;
          }
          else
          {
            while(1);
          }
        }
    break;

    case 10��
       Zigbee_process_done =1;
       zigbee_state =0;
       delay =0;
>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e
       return 1;
     
      break;
    default: 
    break;
    
  }
  
  return 0;
}


/***
**�����ж�������Э�飬ת����zigbeeģʽ
**/
u8 lock_convert2zigbee(void)
{
  u16 data_len;
<<<<<<< HEAD
  u8 ret;
=======
  static u16 delay=0;
  static u32 cmd_id=1;
  u8 data_buff[20]={0};
  static u8 erro_num=0;
>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e

  switch(lock_state )
  {
    case 0:
<<<<<<< HEAD
        BFCT_protocol_Lock.receive_flag =0; 
        BFCT_protocol_Lock.receive_enable =0;  //�ر������ڽ��ܺ���
        BFCT_protocol_Zigbee.receive_enable =0; //�ر�zigbee���ڽ��ܺ���
        data_len = BFCT_protocol_Lock.receive_data[1] + (BFCT_protocol_Lock.receive_data[2] << 8); //��ȡ���ݳ���
        BFCT_protocol_Lock.command=BFCT_protocol_Lock.receive_data[LOCK_CMD_ADDR]; //��ȡ��������
        if(BFCT_protocol_Lock.receive_data[data_len +3] != crc_8n(BFCT_protocol_Lock.receive_data,data_len +3) )
        {    /** ����У�����**/
          data_len =0 ;
          lock_state = 10;
        }
        else {
          data_len =0;
         lock_state ++;
        }
    break;
   
   case 1:
      ret = exe_lock_cmd(BFCT_protocol_Lock.command);
      if(ret)
      {
          lock_state =10;
=======
        BFCT_protocol_Lock.receive_enable =0;  //�ر������ڽ��ܺ���
        BFCT_protocol_Zigbee.receive_enable =0; //�ر�zigbee���ڽ��ܺ���
        data_len = BFCT_protocol_Lock.receive_data[1] + (BFCT_protocol_Lock.receive_data[2] << 8); //��ȡ���ݳ���
        lock_cmd=BFCT_protocol_Lock.receive_data[LOCK_CMD_ADDR]; //��ȡ��������
        if(BFCT_protocol_Lock.receive_data[data_len +3] != crc_8n(BFCT_protocol_Lock.receive_data,data_len +3) )
        {    /** ����У�����**/
          data_len =0 ;
          lock_erro =  lock_erro | crc_8n_ERRO_BIT;
          return 1;
        }
        data_len =0;
        lock_state ++;
    break;
    

   case 2:
      ret = exe_lock_cmd(lock_cmd);
      if(ret)
      {
      if(zigbee_erro ==0 && lock_erro==0 )
      {
        lock_state =10;
      }
      else
      {
        while(1);
      }
>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e
      }
     break;
      
    case 10:
      lock_state =0;
<<<<<<< HEAD
=======
      delay =0;
      sys_timer = 999;
>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e
      Lock_process_done=1;
      return 1;
    break;
    
    default:
    break;
  }
  return 0;
}

/**********
**�������� �� ����zigbee���ݴ���
**********/
void USART_process(void)
{
<<<<<<< HEAD
    u8 res;
=======
    u16 DataLen;
    u8 res;
    static u8 delay=0;
>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e
/********************
    *���ڳ�ʱ��������
    **********************/
    if( t_1ms==1 ) //10ms��ʱ��,��ʱ��ÿ10ms,����˺���
    {
      data3_tc++;
<<<<<<< HEAD
      if( data3_tc > 101 ) //��ʱ���ӵ� 300ms,ֹͣ����
      {
        data3_tc=101;  //��Ϊȫ�ֱ�������Ҫ����ʹ��
      }
      if( data3_tc == 100 ) //��ʱ280ms��ʱ��,��ʾ������ֹ
      {
        if( (BFCT_protocol_Zigbee.receive_enable == 1) && BFCT_protocol_Zigbee.receive_flag != 1)
           BFCT_protocol_Zigbee.receive_flag = 1;
        if( (BFCT_protocol_Lock.receive_enable == 1) && BFCT_protocol_Lock.receive_flag != 1)
           BFCT_protocol_Lock.receive_flag = 1;          
=======
      if( data3_tc > 5 ) //��ʱ���ӵ� 300ms,ֹͣ����
      {
        data3_tc=5;  //��Ϊȫ�ֱ�������Ҫ����ʹ��
      }
      if( data3_tc == 4 ) //��ʱ280ms��ʱ��,��ʾ������ֹ
      {
        if( (BFCT_protocol_Zigbee.receive_enable == 1) && (BFCT_protocol_Zigbee.receive_flag != 1)
           BFCT_protocol_Zigbee.receive_flag = 1;
        if( (BFCT_protocol_Lock.receive_enable == 1) && (BFCT_protocol_Lock.receive_flag != 1)
           BFCT_protocol_Zigbee.receive_flag = 1;          
>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e
        
      }
    }
/****************
 *�жϴ��ڽ�����ɽ������
*****************/
    if(BFCT_protocol_Zigbee.receive_flag == 1)
    { 
<<<<<<< HEAD
  //    USART_Cmd(USART1 , DISABLE);
      data3_tc =101;
      if(Lock_processing_flag ==0)/****ֻ����һ��Э�����������******/
      {
        Zigbee_processing_flag =1;  //����zigbee����  
//       BFCT_protocol_Zigbee.receive_flag =0;
=======
      data3_tc =5;
      if(Lock_processing_flag ==0)/****ֻ����һ��Э�����������******/
      {
        Zigbee_processing_flag =1;  //����zigbee����  
        BFCT_protocol_Zigbee.receive_flag =0;
>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e
      }
    }
    else if(BFCT_protocol_Lock.receive_flag==1)
    {
<<<<<<< HEAD
      USART1->CR2 &= (uint8_t)~(USART_CR2_TEN);
      GPIO_Init(GPIOA,GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast); //PA2,tx,�͵�ƽ���
      data3_tc =101;
      if(Zigbee_processing_flag ==0)/****ֻ����һ��Э�����������******/
      {
        Lock_processing_flag  = 1; //��������������
      //  BFCT_protocol_Lock.receive_flag =0; 
=======
      data3_tc =5;
      if(Zigbee_processing_flag ==0)/****ֻ����һ��Э�����������******/
      {
        Lock_processing_flag  = 1; //��������������
        BFCT_protocol_Lock.receive_flag =0; 
>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e
      }
    }  

/****************
 *Zigbee����������
*****************/
    if(Zigbee_processing_flag ==1)
    {
     res = zigbee_convert2lock();
     if(res==1)
     {
       if(zigbee_erro)
       {
<<<<<<< HEAD
       //  while(1);
=======
         sys_timer =999;
>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e
       }
        Zigbee_processing_flag =0;
        Zigbee_process_done =1;
     }
    }
/****************
 *������������
*****************/
    if(Lock_processing_flag ==1)
    {
      res =lock_convert2zigbee();
      if(res==1)
      {
       if(lock_erro)
       {
<<<<<<< HEAD
         Lock_process_done =1;
       }
        Lock_processing_flag=0;
=======
         sys_timer =999;
       }
        Lock_processing_flag=1;
>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e
        Lock_process_done=1;
      }
    }
/**************
    *�����ڷ��ͺ���
***********/
    if(BFCT_protocol_Lock.send_len)
    {
      res = lock_usart_send();
     if(res==1)
     {
      BFCT_protocol_Lock.send_len =0;
     }  
     else if(res ==2)
     {
      BFCT_protocol_Lock.send_fail=1;
      BFCT_protocol_Lock.send_len =0;
     }
    }
/**************
    *zigbee���ڷ��ͺ���
***********/
    if(BFCT_protocol_Zigbee.send_len)
    {
      res =zigbee_usart_send();
     if(res ==1)
     {
      BFCT_protocol_Zigbee.send_len =0;
     }  
    }
}
             
/*********���������� *****************/             

void main()
{
<<<<<<< HEAD
  volatile  u16 i,j;
  static  u16 delay=0;
  static u16 sys_timer=0;
  disableInterrupts();   //�ر�ϵͳ���ж�
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1); //�ڲ�ʱ��Ϊ1��Ƶ = 16Mhz 
 
  for(i=0;i<100;i++)
    for(j=0;j<1000;j++);

  IOInit();
  TIM2_Init(16000);
  TIM2->CR1 |= TIM_CR1_ARPE;  //ʹ�ܶ�ʱ��2�Զ����ع���    
  TIM2->CR1 |= TIM_CR1_CEN;

  USART1->CR2 &= (uint8_t)~(USART_CR2_TEN);
  GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast); //TXD
  enableInterrupts();   //ʹ��ϵͳ���ж�

  BFCT_protocol_Zigbee.receive_data = G_receive_data ;
  BFCT_protocol_Zigbee.send_data = G_send_data ;
  

  BFCT_protocol_Lock.receive_data = G_receive_data ;
  BFCT_protocol_Lock.send_data = G_send_data ;
  
  data3_tc =101;
=======
   static  u16 timer=0;
   static u16 sys_timer=0;
  disableInterrupts();   //�ر�ϵͳ���ж�
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1); //�ڲ�ʱ��Ϊ1��Ƶ = 16Mhz 
  IOInit();
  TIM2_Init(16000);
  TIM2_ARRPreloadConfig(ENABLE);  //ʹ�ܶ�ʱ��2�Զ����ع���    
  TIM2_Cmd(ENABLE); 


  
  UART1_Init(ZIGBEE_BAUD);
  enableInterrupts();   //ʹ��ϵͳ���ж�

  BFCT_protocol_Zigbee.receive_enable = 0;
  BFCT_protocol_Zigbee.receive_flag = 0;
  BFCT_protocol_Zigbee.receive_data = G_receive_data ;
  BFCT_protocol_Zigbee.send_data = G_send_data ;
  
  BFCT_protocol_Lock.receive_enable=0;
  BFCT_protocol_Lock.receive_len =0;
  BFCT_protocol_Lock.receive_flag = 0;
  BFCT_protocol_Lock.receive_data = G_receive_data ;
  BFCT_protocol_Lock.send_data = G_send_data ;
  
>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e
  while(1)
  {

   led();  //���
   
   USART_process();
   
<<<<<<< HEAD
   if((Zigbee_process_done | Lock_process_done) | (sys_timer >= 10000) )
   {
      delay =0;
=======
   if((Zigbee_process_done && Lock_process_done) | (sys_timer >= 10000) |(Zigbee_process_timeout) |(Lock_process_timeout) )
   {
>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e
      lock_state=0;
      zigbee_state =0;
      lock_state =0;
      sys_timer=0;
      
      zigbee_erro =0;
      lock_erro =0;
      
      Zigbee_process_done =0;  //���������ɱ�־λ
      Lock_process_done =0;
      
      Zigbee_processing_flag =0;  //������ڴ���ʹ�ܺ���
      Lock_processing_flag =0;
      
      BFCT_protocol_Lock.receive_enable=0;  //������ڽ��ձ�־λ
      BFCT_protocol_Lock.receive_len =0;
      
<<<<<<< HEAD
      
      lock_interrupt = 0;
      zigbee_interrupt = 0;
      
      USART1->CR2=0;//disable transmision and receiver
      USART_Cmd(USART1 , DISABLE);

      init_pin_interrupt();//����͹���ģʽ
 //     disableInterrupts();
      
      
   }
  if(lock_interrupt)
  {
      delay ++;
      if(delay ==1){
        config_lock_rx_pin(GPIO_Mode_In_FL_No_IT); //���û�������Ϊ ���жϵĸ������� ��������Ϊ��ģ�黽������
        config_wake_up_out(GPIO_Mode_In_FL_No_IT); //���û�������Ϊ ���жϵĸ������� 
      }
      if( delay > 0  && delay < 9)
      {
        if( get_lock_rx_pin())
        {  }
        else
        {  sys_timer = 10000; delay =0; }
        
      }
      if(delay ==9) {
        delay =0;
        lock_interrupt =0;
        UART1_RemapInit(9600);//������ ���ó� 9600
     //   config_lock_tx_pin(GPIO_Mode_Out_PP_High_Fast); //TXD
        GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast); //TXD
        BFCT_protocol_Lock.receive_enable=1;
        BFCT_protocol_Zigbee.receive_enable=0;
        BFCT_protocol_Lock.receive_len =0;
   //     enableInterrupts();
      }
  }
   
  else if(zigbee_interrupt)
  {   
      config_lock_rx_pin(GPIO_Mode_In_FL_No_IT); //���û�������Ϊ ���жϵĸ������� ��������Ϊ��ģ�黽������
      config_wake_up_out(GPIO_Mode_In_FL_No_IT); //���û�������Ϊ ���жϵĸ������� 
      zigbee_interrupt = 0;
      UART1_Init(ZIGBEE_BAUD);//������ ���ó� 19200
      BFCT_protocol_Lock.receive_enable=0;
      BFCT_protocol_Zigbee.receive_enable =1;
      BFCT_protocol_Zigbee.receive_len =0;
  //    enableInterrupts();
  }

=======
      Zigbee_process_timeout =0;  //��ʱ��־λ����
      Lock_process_timeout=0;
      
      USART1->CR2=0;//disable transmision and receiver
      USART_Cmd(USART1 , DISABLE);
      SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortC , DISABLE); //��
      SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortA , DISABLE); //��
      CLK_PeripheralClockConfig(CLK_Peripheral_USART1 , DISABLE); //ʹ��USART1ʱ��
      
      init_pin_interrupt();//����͹���ģʽ
      disableInterrupts();
      
      if(lock_interrupt)
      {
        if(t_1ms){ 
          delay++;
          if(delay ==1)
          config_lock_rx_pin(GPIO_Mode_In_FL_No_IT); //���û�������Ϊ ���жϵĸ������� ��������Ϊ��ģ�黽������
          if(delay >0) && (delay < 5)
          {
            if( get_lock_rx_pin()){}
            else 
            {
              delay =0;
              sys_timer = 10000;
            }
               
          }
          if(delay ==5)
          {
            lock_interrupt =0;
            UART1_RemapInit(9600);//������ ���ó� 9600
            config_lock_tx_pin(GPIO_Mode_Out_PP_High_Fast); //TXD
            BFCT_protocol_Lock.receive_enable=1;
            BFCT_protocol_Zigbee.receive_enable=0;
            BFCT_protocol_Lock.receive_len =0;
            delay =0��
          }
        }
        

      }
       
      else if(zigbee_interrupt)
      {   
        if(t_1ms){ 
          delay++;
          if(delay ==1)
          wake_up_out(GPIO_Mode_In_FL_No_IT); //���û�������Ϊ ���жϵĸ������� 
          if(delay >0) && (delay < 5)
          {
            if( get_wakeup_out_pin_state()){}
            else 
            {
              delay=0;
              sys_timer = 10000;
            }
               
          }
          if(delay ==5)
          {
            zigbee_interrupt = 0;
            UART1_Init(ZIGBEE_BAUD);//������ ���ó� 9600
            BFCT_protocol_Lock.receive_enable=0;
            BFCT_protocol_Zigbee.receive_enable =1;
            BFCT_protocol_Zigbee.receive_len =0;
            delay =0��
          }
        }
      }
      enableInterrupts();
    }
>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e
    if(t_1ms)
      sys_timer ++;
    
    os_time();   //ϵͳʱ��
  }
  
}