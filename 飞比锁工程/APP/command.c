#include "stm8l15x.h"
#include "global.h"
#include "eeprom.h"
#include "usart.h"
#include "protocol.h"
#include "command.h"
#include "userdata.h"
#include "bsp.h"

u8 zigbee_moni_state=0;
u16 zigbee_delay=0;

#if defined(Nan_Jing)
u16 casual_work_No =0;
#endif 


u8 data_buff[66]={0};

 
/**************************************************************************
**�������ƣ�void crc_8n()     Name: void crc_8n()
**�������� �����
**��ڲ�������
**�������
**************************************************************************/

u8 crc_8n(unsigned char *ptr,unsigned char n)
{
	unsigned char j=8;
	unsigned char cbit,out;
	unsigned char crc=0x79,crc_a=0,crc_b=0;	
	//*ȡ��λ��λ*/	
	do{
		crc_a=*ptr;
		ptr++;
		j=8;
		cbit=1;
		do{
			crc_b=crc_a;
			crc_b=crc_b^crc;
			out=crc_b&cbit;
		//	crc_b=crc_b>>1;
			if(out!=0)
                        {
                                crc=crc^0X18;
                                crc=crc>>1;
                                crc|=0x80;
                        }else
                                crc=crc>>1;
			crc_a=crc_a>>1;
		//	cbit=cbit<<1;
		}while(--j);
	}while(--n);
	return crc;
}



/**********
*�������ƣ�lock_protocal_component
**�������� ����֡��lockЭ��
**���������� buf:������õ�֡��len:���ݳ���+������ cmd:Э������ ��data_buf :��������
**********/
u8  lock_protocal_component(u8*buf, u16 len,u8 cmd,u8* data_buff)
{
    u16 i;
    buf[0]=0x02;      //֡ͷ
    buf[1]=len &0xFf;      //����
    buf[2]=(len >> 8)&0xFf;     //����
    buf[3]=cmd;     //������
    for(i=0;i<len-1;i++)
    {
       buf[4+i] = data_buff[i];
    }
    buf[len+3]=crc_8n(buf,len+3);  //����
    buf[len+4]=0xff;     //֡β
    return (5+len);
}




/**** У��������
*����ֵ �� 0 ��ʾ���� 1����ʾ��ȷ
*******/
u8 check_lock_cmd(u8 lock_cmd)
{
  u8 data_len=0;

  data_len = BFCT_protocol_Zigbee.receive_data[1] + (BFCT_protocol_Zigbee.receive_data[2] << 8); //��ȡ���ݳ���
  if(BFCT_protocol_Zigbee.receive_data[data_len +3] != crc_8n(BFCT_protocol_Zigbee.receive_data,data_len +3) )
  {  /**********����У����� ************/
    data_len =0 ;
#if DEBUG        
    lock_erro =  lock_erro | crc_8n_ERRO_BIT; 
#endif
    return 0;
  } 
  else if(BFCT_protocol_Zigbee.receive_data[3] != lock_cmd)
  {
#if DEBUG    
    lock_erro =  lock_erro | crc_8n_ERRO_BIT;
#endif
    return 0;
  }

  return 1;
}




/*****����lcok��� ����1 ��ʾ������ɣ����� 0��ʾ �������ͣ�����1��ʾ�ɹ�������2��ʾ ʧ��*******/
u8 send_lcokcmd(u8 len,u8 zigbeecmd,u8 *data_buff )
{
  static u8 lock_moni_state =0;
  
  switch(lock_moni_state)
  {
   case 0:
      BFCT_protocol_Lock.send_len = lock_protocal_component(BFCT_protocol_Lock.send_data,len,zigbeecmd,data_buff);
      BFCT_protocol_Lock.receive_enable=0;
      BFCT_protocol_Zigbee.receive_enable=0;
      lock_moni_state++;
    break; 
  case 1:
     if( BFCT_protocol_Lock.send_len ==0) //�ȴ��������
     {
        BFCT_protocol_Lock.receive_enable=1;
        BFCT_protocol_Lock.receive_len =0;
        lock_moni_state =0 ;
        return 1;
     }
    if(BFCT_protocol_Lock.send_fail==1)
    {
      BFCT_protocol_Lock.send_fail =0;
      lock_moni_state =0;
      return 2;
    }
  break;
  default:
  break;
  }
  return 0;
}

 /*****
 *�������ƣ�get_lock_message
  *����ֵ��0: δ��ɣ������� 1:�ɹ������ء� 2�� ʧ�ܣ�����
 ********/ 
  
