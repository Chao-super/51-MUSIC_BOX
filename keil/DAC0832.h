#ifndef __DAC0832_H__
#define __DAC0832_H__

#include <REG52.H>

// DAC0832的控制引脚定义

#define DAC0832_DATA P1   // 数据输出口
sbit DAC0832_CS = P2 ^ 0; // 片选信号

void DAC0832_Init(void);
void DAC0832_Write(unsigned char value);
unsigned char scale(unsigned int value);
void Square_Wave(unsigned int volume);
void non_wave(void);

#endif