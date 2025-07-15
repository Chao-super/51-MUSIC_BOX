#ifndef __MUSIC_H__
#define __MUSIC_H__

#include <REG52.H>
#include "DAC0832.h"

extern unsigned char timer0H, timer0L, timer1H, timer1L;
extern bit wave_flag;
extern bit note_play_flag;

void Playnote(const unsigned char *song, int index);
void startplay(void);
void duration(void);

#endif