u8  get_lock_message(void)
{
  u8 ret =0;
  if(BFCT_protocol_Lock.receive_flag==1)
  {
    GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast); //TXD
    BFCT_protocol_Lock.receive_flag=0;
    ret =1;
  }
   if(t_1ms)
   {
     zigbee_delay++;
     if(zigbee_delay >= (u16)1000){
      
        zigbee_delay =0;
        ret =2;
     }
   }
  return ret;
}
/********
* ����������
*����ֵ�� 0 Ϊ��ɣ�1�����
************/
u8 lock_opennet()
{
  static u8 lock_moni_state =0,len=0,cmd;
  u8 ret;
  
  switch(lock_moni_state)
  {
  case 0:
#if defined(Fei_Bi)
    for_cmd_id(cmd_id,REQUEST,data_buff);
    data_buff[5+0] = 0;data_buff[5+1] = 0;data_buff[5+2] = 0;data_buff[5+3] = 0;data_buff[5+4] = 0;data_buff[5+5] = 0;
    data_buff[5+6] = 0;data_buff[5+7] = 0;data_buff[5+8] = 0;data_buff[5+9] = 0;data_buff[5+10] = 0;
    len = 0x0a;
#elif defined(Nan_Jing)
    len = 0x01;
#endif
    cmd = ZIGBEE_CMD_OPENNET;
    lock_moni_state++;
    break;
  case 1:
      ret = send_zigbeecmd(len,cmd,data_buff);
      if(ret == 1) {
        lock_moni_state =0;
#if defined(Fei_Bi)
        cmd_id++;
#endif
      }
    break;
  default:
    break;
  }
  return ret;
}
/*****�����ر� ******/
u8 lock_be_closed(void)
{
  static u8 lock_moni_state=0;
  static u16 delay =0;
  u8 ret;
  #if    defined( Nan_Jing)
  switch(lock_moni_state)
  {
  case 0:

      read_userdata4eeprom ( casual_work_addr,(u8 *)&casual_work_No,2);
      if(casual_work_No == (u16)0x999)  
      {
         data_buff[0] = 0X99;
         data_buff[1] = 0X09;
         data_buff[2] = 0X80;
         zigbee_delay =0;
         lock_moni_state ++;
      }
      else
        return 1;
    break;
  case 1:
       if(t_1ms)
       {
         delay++;
         if(delay >= (u16)200){
            delay =0;
            lock_moni_state++;
         }
       }
    break;

  case 2:
    ret = send_lcokcmd(0x04,LOCK_DELETE_USER,data_buff );
    if(ret ==1)
    {
      BFCT_protocol_Lock.receive_flag =0;
      lock_moni_state ++;

    }
    else if(ret ==2)
    {
      lock_moni_state =0;
      return 1;
    }
    break;
    
  case 3:
       if(BFCT_protocol_Lock.receive_flag==1)
       {
          GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast); //TXD
          BFCT_protocol_Lock.receive_flag=0;
          ret = check_lock_cmd(0x82);
          if(ret ==1)
          { 
            if(BFCT_protocol_Lock.receive_data[4] == 0x00){
              casual_work_No = 0x0000;
              write_userdata2eeprom( casual_work_addr,(u8 *)&casual_work_No,2);
            }
            lock_moni_state =0;
            return 1;
          }
       }
       if(t_1ms)
       {
         delay++;
         if(delay >= (u16)1000){
            delay =0;
            lock_moni_state=0;
            return 1;
         }
       }
    break;
  default:
    break;
    
  }
  return 0;
  
#else
  return 1;
#endif 
  
}

/********
* �ر���������
*����ֵ�� 0 Ϊ��ɣ�1�����
************/
u8 lock_closenet(void) 
{
  u8 ret=0;
  ret = send_zigbeecmd(0x01,ZIGBEE_CMD_CLOSENET,data_buff);
  return ret;
  
}

/*****���߸�ʽ������*********/
u8 lock_format(void)
{
  u8 ret;
  ret = send_zigbeecmd(0x01,ACK_ZIGBEE_FORMAT,data_buff);
  return ret;
}

/*****���밲ȫģʽ���� *******/
u8 lock_login_safemode()
{
  return 1;
}
//*******�˳���ȫģʽ ************
u8 lock_logout_safe_mode()
{
  return 1;
}
/****���幦��******/
u8  lock_alarm(void)
{
  static u8 lock_moni_state = 0,len =0,cmd=0;
  u8 ret=0 ;
  switch(lock_moni_state)
  {
    case 0:
#if defined(Fei_Bi)
      for_cmd_id(cmd_id,REQUEST,data_buff);
      data_buff[5+0] = 0;data_buff[5+1] = 0;data_buff[5+2] = 0;data_buff[5+3] = 0;data_buff[5+4] = 0;data_buff[5+5] = 0;
      data_buff[5+6] = 0;data_buff[5+7] = 0;data_buff[5+8] = 0;data_buff[5+9] = 0;data_buff[5+10] = 0;
      cmd = ZIGBEE_CMD_WAKEUP_MODULE;
      len = 0x0a;
#elif defined(Nan_Jing)
      len = 0x01;
      return 1;
#endif
      lock_moni_state++;
      break;
    case 1:
        ret = send_zigbeecmd(len,cmd,data_buff);
        if(ret == 1) {
          lock_moni_state =0;
#if defined(Fei_Bi)
          cmd_id++;
#endif
        }
      break;
    default:
      break;
  }
  return ret;
}
/**********�˳�����ģʽ*********/
u8 lock_logout_settting_mode(void)
{
    static u8 lock_moni_state=0;
    static u16 delay=0;
    u8 ret;

    switch(lock_moni_state)
    {
      case 0:
          read_userdata4eeprom(zigbee_clock_addr, data_buff, 6);
          if(data_buff[0] |data_buff[1] | data_buff[2])
          {
            lock_moni_state++;
            delay =0;
          }
          else
            return 1;
      break;
  case 1:
       if(t_1ms)
       {
         delay++;
         if(delay >= (u16)300){
            delay =0;
            lock_moni_state++;
         }
       }
    break;

      case 2:
          ret = send_lcokcmd(0x08,LOCK_CLOCK_CALIBRATION,data_buff );
          if(ret ==1)
          {
            GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast); //TXD
            data_buff[0]=0; data_buff[1]=0; data_buff[2]=0; data_buff[3]=0; data_buff[4]=0; data_buff[5]=0; data_buff[6]=0; 
            write_userdata2eeprom(zigbee_clock_addr, data_buff, 6);
            delay =0;
            lock_moni_state = 0;
            return 1;
          }
          else if(ret ==2)
          {
            delay =0;
            lock_moni_state =0;
            return 1;
          }
      break;
    default:
    break; 

    }
    return 0;
}

