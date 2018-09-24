#include "protocol.h"
#include "global.h"
#include "userdata.h"
#include "command.h"


/***********
** �������ƣ�sum_check()
**������������У��
**��ڲ��� �� char * ��u8 len
**����ֵ  ����� sum
***********/

u8 sum_check(u8 *p,u16 len)
{
  u16 i;
  u8 sum=0;
  for(i=0;i<len;i++)
  {
    sum = sum +p[i];
  }
  return sum;
}

/*******��������֤****/
u8 check_code(u8 *buf)
{
  u16 data_len = 0;
  
  data_len = (buf[2] << 8) + buf[3];

  if(buf[data_len +3] != sum_check(buf,data_len +3) )
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
  buf[0]=0xA1;      //֡ͷ
  buf[1]=cmd;      //��Ч����
  buf[2]=(len >>8) &0xff;      //������
  buf[3]=len & 0xff;      //����������ID
  
  for(i=0;i<len-1;i++){
     buf[4+i]=data_buff[i];      //Ӧ��  
  }
  buf[3+len]=sum_check(buf,len+3);     //����
  return (len+4); 
}


/**** У��zigbee����
*����ֵ �� 0 ��ʾ���� 1����ʾ��ȷ
*******/
u8 check_zigbee_cmd(u8 zigbee_cmd)
{
  u8 data_len;
  data_len = (BFCT_protocol_Zigbee.receive_data[2] << 8) + BFCT_protocol_Zigbee.receive_data[3];
  if(BFCT_protocol_Zigbee.receive_data[data_len +3] != sum_check(BFCT_protocol_Zigbee.receive_data,data_len +3) )
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
        BFCT_protocol_Zigbee.send_len = zigbee_protocal_component(BFCT_protocol_Zigbee.send_data,len,zigbeecmd,data_buff); //data_buff = 0 0 0 0 0 0 0 0 0 0 0 0 
        if(zigbeecmd == ZIGBEE_CMD_ACK){
        zigbee_moni_state ++;
        delay=0;   
        }
        else{
        BFCT_protocol_Zigbee.receive_enable=1;
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
    break;
       
  case 2:
      if(BFCT_protocol_Zigbee.receive_flag == 1)
      {
        
        BFCT_protocol_Zigbee.receive_flag = 0;
        ret = check_zigbee_cmd(ZIGBEE_CMD_ACK);
        BFCT_protocol_Zigbee.receive_enable=0;
        BFCT_protocol_Zigbee.receive_len =0;
        zigbee_moni_state=0;
        return 1;
      }
     if(t_1ms)
     {
       delay++;
       if(delay > 1000){
         zigbee_moni_state=0;
         delay=0;         
         zigbee_erro =  zigbee_erro | TIMEOUT_BIT;
         return 1;
       }
     }
     break;
  default:
    break;
  }
  return 0;
}