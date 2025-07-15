#ifndef __SWITCH_H__
#define __SWITCH_H__

#include <REG52.H>
#include "delay.h"
// EC11的A相波、B相波、按键分别接在P2.3、P2.4、P2.5
sbit EC11_A   = P2 ^ 3; // EC11时钟线
sbit EC11_B   = P2 ^ 4; // EC11信号线
sbit EC11_Key = P2 ^ 5; // EC11旋转编码器的按键

// 上一首、下一首、循环播放独立按键分别接在P3.2、P3.3、P3.4
sbit PREV_KEY = P3 ^ 3;
sbit NEXT_KEY = P3 ^ 4;
sbit LOOP_KEY = P3 ^ 2;

extern int Song_choose;
extern signed int volume;//用unsigned int防止无符号整型下溢
extern bit Play_flag; // 播放标志
extern unsigned int Loop_mode; // 循环模式

// 歌曲长度数组
void EC11_Action(signed char num); // EC11动作函数声明
void EC11_Scan();
bit KEY_pressed(bit key, bit close_state); // 按键检测
bit Play_flag_fun(void);                       // 播放标志
signed char Loop_mode_fun(void);             // 循环标志
bit Cut_Scan(unsigned int max);  // 歌曲选择

#endif