/********ǿ�ƿ�������*********/
u8 lock_openclock_byforce()
{
  return 1;
}
/******�ϴ���״̬********/
u8 lock_state_updata()
{
  return 1;
}

/******��������û�����*******************/
u8 lock_add_user()
{
  static u8 lock_moni_state=0;
  u8 ret,ret1;
  
  switch(lock_moni_state)
  {
  case 0:
    
      lockdata_2_zigbeedata (lock_user_attribute,&(BFCT_protocol_Lock.receive_data[14]),&data_buff[0]); //�û����� ��Ϣ
      lockdata_2_zigbeedata (lock_user_No,&(BFCT_protocol_Lock.receive_data[4]),&data_buff[1]); //�û������Ϣ
      lockdata_2_zigbeedata(lock_password,&(BFCT_protocol_Lock.receive_data[37]),&data_buff[5]);   //���볤��  
      lock_moni_state++;

    break;
  case 1:
      ret = send_zigbeecmd(0x03,ZIGBEE_CMD_ADDUSER_FROM_LOCK,data_buff);
      if(ret)
      {
         if(zigbee_erro){
            lock_moni_state =0;
            return 1;
         }
         else{
           lock_moni_state ++; 
         }
      }
    break;
  case 2:
       data_buff[2] = 0x01;  // ������ʽ
       data_buff[3] = 0x06;  //  ������ 
       lock_moni_state++;
    break;
  case 3:
      ret1 = send_zigbeecmd(0X0b,ACK_ZIGBEE_MODIFY_USERINFO,data_buff);
      if(ret1)
      {
        lock_moni_state=0; 
        return 1;
      }
    break;  
  default:
    break;
  }
  return 0;
}

u8 lock_be_opened(void)
{
  static u8 lock_moni_state=0,len =0,cmd =0;
  static u16 delay=0;
  u8 ret;
  static u8 battery1_level =0, battery2_level =0,user_No=0;   
  
  switch(lock_moni_state)
  {
    case 0:
        battery1_level = BFCT_protocol_Lock.receive_data[4];  
        battery2_level = BFCT_protocol_Lock.receive_data[5]; 
        user_No      = BFCT_protocol_Lock.receive_data[8]; 
#if defined(Fei_Bi)
        for_cmd_id(cmd_id,REQUEST,data_buff);
        data_buff[5+0] =  user_No; data_buff[5+1] = 0;  //�û����
        data_buff[5+2] = 0x01   ;data_buff[5+3] = 0;data_buff[5+4] = 0;data_buff[5+5] = 5;
        data_buff[5+6] = 0;data_buff[5+7] = 0;data_buff[5+8] = 0;data_buff[5+9] = 0;data_buff[5+10] = 0;
        len = 0x0a;
        if(remote_open_flag == 1){
            remote_open_flag =0;
            cmd = ACK_ZIGBEE_CMD_OPENLOCK;
        }
        else
            cmd = ACK_ZIGBEE_CMD_LOCAL_OPEN_LOCK;
#elif defined(Nan_Jing)
   
        data_buff[0] =0x01;//   //�û�����
        data_buff[1] = u8_BCD_2_hex(user_No);  //�û����
        data_buff[2] =0x01;  // ������ʽ
        data_buff[3] = 0x01;  //  ������ 
        len = 0x05; cmd = ACK_ZIGBEE_CMD_UPLOAD;
#endif
        BFCT_protocol_Zigbee.receive_len =0;
        BFCT_protocol_Zigbee.receive_enable=1;
        BFCT_protocol_Lock.receive_enable=0;
        BFCT_protocol_Zigbee.receive_flag =0;
        delay=0;
        lock_moni_state++;
    break;
    case 1:
      ret = send_zigbeecmd(len,cmd,data_buff);
      if(ret)
      {
         if(zigbee_erro){
            delay =0;
            lock_moni_state =0;
            return 1;
         }
#if defined(Fei_Bi)
         cmd_id++;
#endif
         lock_moni_state++; 
      }
    break;
  case 2:
      if( (battery1_level +battery2_level)< 50 )
      {
#if defined(Fei_Bi)
        for_cmd_id(cmd_id,REQUEST,data_buff);
        data_buff[5+0] = 0;data_buff[5+1] = u8_BCD_2_hex(user_No);  //�û����
        data_buff[5+2] = 0;data_buff[5+3] = 0;data_buff[5+4] = 0;data_buff[5+5] = 5;
        data_buff[5+6] = 0;data_buff[5+7] = 0;data_buff[5+8] = 0;data_buff[5+9] = 0;data_buff[5+10] = 0;
        len = 0x0a;
        cmd = ACK_ZIGBEE_CMD_LOW_POWER;
#elif defined(Nan_Jing)
        len =0x01;  cmd = ACK_ZIGBEE_CMD_LOW_POWER;
#endif
        
        delay = 0;
        lock_moni_state ++;
      }
      else
      {
        delay =0;
        lock_moni_state =0;
        return 1;
      }
    break;
    case 3:
         if(t_1ms)
         {
           delay++;
           if(delay == 500)
           {
              BFCT_protocol_Zigbee.receive_len =0;
              BFCT_protocol_Zigbee.receive_enable=1;
              BFCT_protocol_Lock.receive_enable=0;
              BFCT_protocol_Zigbee.receive_flag =0;
              delay=0;
              lock_moni_state++;
           }
         }
    break;
    case 4:
      ret = send_zigbeecmd(len,cmd,data_buff);
      if(ret)
      {
#if defined(Fei_Bi)
         cmd_id++;
#endif
         delay = 0;
         lock_moni_state=0; 
         return 1;

      }
    break;  
    default:
    break;
  }
  return 0;
}
/********�Ƿ��û����ϱ�����**********/
u8 lolck_illage_user_report(void)
{
  u8 ret;
  ret = send_zigbeecmd(0x01,ACK_ZIGBEE_ILLAGEL_USER_REPORT,data_buff);
  if(ret)
  {
      return 1; 
  }
  else
  return 0;
}

