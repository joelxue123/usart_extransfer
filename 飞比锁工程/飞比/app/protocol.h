#ifndef __PROTOCOL_H
#define __PROTOCOL_H
#include "stm8l15x.h"

enum{
  REQUEST = 0x00, // �����ϱ�
  ACK = 0x01      //Ӧ��
};

#define  protocal_header (0xaa)
#define  protocal_tail (0x55)
#define protocal_len (((BFCT_protocol_Zigbee.receive_data[1] + 9)) 

#define UART_CMD_ADDR 1
#define DATA_ADDR 4


/********* zigbee ģ��Э�� �궨�� **************/

#define UART_CMD_ADDR 1
#define DATA_ADDR 4


/********* zigbee ģ��Э�� �궨�� **************/
#define ZIGBEE_CMD_ADDR 0x02

#define CMD_CRC_ERRO 1
#define CMD_LEN_ERRO 1
#define ZIGBEE_CMD_ACK 0x00

#define ZIGBEE_CMD_ADD_USER 0x13  
#define ZIGBEE_CMD_DELETE_USER 0x14  
#define ZIGBEE_CMD_MODIFY_USER 0x15  

#define ZIGBEE_CMD_OPENNET_FAIL 0x95
#define ZIGBEE_CMD_OPENNET_SUCEESS 0x83
#define ZIGBEE_CMD_ONLINE_INFO  0X84

#define ZIGBEE_CMD_WAKEUP_MODULE 0X77

#define ZIGBEE_CMD_OPENLOCK_BYFORCE 0x22

#define ZIGBEE_CMD_OPENLOCK 0X60
#define ZIGBEE_CMD_LOCAL_OPEN_LOCK 0x80
#define ZIGBEE_CMD_OPENNET 0x40
#define ZIGBEE_CMD_CLOSENET 0x41
#define ZIGBEE_CMD_LOW_POWER 0x30
#define ZIGBEE_CMD_CLOCK_SYNC 0x62
#define ZIGBEE_CMD_ADMIN_IDENTIFICATION 0x11
#define ZIGBEE_CMD_INQURE_USERINFO 0x12
#define ZIGBEE_CMD_INQURE_LOCK_STATE 0x17
#define ZIGBEE_CMD_ADDUSER_FROM_LOCK 0x33

#define ACK_ZIGBEE_ILLAGEL_USER_REPORT 0x20
#define ACK_ZIGBEE_CMD_INQURE_LOCK_STATE  0x3B
#define ACK_ZIGBEE_CMD_LOW_POWER 0X30
#define ACK_ZIGBEE_CMD_UPLOAD 0X3D
#define ACK_ZIGBEE_CMD_DELETE_USER  0X34
#define ACK_ZIGBEE_MODIFY_USERINFO 0x35
#define ACK_ZIGBEE_FORMAT 0x36
#define ACK_ZIGBEE_CMD_LOCAL_OPEN_LOCK 0x80
#define ACK_ZIGBEE_CMD_OPENNET_SUCESS 0x40
#define ACK_ZIGBEE_CMD_OPENLOCK 0x54
/********* zigbee ģ��Э�� �궨�� **************/

/******* ��Э��궨�� ************************/

#define LOCK_CMD_ADDR 0x03

#define LOCK_DELETE_USER 0x02
#define LOCK_INQUIRY_USER_INFO 0x12
#define LOCK_LOCK_BE_OPENED 0X15
#define LOCK_OPENNET 0X1C
#define LOCK_CLOSENET 0X1D
#define LOCK_OPENLOCK 0x06
#define LOCK_OPENLOCK_BYFORCE 0x1A
#define LOCK_LOGIN_SAFE_MODE 0x1B
#define LOCK_LOGOUT_SAFE_MODE 0x9B
#define LOCK_LOGOUT_SETTING_MODE 0x26
#define LOCK_ALARM 0X1E

#define LOCK_USER_ADDED 0x21
#define LOCK_FORMAT 0x23
#define LCOK_LOCK_BE_CLOSED 0X16
#define LOCK_STATE_UPDATA 0X17
#define LOCK_illgal_user_REPORT 0x18
#define LOCK_delete_USER 0x24
#define LOCK_ADD_USER_FROM_WIRELESS 0X01
#define LOCK_CLOCK_CALIBRATION 0X08
#define LOCK_CMD_INQURE_LOCK_STATE 0x09

#define ACK_LOCK_INQUIRY_USER_INFO 0x92
#define ACK_LOCK_CMD_INQURE_LOCK_STATE  0X89
#define ACK_LOCK_OPENLOCK 0X86
#define ACK_OPENLOCK_BYFORCE 0X9a
#define ACK_LOCK_OPENNET 0X9c

/******* ��Э��궨�� ************************/



extern const u8 key[];
extern u8 password[6];
extern u8 npass[6];
extern u8 remote_open_flag;
extern u32 cmd_id ;
/******��������ӿ� **********************/
u8 exe_zigbee_cmd(u8 zigbee_cmd);
u8 exe_lock_cmd(u8 lock_cmd);

u8 send_zigbeecmd(u8 len,u8 zigbeecmd,u8 *data_buff );
u8 check_zigbee_cmd(u8 zigbee_cmd);
u8  zigbee_protocal_component(u8 *buf,u16 len,u8 cmd,u8* data_buff);
u8 check_code(u8 *buf);
void for_cmd_id(u32 cmd_id,u8 cmd_type,u8 *buf);
u8 zigbee_usart_send(void);

#endif 