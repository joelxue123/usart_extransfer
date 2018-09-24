#ifndef __EEPROM_H
#define __EEPROM_H
#include "stm8l15x.h"


extern void EEPROM_multbyte_read(u32 start_address , u8 *data,u8 len);
void EEPROM_multbyte_write(u32 start_address , u8 *data,u8 len);
<<<<<<< HEAD
void write_userdata2eeprom(u16 addr, u8 *ptr, u8 len);
void read_userdata4eeprom(u16 addr, u8 *ptr, u8 len);
=======

>>>>>>> e22c336f0c9dd1b56581e59a67f0d47c2ac06c3e
#endif