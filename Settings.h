#ifndef _SETTING_H__
#define _SETTING_H__

#include "OLED.h"
#include "LED.h"
#include "DAC0832.h"
#include "Timer.h"
#include <REG52.H>
#include "delay.h"
#include "song.h"
#include "EEPROM.h"
#include "switch.h"
extern bit Init_flag;

void OLED_MusicBox_UI(bit is_play, unsigned char volume, unsigned char loop_mode);
void Setting_Init(void);
void UI_Update(bit is_play, unsigned char volume, unsigned char loop_mode, unsigned char index);
void DATA_update(unsigned char Song_choose);
#endif
