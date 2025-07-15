#include "delay.h"
#include <INTRINS.H>
#include <REG52.H>

/// @brief 毫秒级的延时函数
/// @param ms 需要延时的毫秒数
void delay_ms(unsigned int ms)
{
    unsigned char data i, j;
    while (ms--) {
        _nop_();
        _nop_();
        _nop_();
        i = 11;
        j = 190;
        do {
            while (--j);
        } while (--i);
    }
}

/// @brief 微秒级的延时函数
/// @param us 需要延时的微秒数`
void delay_us(unsigned int us)
{
    while (us--) {
        _nop_();
        _nop_();
        _nop_();
    }
}