/*****ɾ���û�**********/
u8 lock_delete_user(void)
{
  static u8 lock_moni_state=0;
  u8 ret;  
  
  switch(lock_moni_state)
  {
    case 0:   
        data_buff[0] =0x01;//   //�û�����
        lockdata_2_zigbeedata( lock_user_No,&BFCT_protocol_Lock.receive_data[4], &data_buff[1]);
        lock_moni_state++;
      break;
  case 1:
      ret = send_zigbeecmd(0x03,ACK_ZIGBEE_CMD_DELETE_USER,data_buff);
      if(ret)
      {
        lock_moni_state = 0;
          return 1; 
      }
      break;
  default:
    break;
  }
  return 0;
}



/********************zigbee�·����� **********************************/
/****************
*���������� ��������
************************/
u8 zigbee_openlock(void)
{
  static u8 len=0 ,cmd =0; 
  u8 ret,i;

  switch(zigbee_moni_state)
    {
      case 0:
#if defined(Fei_Bi)
        cmd = ZIGBEE_CMD_OPENLOCK;
        cmd_id = *((u32*)&BFCT_protocol_Zigbee.receive_data[3]);
        for_cmd_id(cmd_id,ACK,data_buff);
        len =0x01;
#elif defined(Nan_Jing)
       cmd =  ZIGBEE_CMD_ACK;
       len =0x01;
#endif
        zigbee_moni_state++;
      break;
      case 1:
         ret = send_zigbeecmd(len,cmd,data_buff);
         if(ret)
         {
           if(1)
           {
#if defined(Nan_Jing)
              data_buff[0] = 0;data_buff[1] = 0;data_buff[2] = 0;data_buff[3] = 0;data_buff[4] = 0;data_buff[5] = 0;
              write_userdata2eeprom(zigbee_psw_addr,data_buff,0x06);
              zigbee_delay = 0;
              zigbee_moni_state++;     
#elif defined(Fei_Bi)
              encryption();
              for(i=0;i<6;i++){
               if(BFCT_protocol_Zigbee.receive_data[8+i] != npass[i])
                  break;
              }
              if(i == 6)
              zigbee_moni_state ++;
              else{
                  for_cmd_id(cmd_id,REQUEST,data_buff);
                  data_buff[5+0] = 0;data_buff[5+1] = 1;data_buff[5+2] = 0;data_buff[5+3] = 0;data_buff[5+4] = 0;data_buff[5+5] = 0;
                  data_buff[5+6] = 0;data_buff[5+7] = 0;data_buff[5+8] = 0;data_buff[5+9] = 0;data_buff[5+10] = 0;
                  len = 0x0a;
                  cmd = ACK_ZIGBEE_CMD_OPENLOCK;
                  zigbee_moni_state = 10;
              } 
#endif
           }
           else 
           {
             zigbee_delay = 0;
             BFCT_protocol_Zigbee.receive_flag = 0;
             BFCT_protocol_Lock.receive_enable=0;
             BFCT_protocol_Zigbee.receive_enable=0;
             BFCT_protocol_Zigbee.send_len = zigbee_protocal_component(BFCT_protocol_Zigbee.send_data,0x01,0x02,data_buff); //data_buff = 0 0 0 0 0 0 0 0 0 0 0 0 
             zigbee_moni_state =10;
           }
         }
        break;
       case 2:
         ret = send_lcokcmd(0x01,LOCK_OPENLOCK,data_buff );
         if(ret == 1)
         {
            BFCT_protocol_Lock.receive_flag=0;
            zigbee_moni_state++;
         }
         else if(ret == 2)
         {

            BFCT_protocol_Zigbee.receive_enable=1;
            BFCT_protocol_Lock.receive_enable=0;
#if defined(Nan_Jing)
            len = 0x01; cmd = 0x0D
            zigbee_moni_state=10;
#elif  defined(Fei_Bi)
            for_cmd_id(cmd_id,REQUEST,data_buff);
            data_buff[5+0] = 0;data_buff[5+1] = 1;data_buff[5+2] = 0;data_buff[5+3] = 0;data_buff[5+4] = 0;data_buff[5+5] = 0;
            data_buff[5+6] = 0;data_buff[5+7] = 0;data_buff[5+8] = 0;data_buff[5+9] = 0;data_buff[5+10] = 0;
            len = 0x0a;
            cmd = ACK_ZIGBEE_CMD_OPENLOCK;
            zigbee_moni_state = 10;
#endif            
            } 
      break;
     case 3:
       ret = get_lock_message();
       if(ret ==1)
       {
          ret = check_lock_cmd(ACK_LOCK_OPENLOCK);
          if(ret ==1)
          { 
#if  defined(Fei_Bi)
            remote_open_flag =1;
#endif
            zigbee_moni_state = 0;
            zigbee_delay = 0;
            return 1;
          }
          else
          {
            len = 0x01; cmd = 0x0D;
            BFCT_protocol_Zigbee.receive_enable=1;
            BFCT_protocol_Lock.receive_enable=0;
            zigbee_delay =0;
            zigbee_moni_state=10;
          }
       }
       else if(ret ==2)
       {
          zigbee_delay =0;
          zigbee_moni_state=10;
       }
       break;
         
       case 10:
          ret = send_zigbeecmd(len,cmd,data_buff);
          if(ret == 1) {
            zigbee_delay =0;
            zigbee_moni_state =0;
#if defined(Fei_Bi)
            cmd_id++;
#endif
            return 1;
          }
         break;  
         
       default:
         zigbee_delay =0;
         zigbee_moni_state =0;
         return 1;
       break;
    }
  return 0;
}

