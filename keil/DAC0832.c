#include <REG52.H>
#include "DAC0832.h"
#include "delay.h"

/// @brief 向DAC0832写入数据
/// @param value 要写入的数据(此处为音量)
void DAC0832_Write(unsigned char value)
{
    DAC0832_CS   = 1;     // 先拉高CS，准备写入
    DAC0832_DATA = value; // 输出数据到P1
    DAC0832_CS   = 0;     // CS拉低，锁存数据
    delay_us(10);         // 可加短延时，确保锁存
    DAC0832_CS = 1;       // CS恢复高电平
}

/// @brief 初始化DAC0832
void DAC0832_Init(void)
{

    DAC0832_CS   = 1;   // CS高，初始化为不选中状态
}

unsigned char scale(unsigned int value)
{
    return (unsigned char)(value * 2.55);
}
void Square_Wave(unsigned int volume)
{
    static bit wave_flag;
    if (wave_flag) {
        DAC0832_Write(0x00);
    } else {
        DAC0832_Write(scale(volume));
    }
    wave_flag = !wave_flag;
}
void non_wave(void)
{
    DAC0832_Write(0x00);
}