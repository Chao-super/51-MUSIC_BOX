#ifndef _LED_H__
#define _LED_H__
#include <REG52.H>
// 共阳极数码管
#define LED_PORT P0
sbit LED_DIG1 = P2 ^ 7;
sbit LED_DIG2 = P2 ^ 6;

void LED_Init(void);
void LED_Display(unsigned int num);
void LED_Init_Display();

#endif // _LED_H__