/*****zigbee������ʧ�� *******/
u8 zigbee_opennet_fail_2(void)
{
  u8 ret;
  
  ret = send_zigbeecmd(0X01,ZIGBEE_CMD_ACK,data_buff);
  return ret;
}
/*****zigbee�������� *******/
#if defined(Fei_Bi)
u8 zigbee_online_info(void)
{
  static u8 len,cmd;
  u8 ret =0;
  switch(zigbee_moni_state)
  {
  case 0:
#if defined(Fei_Bi) 
    cmd = ZIGBEE_CMD_ONLINE_INFO;
    cmd_id = *((u32*)&BFCT_protocol_Zigbee.receive_data[3]);
    for_cmd_id(cmd_id,ACK,data_buff);
    len =1;
#endif
    zigbee_moni_state ++;
    break;
  case 1:
     ret = send_zigbeecmd(len,cmd,data_buff);
     if(ret ==1) zigbee_moni_state =0;
    break;
  default:
    break; 
  }

  return ret;
}
#endif
/*****zigbee������ʧ�� *******/
u8 zigbee_opennet_fail()
{
  u8 ret;
  
  ret = send_zigbeecmd(0X01,ZIGBEE_CMD_ACK,data_buff);
  return ret;
}

/*****zigbee ����û� *******/
u8 zigbee_adduser(void)
{
#if  defined( Nan_Jing)
  u8 ret;

  switch(zigbee_moni_state)
  {
  case 0:
      data_buff[0] = 0x3f;
      data_buff[1] = 0x10;
      data_buff[2] = 0x01;
      zigbeedata_2_lockdata(zigbee_user_No,&data_buff[1],&BFCT_protocol_Zigbee.receive_data[4]);
      zigbeedata_2_lockdata(zigbee_user_attribute,&data_buff[11],&BFCT_protocol_Zigbee.receive_data[4]);
      data_buff[33]=0x80;
      //zigbeedata_2_lockdata(zigbee_user_attribute,&data_buff[34],&BFCT_protocol_Zigbee.receive_data[7]);
      data_buff[34]=0x11;data_buff[35]=0x11;data_buff[36]=0x11;data_buff[37]=0xff;data_buff[38]=0xff;data_buff[39]=0xff;data_buff[40]=0xff;
      zigbee_moni_state++;
    break;
    
  case 1:
       ret = send_zigbeecmd(0X01,ZIGBEE_CMD_ACK,data_buff);
       if(ret)
       {
          zigbee_delay = 0;
          if(BFCT_protocol_Zigbee.receive_data[4] == 0x03)
          {
            data_buff[1] = 0x99;
            data_buff[2] = 0x09;
            casual_work_No = 0x999;
            write_userdata2eeprom ( casual_work_addr,(u8 *)&casual_work_No,2);
          }
          zigbee_moni_state++;
      }
    break;
   case 2:
       ret = send_lcokcmd(0x2a,0x01,data_buff );
       if(ret == 1)
       {
          BFCT_protocol_Lock.receive_flag=0;
          zigbee_delay =0;
          zigbee_moni_state++;
       }
       else if(ret == 2)
       {
         zigbee_delay = 0;
         zigbee_moni_state =0;
         return 1;
       }
  break;
  case 3:
     if(BFCT_protocol_Lock.receive_flag==1)
     {
        BFCT_protocol_Lock.receive_flag=0;
        ret = 1; //check_lock_cmd(0x81);
        if(ret ==1)
        { 
            data_buff[0] = 0x01;data_buff[1] = 0x03;data_buff[2] = 0x33;data_buff[3] = 0x01;data_buff[4] = 0x06;
            zigbee_delay =0;
            zigbee_moni_state =10;
        }
        else
        {
          zigbee_delay =0;
          zigbee_moni_state = 0;
          return 1;
        }    
     }
     if(t_1ms)
     {
       zigbee_delay++;
       if(zigbee_delay >= (u16)1000){
#if DEBUG    
          lock_erro =  lock_erro | TIMEOUT_BIT;
#endif
          zigbee_delay =0;
          zigbee_moni_state=0;
       }
     }
   break;      
  case 10:      //��̬����
       ret = send_zigbeecmd(0X0c,0x50,data_buff);
       if(ret)
       {
         zigbee_moni_state = 0;    
         return 1;
       }
    break;
  default:
    break; 
  }
  return 0;
#else
  return 1;
#endif
}
/*****zigbee ɾ���û� *******/
u8 zigbee_delete_user(void)
{
  u8 ret;
  static u16  zigbee_delay = 0;
  static  u8  zigbee_moni_state =0;
  static u8 user_attribute =0,user_No =0;
  switch(zigbee_moni_state)
    {
      case 0:
           ret = send_zigbeecmd(0x01,ZIGBEE_CMD_ACK,data_buff);
           if(ret)
           {
             if(zigbee_erro){
                zigbee_moni_state =0;
                return 1;
             }
             user_attribute = BFCT_protocol_Zigbee.receive_data[4];
             user_No = BFCT_protocol_Zigbee.receive_data[5];
             zigbeedata_2_lockdata(zigbee_user_No,&data_buff[0],&(BFCT_protocol_Zigbee.receive_data[5]));
             data_buff[2] = 0XFF;
             zigbee_delay = 0;
             zigbee_moni_state++;         
           }
        break;
      
       case 1:
       ret = send_lcokcmd(0x04,LOCK_DELETE_USER,data_buff );
       if(ret == 1)
       {
          BFCT_protocol_Lock.receive_flag=0;
          zigbee_delay =0;
          zigbee_moni_state++;
       }
       else if(ret == 2)
       {
          BFCT_protocol_Zigbee.send_len = zigbee_protocal_component(BFCT_protocol_Zigbee.send_data,0x01,0x0a,data_buff); //data_buff = 0 0 0 0 0 0 0 0 0 0 0 0 
          BFCT_protocol_Zigbee.receive_enable=1;
          BFCT_protocol_Lock.receive_enable=0;
          zigbee_delay =0;
          zigbee_moni_state=10;
       }
        break; 
     case 2:
       if(BFCT_protocol_Lock.receive_flag==1)
       {
          GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast); //TXD
          BFCT_protocol_Lock.receive_flag=0;
          ret = check_lock_cmd(0x82);
          if(ret ==1)
          { 
            if(BFCT_protocol_Lock.receive_data[4] == 0x00){
              data_buff[0] = 0x02;data_buff[1] = user_attribute; data_buff[2] = user_No;
             BFCT_protocol_Zigbee.send_len = zigbee_protocal_component(BFCT_protocol_Zigbee.send_data,0x04,0x50,data_buff); // 
            }
            else
            BFCT_protocol_Zigbee.send_len = zigbee_protocal_component(BFCT_protocol_Zigbee.send_data,0x01,0x0a,data_buff); 
            
            BFCT_protocol_Zigbee.receive_flag =0;
            BFCT_protocol_Zigbee.receive_enable=1;
            BFCT_protocol_Lock.receive_enable=0;
            zigbee_delay = 0;
            zigbee_moni_state = 10;
          }
          else
          {
            BFCT_protocol_Zigbee.send_len = zigbee_protocal_component(BFCT_protocol_Zigbee.send_data,0x01,0x0a,data_buff); //data_buff = 0 0 0 0 0 0 0 0 0 0 0 0 
            BFCT_protocol_Zigbee.receive_enable=1;
            BFCT_protocol_Lock.receive_enable=0;
            zigbee_delay =0;
            zigbee_moni_state=10;
          }    
       }
       if(t_1ms)
       {
         zigbee_delay++;
         if(zigbee_delay >= (u16)1000){
           lock_erro =  lock_erro | TIMEOUT_BIT;
            BFCT_protocol_Zigbee.send_len = zigbee_protocal_component(BFCT_protocol_Zigbee.send_data,0x01,0x0a,data_buff); //data_buff = 0 0 0 0 0 0 0 0 0 0 0 0 
            BFCT_protocol_Zigbee.receive_enable=1;
            BFCT_protocol_Lock.receive_enable=0;
            zigbee_delay =0;
            zigbee_moni_state=10;
         }
       }
       break;
         
       case 10:
          if(BFCT_protocol_Zigbee.receive_flag == 1)
          {
            BFCT_protocol_Zigbee.receive_flag = 0;
            ret = check_zigbee_cmd(ZIGBEE_CMD_ACK);
            if(ret ==1)
            {
              zigbee_delay = 0;
              zigbee_moni_state =0;
              return 1;
            }
          }
         if(t_1ms)
         {
           zigbee_delay++;
           if(zigbee_delay > 500){
             zigbee_moni_state=0;
             zigbee_delay=0;
             lock_erro =  lock_erro | TIMEOUT_BIT;
             return 1;
           }
         }
         break;  
         
       default:
       break;
    }
  return 0;                                      
}
/*****zigbee ��������û� *******/
u8 zigbee_modify_user(void)
{
  u8 ret;
  ret = send_zigbeecmd(0x01,ZIGBEE_CMD_ACK,data_buff);
  return ret;
}
/*****zigbee ������ɹ� *******/
u8 zigbee_opennet_sucess()
{
  static u8 len,cmd;
  u8 ret;
  switch(zigbee_moni_state)
  {
  case 0:
#if defined(Fei_Bi) 
    cmd = ZIGBEE_CMD_OPENNET_SUCEESS;
    cmd_id = *((u32*)&BFCT_protocol_Zigbee.receive_data[3]);
    for_cmd_id(cmd_id,ACK,data_buff);
    len =1;
#elif defined(Nan_Jing)
    cmd = ZIGBEE_CMD_ACK;
    len =1;
#endif
    zigbee_moni_state ++;
    break;
  case 1:
     ret = send_zigbeecmd(len,cmd,data_buff);
     if(ret ==1) zigbee_moni_state =0;
    break;
  default:
    break; 
  }

  return ret;
}
/*****zigbee ʱ��ͬ�� *******/
u8 zigbee_clock_sync()
{
  u8 ret;

  switch(zigbee_moni_state)
  {
  case 0:
    data_buff[0]= BFCT_protocol_Zigbee.receive_data[9]; //��
    data_buff[1]= BFCT_protocol_Zigbee.receive_data[8]; //��
    data_buff[2]= BFCT_protocol_Zigbee.receive_data[7]; //ʱ
    data_buff[3]= BFCT_protocol_Zigbee.receive_data[6]; //��
    data_buff[4]= BFCT_protocol_Zigbee.receive_data[5]; //��
    data_buff[5]= BFCT_protocol_Zigbee.receive_data[4];  //��
    write_userdata2eeprom( zigbee_clock_addr,data_buff,6); //��flash дʱ��
    zigbee_moni_state ++;
    break;
    
  case 1:
       ret = send_zigbeecmd(0X01,ZIGBEE_CMD_ACK,data_buff);
       if(ret)
       {
         zigbee_moni_state = 0;    
         return 1;
       }
    break;
  default:
    break; 
  }
  return 0;
}
/*****zigbee ��ѯ�û���Ϣ *******/
u8 zigbee_inqure_userinfo(void)
{
  u8 ret,i;
  static u8 admin_No=0,ordinary_No=0,admin_No_exist =1,ordinary_No_exist;
  static u8 get_mes_num = 0,user_attri = 0;
  static u8 j=0;

  switch(zigbee_moni_state)
  {
  case 0:
       ret = send_zigbeecmd(0X01,ZIGBEE_CMD_ACK,data_buff);
       if(ret)
       {
         data_buff[0]=0;  data_buff[1]=0;  data_buff[2]=2;   
         user_attri = BFCT_protocol_Zigbee.receive_data[4]; //
         zigbee_moni_state++;
       }
    break;
    
   case 1:
      BFCT_protocol_Lock.send_len = lock_protocal_component(BFCT_protocol_Lock.send_data,0x04,LOCK_INQUIRY_USER_INFO,data_buff);
      zigbee_delay=0;
      BFCT_protocol_Lock.receive_enable=0;
      BFCT_protocol_Zigbee.receive_enable=0;
      USART_Cmd(USART1 , DISABLE);
      zigbee_moni_state++;
    break; 
  case 2:
     if( BFCT_protocol_Lock.send_len ==0) //�ȴ��������
     {
        BFCT_protocol_Lock.receive_enable=1;
        BFCT_protocol_Lock.receive_len =0;
        BFCT_protocol_Lock.receive_flag = 0;
        data3_tc = 101;
        zigbee_delay =0;
        zigbee_moni_state++;
     }
  
      if(BFCT_protocol_Lock.send_fail==1)
      {
        BFCT_protocol_Lock.send_fail =0; 
        BFCT_protocol_Zigbee.receive_enable=1;
        BFCT_protocol_Lock.receive_enable=0;
         data_buff[0] = 0xee;
         BFCT_protocol_Zigbee.send_len = zigbee_protocal_component(BFCT_protocol_Zigbee.send_data,0x02,0x3a,data_buff);
        zigbee_delay =0;
        zigbee_moni_state=0x05;
      }
  break;
  case 3:
     if(BFCT_protocol_Lock.receive_flag==1)
     {
        BFCT_protocol_Lock.receive_flag=0;
        ordinary_No =0;
        admin_No =0;
        GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast); //TXD
        ret = check_lock_cmd(ACK_LOCK_INQUIRY_USER_INFO);
        if(ret ==1)
        { 
          get_mes_num = BFCT_protocol_Lock.receive_data[4];
          for(i=0; i< BFCT_protocol_Lock.receive_data[4];i++)
          {
            if( BFCT_protocol_Lock.receive_data[i*33+15] & 0x80)
            {
              ordinary_No++;
              data_buff[51-ordinary_No*10] = 0x06;
              data_buff[51+1-ordinary_No*10]= u16_BCD_2_hex( *(u16*)&BFCT_protocol_Lock.receive_data[i*33+5]);
              data_buff[51+2-ordinary_No*10]=1;data_buff[51+3-ordinary_No*10]=1;
              data_buff[51+4-ordinary_No*10]=1;data_buff[51+5-ordinary_No*10]=1;
               data_buff[51+6-ordinary_No*10]=1;data_buff[51+7-ordinary_No*10]=1;
                data_buff[51+8-ordinary_No*10]=1;data_buff[51+9-ordinary_No*10]=1;
            }
            else
            {
              data_buff[1+admin_No*10] = 0x06;
              data_buff[2+admin_No*10]=u16_BCD_2_hex( *(u16*)&BFCT_protocol_Lock.receive_data[i*33+5]);
              data_buff[3+admin_No*10]=1;data_buff[4+admin_No*10]=1;
              data_buff[5+admin_No*10]=i;data_buff[6+admin_No*10]=0;
              data_buff[7+admin_No*10]=0;data_buff[8+admin_No*10]=0;
              data_buff[9+admin_No*10]=admin_No;data_buff[10+admin_No*10]=ordinary_No;
              admin_No++;
              
            }
          }
            
          BFCT_protocol_Zigbee.receive_enable=1;
          BFCT_protocol_Lock.receive_enable=0;
          zigbee_delay =0;
          zigbee_moni_state++;
        }
        else
        {        
          data_buff[0] = 0xee;
          BFCT_protocol_Zigbee.send_len = zigbee_protocal_component(BFCT_protocol_Zigbee.send_data,0x02,0x38,data_buff);
          BFCT_protocol_Zigbee.receive_enable=1;
          BFCT_protocol_Lock.receive_enable=0;
          zigbee_delay =0;
          zigbee_moni_state = 0x05;
        }    
     }
     if(t_1ms)
     {
       zigbee_delay++;
       if(zigbee_delay >= (u16)4000){
          zigbee_delay =0;
          zigbee_moni_state=0;
          return 1;
       }
     }
   break;
    case 4:
      if(user_attri == 0)
      {
        if(admin_No >0)
        {
           data_buff[0] = admin_No;
           admin_No_exist =1;
           BFCT_protocol_Zigbee.send_len = zigbee_protocal_component(BFCT_protocol_Zigbee.send_data,admin_No*10+2,0x38,data_buff); 
           ordinary_No =0;
        }
        else
        {
          if(get_mes_num < 2) {
            if(admin_No_exist)
            {
              j = 0;
             get_mes_num =0;
             admin_No_exist =0;
             zigbee_delay =0;
             zigbee_moni_state = 0;
             return 1;
            }else {
              data_buff[0] = 0xee;
              BFCT_protocol_Zigbee.send_len = zigbee_protocal_component(BFCT_protocol_Zigbee.send_data,0x02,0x38,data_buff);
            }
          }
          else
          {
             zigbee_delay =0;
             zigbee_moni_state = 6;
             return 0;
          }
        }
      }
      else if(user_attri == 1)
      {
          if(ordinary_No >0)
            
          {
             ordinary_No_exist =1;
             data_buff[50 - ordinary_No*10 ] = ordinary_No;
             BFCT_protocol_Zigbee.send_len = zigbee_protocal_component(BFCT_protocol_Zigbee.send_data,ordinary_No*10+2,0x39,&data_buff[50 - ordinary_No*10]);  
          }
          else{
            if(get_mes_num < 2) {
              if(ordinary_No_exist){
                 j = 0;
                 get_mes_num =0;
                 ordinary_No_exist =0;
                 zigbee_delay =0;
                 zigbee_moni_state = 0;
                 return 1;
              }else{
                   data_buff[0] = 0xee;
                   BFCT_protocol_Zigbee.send_len = zigbee_protocal_component(BFCT_protocol_Zigbee.send_data,0x02,0x39,data_buff);
              }
            }
            else
            {
               zigbee_delay =0;
               zigbee_moni_state = 6;
               return 0;
            }
          }
      }
      else
      {
          data_buff[0] = 0xee;
          BFCT_protocol_Zigbee.send_len = zigbee_protocal_component(BFCT_protocol_Zigbee.send_data,0x02,0x3a,data_buff);
      }
      zigbee_delay =0;
      zigbee_moni_state++;
      data3_tc = 101; 
      BFCT_protocol_Zigbee.receive_flag =0;
    break;

    case 5:
        if(BFCT_protocol_Zigbee.receive_flag == 1)
        {
          BFCT_protocol_Zigbee.receive_flag =0;
          ret = check_zigbee_cmd(ZIGBEE_CMD_ACK);
          if(ret ==1)
          {
            if(get_mes_num ==2 )
            {
              zigbee_moni_state++;
            }
            else
            {
                j = 0;
                get_mes_num =0;
                 zigbee_delay = 0;
                 zigbee_moni_state =0;
                 return 1;
            }
            if( user_attri  == 0x02)
            {
                j = 0;
                get_mes_num =0;
                 zigbee_delay = 0;
                 zigbee_moni_state =0;
                 return 1;
            }
            
          }
        }
       if(t_1ms)
       {
         zigbee_delay++;
         if(zigbee_delay > 1000){
           zigbee_moni_state=0;
           zigbee_delay=0;
           return 1;
         }
       }
    break;
  case 6:
        if(t_1ms)
       {
         zigbee_delay++;
         if(zigbee_delay > 300){
           j++;
           data_buff[0]=2*j;  data_buff[1]=0;  data_buff[2]=2; 
           zigbee_moni_state =0x01;
           zigbee_delay=0;
         }
       }    
    break;
        
  default:
    zigbee_moni_state = 0;  
    break; 
  }
  return 0;
}

