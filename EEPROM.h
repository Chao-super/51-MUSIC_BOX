#ifndef _EEPROM_H__
#define _EEPROM_H__

#include "delay.h"
#include <REG52.H>

sbit SDA = P3 ^ 6; // I2C数据线引脚
sbit SCL = P3 ^ 5; // I2C时钟线引脚

void EEP_I2c_Start(void);
void EEP_I2c_Stop(void);
unsigned char IIC_hostWait_Ack(void); // 等待ACK
void IIC_hostAck(void); // 发送ACK
void IIC_hostNAck(void); // 发送NACK    
unsigned char EEP_I2c_ReadByte(unsigned char ack); // 读取一个字节
void EEP_I2c_WriteByte(unsigned char txByte); // 写入一个字节
unsigned char AT24C64_ReadOneByte(unsigned int ReadAddr); // 读取一个字节
void AT24C64_WriteOneByte(unsigned int WriteAddr, unsigned char DataToWrite); // 写入一个字节

#endif