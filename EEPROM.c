#include "EEPROM.h"

void EEP_I2c_Start(void)
{
    SDA = 1;
    delay_us(5);
    SCL = 1;
    delay_us(5);
    SDA = 0;
    delay_us(5);
    SCL = 0;
    delay_us(5);
}

void EEP_I2c_Stop(void)
{
    SCL = 0;
    delay_us(5);
    SDA = 0;
    delay_us(5);
    SCL = 1;
    delay_us(5);
    SDA = 1;
    delay_us(5);
}

unsigned char IIC_hostWait_Ack(void) // ack creat from slave
{
    unsigned char ucErrTime = 0;

    SDA = 1;
    delay_us(5);
    SCL = 1;
    delay_us(5);
    SCL = 1;
    delay_us(5);
    while (SDA == 1) {
        ucErrTime++;
        if (ucErrTime > 250) {
            EEP_I2c_Stop();
            return 1;
        }
    }
    SCL = 0;
    return 0;
}

// 产生ACK应答 // 4
void IIC_hostAck(void)
{
    SCL = 0;
    SDA = 0;
    delay_us(5);
    SCL = 1;
    delay_us(5);
    SCL = 0;
}

// 不产生ACK应答 // 5
void IIC_hostNAck(void)
{
    SCL = 0;
    SDA = 1;
    delay_us(5);
    SCL = 1;
    delay_us(5);
    SCL = 0;
}

void EEP_I2c_WriteByte(unsigned char txByte)
{
    unsigned char count;
    SCL = 0;
    for (count = 0; count < 8; count++) // shift bit for masking (8 times)
    {

        if ((txByte << count) & 0x80)
            SDA = 1; // masking txByte, write bit to SDA-Line
        else
            SDA = 0;
        delay_us(10); // data set-up time (t_SU;DAT)
        SCL = 1;       // generate clock pulse on SCL
        delay_us(10); // SCL high time (t_HIGH)
        SCL = 0;
        delay_us(10);
    }

    // check slave ack after send write
}

// 产生ACK应答 // 7
unsigned char EEP_I2c_ReadByte(unsigned char ack)
{

    unsigned char rxByte;
    unsigned char count;
    rxByte = 0x00;
    // release SDA-line
    for (count = 0; count < 8; count++) // shift bit for masking (8 times)
    {
        SCL = 0; // start clock on SCL-line
        delay_us(5);
        SCL = 1;
        // SCL high time (t_HIGH)
        rxByte <<= 1;
        if (SDA == 1)
            rxByte |= 1; // read bit
        delay_us(5);   // data hold time(t_HD;DAT)
    }

    // SDA=1;            // release SDA-line
    if (!ack)
        IIC_hostNAck(); // 发送nACK
    else
        IIC_hostAck(); // 发送ACK
    delay_us(5);
    return rxByte; // return with no error
}

/*-------------------------------AT24c64---------------------------------------------*/

unsigned char AT24C64_ReadOneByte(unsigned int ReadAddr)
{
    unsigned char temp = 0;
    EEP_I2c_Start();

    EEP_I2c_WriteByte(0XA0); // 发送读命令
    IIC_hostWait_Ack();
    EEP_I2c_WriteByte(ReadAddr >> 8); // 发送高地址
    IIC_hostWait_Ack();

    EEP_I2c_WriteByte(ReadAddr & 0xFF); // 低8位
    IIC_hostWait_Ack();
    EEP_I2c_Start();
    EEP_I2c_WriteByte(0XA1); // 进入接收模式
    IIC_hostWait_Ack();
    temp = EEP_I2c_ReadByte(0);
    EEP_I2c_Stop(); // 产生一个停止条件
    delay_us(5);
    return temp;
}
// 9
// AT24CXX  Byte Write
// WriteAddr  :写入数据的目的地址
// DataToWrite:要写入的数据
void AT24C64_WriteOneByte(unsigned int WriteAddr, unsigned char DataToWrite)
{
    unsigned char i;
    EEP_I2c_Start();

    EEP_I2c_WriteByte(0xA0); // 设备地址+写
    IIC_hostWait_Ack();
    EEP_I2c_WriteByte((WriteAddr >> 8) & 0xFF); // 高8位
    IIC_hostWait_Ack();
    EEP_I2c_WriteByte(WriteAddr & 0xFF);        // 低8位
    IIC_hostWait_Ack();
    EEP_I2c_WriteByte(DataToWrite);               // 数据
    IIC_hostWait_Ack();
    EEP_I2c_Stop();
    delay_ms(10); // 写后延时
}