/*****zigbee ��ѯ��״̬ *******/
u8 zigbee_inqure_lockstate()
{
  u8 ret;

  switch(zigbee_moni_state)
  {
  case 0:
       ret = send_zigbeecmd(0X01,ZIGBEE_CMD_ACK,data_buff);
       if(ret)
       {
         zigbee_delay =0;
         zigbee_moni_state++;
       }
    break;
    
   case 1:
       ret = send_lcokcmd(0x01,LOCK_CMD_INQURE_LOCK_STATE,data_buff );
       if(ret == 1)
       {
          BFCT_protocol_Lock.receive_flag=0;
          zigbee_delay =0;
          zigbee_moni_state++;
       }
       else if(ret == 2)
       {
        zigbee_delay =0;
        zigbee_moni_state=0;
        return 1;
       }

    break; 
  case 2:
      ret = get_lock_message();
      if(ret ==1)
      {
          ret = check_lock_cmd(ACK_LOCK_CMD_INQURE_LOCK_STATE);
          if(ret ==1)
          { 
            lockdata_2_zigbeedata(lcok_power_level_state,&(BFCT_protocol_Lock.receive_data[20]),&data_buff[0]); // power_level_state
            lockdata_2_zigbeedata(lock_anti_lock_state,&(BFCT_protocol_Lock.receive_data[20]),&data_buff[1]); // anti_lock_state
            lockdata_2_zigbeedata(lock_motor_state,&(BFCT_protocol_Lock.receive_data[20]),&data_buff[2]); // system_locked_state
            lockdata_2_zigbeedata(lock_square_tongue_locked_state,&(BFCT_protocol_Lock.receive_data[20]),&data_buff[3]); // square_tongue_locked_state

            zigbee_delay =0;
            zigbee_moni_state++;
          }
          else
          {
            zigbee_delay =0;
            zigbee_moni_state = 0;
            return 1;
          }    
      }
      else if(ret ==2)
      {
          zigbee_moni_state = 0;
          return 1;
      }
   break;
    case 3:
         ret = send_zigbeecmd(0x05,0x3B,data_buff);
         if(ret)
         {
            zigbee_delay = 0;
            zigbee_moni_state =0;
            return 1;
         }
    break;
        
  default:
    break; 
  }
  return 0;
}

u8 zigbee_amdin_identification(void)
{
  u8 ret;

  switch(zigbee_moni_state)
  {
  case 0:
       ret = send_zigbeecmd(0X01,ZIGBEE_CMD_ACK,data_buff);
       if(ret)
       {
         zigbee_moni_state ++;    
       }
    break;
    
  case 1:
       ret = send_zigbeecmd(0X01,0x05,data_buff);
       if(ret)
       {
         zigbee_moni_state = 0;    
         return 1;
       }
    break;
        
  default:
    zigbee_moni_state =0 ;
    break; 
  }
  return 0;
}
