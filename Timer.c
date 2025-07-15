#include <REG52.H>
#include "Timer.h"

/**
 * @brief  定时器1初始化，1毫秒
 * @param  无
 * @retval 无
 */
void Beat_Timer_Init(void)
{
    unsigned int tmp;
    TMOD &= 0x0F; // 设置定时器模式
    TMOD |= 0x10; // 设置定时器模式
    TF1 = 0;      // 清除TF1标志
    TR1 = 1;      // 定时器1开始计时
    ET1 = 1;
    EA  = 1;
}

void Sound_Timer_Init(void)
{
    TMOD &= 0xF0;
    TMOD |= 0x01;
    ET0 = 1;
    TR0 = 0;
    EA  = 1;
}

void Loop_Timer_Init(void)
{
    unsigned int tmp;
    T2CON  = 0;                                // 定时器2模式
    tmp    = 65536 - 5 * 11059200 / 12 / 1000; // 1ms
    RCAP2H = (unsigned char)(tmp >> 8);
    RCAP2L = (unsigned char)tmp;
    TH2    = RCAP2H;
    TL2    = RCAP2L;
    ET2    = 1; // 使能定时器2中断
    TR2    = 1; // 启动定时器2
    EA     = 1;
}
