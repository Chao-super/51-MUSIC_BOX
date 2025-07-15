#include "LED.h"
#include "delay.h"

char code table[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};//段码表

void LED_Init(void)
{
    LED_PORT = 0x255; // Set all LED pins to output
    LED_DIG1 = 0;
    LED_DIG2 = 0;
}

void LED_Display(unsigned int num)
{
    static bit dig = 0;
    if (num < 10) {
        if (dig == 0) {
            LED_DIG1 = 1;
            LED_DIG2 = 0;
            LED_PORT = table[0];
        } else {
            LED_DIG1 = 0;
            LED_DIG2 = 1;
            LED_PORT = table[num];
        }
    } else if (num > 10) {
        if (dig == 0) {
            LED_DIG1 = 1;
            LED_DIG2 = 0;
            LED_PORT = table[num / 10];
        } else {
            LED_DIG1 = 0;
            LED_DIG2 = 1;
            LED_PORT = table[num % 10];
        }

    } else if (num > 99) {
        if (dig == 0) {
            LED_DIG1 = 1;
            LED_DIG2 = 0;
            LED_PORT = 0xbf;
        } else {
            LED_DIG1 = 0;
            LED_DIG2 = 1;
            LED_PORT = 0xbf;
        }
    }
    dig = !dig;
}

/// @brief 初始化显示HI
void LED_Init_Display()
{
    static bit dig = 0;
    if (dig == 0) {
        LED_DIG1 = 1;
        LED_DIG2 = 0;
        LED_PORT = 0x89;
    } else {
        LED_DIG1 = 0;
        LED_DIG2 = 1;
        LED_PORT = table[1];
    }
    dig = !dig;
}