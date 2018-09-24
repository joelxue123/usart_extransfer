#include "protocol.h"
#include "global.h"
#include "userdata.h"
#include "command.h"
#include "usart.h"
#include "bsp.h"

const u8 key[]={0x46,0x45,0x49,0x42,0x49,0x47};
u8 password[6]={1,2,3,4,5,6};
u8 npass[6];
u8 remote_open_flag = 0;
 u32 cmd_id = 1;

/***********
** �������ƣ�xor_check()
**������������У��
**��ڲ��� �� char * ��u8 len
**����ֵ  ������� xor
***********/

u8 xor_check(u8 *p,u16 len)
{
  u16 i;
  u8 xor=0;
  for(i=0;i<len;i++)
  {
    xor = xor^p[i];
  }
  return xor;
}

/***********
** �������ƣ�xor_check()
**������������У��
**��ڲ��� �� char * ��u8 len
**����ֵ  ������� xor
***********/
void for_cmd_id(u32 cmd_id,u8 cmd_type,u8 *buff)
{
  buff[0]=( cmd_id >> 24) & 0xff;      //����������ID
  buff[1]=(cmd_id >> 16) & 0xff;      //����������ID
  buff[2]=(cmd_id >> 8) & 0xff;     //����������ID
  buff[3]=(cmd_id ) & 0xff;    //����������ID
    
  if(cmd_type ==0)
     buff[4]=0x00;      //Ӧ��
  else{
     buff[4]=0x01;      //Ӧ��
  }
}

/***********
** �������ƣ�encryption()
**�����������������
**��ڲ��� ��
**����ֵ 
***********/

void encryption(void)
{
  u8 i;
  for(i=0;i<6;i++)
  {
    npass[i] =(u8) key[i] ^  password[i];
  }
}

/*******��������֤****/
u8 check_code(u8 *buf)
{
  u16 data_len = 0;
  data_len = buf[1];
  if(buf[data_len +8] != (xor_check(buf,data_len +8)) )
  {
    return 1;
  }
  else 
    return 0;
}

/**********
*�������ƣ�zigbee_protocal_component
**�������� ����֡��zigbeeЭ��
**���������� buf:������õ�֡��len:���ݳ���+������ cmd:Э������ ��data_buf :��������
**********/
u8  zigbee_protocal_component(u8 *buf,u16 len,u8 cmd,u8* data_buff)
{
  u8 i;
  buf[0]=0xAA;      //֡ͷ
  buf[1]=len;      //��Ч����
  buf[2]=cmd;      //������

  for(i=0;i<len+4;i++){
     buf[3+i]=data_buff[i];      //Ӧ��  
  }
  buf[8+len]=xor_check(BFCT_protocol_Zigbee.send_data,len+8);     //����
  buf[9+len]=0x55;      
  return (len+10); 
}


/**** У��zigbee����
*����ֵ �� 0 ��ʾ���� 1����ʾ��ȷ
*******/
u8 check_zigbee_cmd(u8 zigbee_cmd)
{
  u8 data_len;
  data_len = BFCT_protocol_Zigbee.receive_data[1];
  if(BFCT_protocol_Zigbee.receive_data[data_len +8] != (xor_check(BFCT_protocol_Zigbee.receive_data,data_len +8)) )
  { /**********����У����� ************/
  data_len =0 ;
#if DEBUG    
  zigbee_erro =  zigbee_erro | sum_check_BIT;
#endif
  return 0;
  }
  if( BFCT_protocol_Zigbee.receive_data[ZIGBEE_CMD_ADDR] != zigbee_cmd)   //��ȡ�������� != ZIGBEE_CMD_ACK)
  {
  data_len =0 ;
#if DEBUG
  zigbee_erro =  zigbee_erro | NACK_BIT;
#endif
  return 0;
  }
  return 1;
}

/*****����zigbee��� ����1 ��ʾ������ɣ����� 0��ʾ ���� ʧ��*******/
u8 send_zigbeecmd(u8 len,u8 zigbeecmd,u8 *data_buff )
{
    static u8 zigbee_moni_state=0;
    static u16 delay=0;
    u8 ret;
    
    switch(zigbee_moni_state)
    {
    case 0:               
          BFCT_protocol_Zigbee.send_len =  zigbee_protocal_component(BFCT_protocol_Zigbee.send_data,len,zigbeecmd,data_buff);
          if(data_buff[4] == ACK){
          zigbee_moni_state ++;
          delay=0;   
          }
          else{
          BFCT_protocol_Lock.receive_enable=0;  //������ڽ��ձ�־λ
          BFCT_protocol_Lock.receive_len =0;
          BFCT_protocol_Zigbee.receive_enable = 1;
          BFCT_protocol_Zigbee.receive_len =0;
          
          BFCT_protocol_Zigbee.receive_flag =0;
          zigbee_moni_state =2;
          delay=0; 
          }
  
      break;
    case 1:
      if(BFCT_protocol_Zigbee.send_len ==0)
      {
        zigbee_moni_state =0;
        delay =0;
        return 1;
      }
     if(t_1ms)
     {
       delay++;
       if(delay > 500){
         zigbee_moni_state=0;
         delay=0;
         lock_erro =  lock_erro | TIMEOUT_BIT;
         return 1;
       }
     }
      break;
         
    case 2:
        if(BFCT_protocol_Zigbee.receive_flag == 1)
        {
          BFCT_protocol_Lock.receive_enable=0;  //������ڽ��ձ�־λ
          BFCT_protocol_Lock.receive_len =0;
          BFCT_protocol_Zigbee.receive_enable = 0;
          
          BFCT_protocol_Zigbee.receive_flag = 0;
          ret = check_zigbee_cmd(zigbeecmd);
          zigbee_moni_state=0;
          return 1;
        }
       if(t_1ms)
       {
         delay++;
         if(delay > 1000){
           zigbee_moni_state=0;
           delay=0;
  #if DEBUG             
           zigbee_erro =  zigbee_erro | TIMEOUT_BIT;
  #endif
           return 1;
         }
       }
       break;
    default:
      break;
    }
    return 0;
}

/**********
**�������� �� �Ͼ�ģ�� �ظ� ACK ����
**********/
u8 zigbee_usart_send(void)
{
    static u8 delay=0;
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
        send_hex(BFCT_protocol_Zigbee.send_data,BFCT_protocol_Zigbee.send_len); // ��������

      } 
      if(delay ==44)
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