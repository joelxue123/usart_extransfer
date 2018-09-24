#include "protocol.h"
#include "global.h"
#include "userdata.h"
#include "command.h"
#include "bsp.h"


u8 exe_zigbee_cmd(u8 zigbee_cmd)
{
  u8  ret;
  
    if(zigbee_cmd==ZIGBEE_CMD_ACK)
    { 
      return 1;
    }
    else if(zigbee_cmd==ZIGBEE_CMD_ADD_USER)  //添加用户
    {
      ret = zigbee_adduser();
    }
    else if(zigbee_cmd==ZIGBEE_CMD_DELETE_USER)  //删除用户
    {
      ret = zigbee_delete_user();
    }
    else if(zigbee_cmd==ZIGBEE_CMD_MODIFY_USER) //修改用户
    {
      ret = zigbee_modify_user();
    }
    else if(zigbee_cmd==ZIGBEE_CMD_OPENNET_FAIL)
    {
      ret = zigbee_opennet_fail();
    }
    else if(zigbee_cmd==ZIGBEE_CMD_OPENNET_SUCEESS)
    {
      ret = zigbee_opennet_sucess();
    }
    else if(zigbee_cmd==ZIGBEE_CMD_OPENLOCK)
    {
      ret = zigbee_openlock(); 
    }
    else if(zigbee_cmd==ZIGBEE_CMD_CLOCK_SYNC)
    {
      ret = zigbee_clock_sync();
    }
    else if(zigbee_cmd==ZIGBEE_CMD_INQURE_USERINFO)
    {
      ret = zigbee_inqure_userinfo();
    }
    else if(zigbee_cmd==ZIGBEE_CMD_INQURE_LOCK_STATE)
    {
      ret = zigbee_inqure_lockstate();
    }
    else if(zigbee_cmd==ZIGBEE_CMD_ADMIN_IDENTIFICATION)
    {
       ret = zigbee_amdin_identification();
    }
#if defined(Nan_Jing)
    else if(zigbee_cmd==ZIGBEE_CMD_OPENNET_FAIL_2)
    {
       ret = zigbee_opennet_fail_2();
    }
#endif
#if defined(Fei_Bi)
    else if(zigbee_cmd==ZIGBEE_CMD_ONLINE_INFO)
    {
       ret = zigbee_online_info();
    }

#endif
    else 
    {
      ret =1;
    }
    
    return ret;
}


u8 exe_lock_cmd(u8 lock_cmd)
{
  u8 ret;
  
  
    if(lock_cmd==LOCK_OPENNET)
    {
      ret = lock_opennet();
    }
    else if(lock_cmd==LOCK_CLOSENET)
    {
      ret = lock_closenet();
    }
    else if(lock_cmd==LOCK_LOCK_BE_OPENED)
    {
      ret =  lock_be_opened();
    }
    else if(lock_cmd==LCOK_LOCK_BE_CLOSED)
    {
      ret =  lock_be_closed();
    } 
    else if(lock_cmd==LOCK_OPENLOCK_BYFORCE)
    {
      ret = lock_openclock_byforce();
    }
    else if(lock_cmd==LOCK_LOGIN_SAFE_MODE)
    {
      ret = lock_login_safemode();
    }
    else if(lock_cmd==LOCK_LOGOUT_SAFE_MODE)
    {
      ret = lock_logout_safe_mode();
    }
    else if(lock_cmd==LOCK_LOGOUT_SETTING_MODE)
    {
      ret = lock_logout_settting_mode();
    }
    else if(lock_cmd==LOCK_USER_ADDED)
    {
      ret =lock_add_user();
    }
    else if(lock_cmd==LOCK_STATE_UPDATA)
    {
      ret = lock_state_updata();
    }
    else if(lock_cmd==LOCK_delete_USER)
    {
      ret =lock_delete_user();
    }    
    else if(lock_cmd==LOCK_ALARM)
    {
#if defined(Fei_Bi)
      ret =lock_alarm();
#elif defined(Nan_Jing)
      ret =1;
#endif
    }
    else if(lock_cmd==LOCK_illgal_user_REPORT) //非法开锁 锁上报
    {
      ret = lolck_illage_user_report();
    }
    else if(lock_cmd==LOCK_FORMAT) //非法开锁 锁上报
    {
      ret = lock_format();
    }
    else 
    {
      ret =1;
    }
    
    return ret;
}