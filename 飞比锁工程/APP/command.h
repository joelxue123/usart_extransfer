#ifndef __COMMAND_H
#define __COMMAND_H
#include "stm8l15x.h"

/*********������������*********/
u8 sum_check(u8 *p,u16 len);
u8 xor_check(u8 *p,u16 len);
void encryption(void);
u8 crc_8n(unsigned char *ptr,unsigned char n);



/********lock�ظ�����********************/
u8 lock_opennet(void);
u8 lock_closenet(void);
u8 lock_login_safemode(void);
u8 lock_logout_safe_mode(void);
u8 lock_logout_settting_mode(void);
u8 lock_openclock_byforce(void);
u8 lock_state_updata(void);
u8 lock_add_user(void);
u8 lock_be_opened(void);
u8 lock_be_closed(void);
u8 lolck_illage_user_report(void);
u8 lock_delete_user(void);
u8 lock_format(void);
u8  lock_alarm(void);

/***********zigbee�ظ�����******************/

u8 zigbee_opennet_sucess();
u8 zigbee_clock_sync();
u8 zigbee_openlock(void);
u8 zigbee_opennet_fail();
u8 zigbee_adduser();
u8 zigbee_inqure_userinfo();
u8 zigbee_inqure_lockstate();
u8 zigbee_amdin_identification(void);
u8 zigbee_modify_user(void);
u8 zigbee_delete_user(void);
u8 zigbee_opennet_fail_2(void);
u8 zigbee_opennet_fail();
u8 zigbee_online_info(void);
#endif 