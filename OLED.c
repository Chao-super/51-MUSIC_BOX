#include "OLED.h"
#include "oledfont.h"

// IIC起始信号
void IIC_Start()
{
    OLED_SCLK_Set();
    OLED_SDA_Set();
    OLED_SDA_Clr(); // SDA拉低表示开始
    OLED_SCLK_Clr();
}

// IIC停止信号
void IIC_Stop()
{
    OLED_SCLK_Set();
    OLED_SDA_Clr();
    OLED_SDA_Set(); // SDA拉高表示停止
}

// IIC等待应答（本实现未实际检测应答，仅做时序延迟）
void IIC_Wait_Ack()
{
    OLED_SCLK_Set();
    OLED_SCLK_Clr();
}

// IIC写一个字节
void Write_IIC_Byte(unsigned char IIC_Byte)
{
    unsigned char i;
    unsigned char m, da;
    da = IIC_Byte;
    OLED_SCLK_Clr();
    for (i = 0; i < 8; i++) {
        m = da & 0x80; // 取最高位
        if (m == 0x80) {
            OLED_SDA_Set(); // 发送1
        } else {
            OLED_SDA_Clr(); // 发送0
        }
        da = da << 1;    // 左移一位
        OLED_SCLK_Set(); // 产生时钟上升沿
        OLED_SCLK_Clr(); // 产生时钟下降沿
    }
}

// IIC写命令
void Write_IIC_Command(unsigned char IIC_Command)
{
    IIC_Start();
    Write_IIC_Byte(0x78); // OLED从机地址
    IIC_Wait_Ack();
    Write_IIC_Byte(0x00); // 写命令
    IIC_Wait_Ack();
    Write_IIC_Byte(IIC_Command); // 发送命令
    IIC_Wait_Ack();
    IIC_Stop();
}

// IIC写数据
void Write_IIC_Data(unsigned char IIC_Data)
{
    IIC_Start();
    Write_IIC_Byte(0x78); // OLED从机地址
    IIC_Wait_Ack();
    Write_IIC_Byte(0x40); // 写数据
    IIC_Wait_Ack();
    Write_IIC_Byte(IIC_Data); // 发送数据
    IIC_Wait_Ack();
    IIC_Stop();
}

// OLED写一个字节（命令/数据）
void OLED_WR_Byte(unsigned dat, unsigned cmd)
{
    if (cmd) {
        Write_IIC_Data(dat); // 写数据
    } else {
        Write_IIC_Command(dat); // 写命令
    }
}

// 全屏填充
void fill_picture(unsigned char fill_Data)
{
    unsigned char m, n;
    for (m = 0; m < 8; m++) {
        OLED_WR_Byte(0xb0 + m, 0); // 设置页地址
        OLED_WR_Byte(0x02, 0);     // 设置列低地址
        OLED_WR_Byte(0x10, 0);     // 设置列高地址
        for (n = 0; n < 128; n++) {
            OLED_WR_Byte(fill_Data, 1); // 填充数据
        }
    }
}

// 设置显示位置
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WR_Byte(0xb0 + y, OLED_CMD);                       // 设置页地址
    OLED_WR_Byte((((x + 2) & 0xf0) >> 4) | 0x10, OLED_CMD); // 设置列高地址
    OLED_WR_Byte(((x + 2) & 0x0f), OLED_CMD);               // 设置列低地址
}

// 打开OLED显示
void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); // 设置DCDC命令
    OLED_WR_Byte(0X14, OLED_CMD); // DCDC ON
    OLED_WR_Byte(0XAF, OLED_CMD); // 显示ON
}

// 关闭OLED显示
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); // 设置DCDC命令
    OLED_WR_Byte(0X10, OLED_CMD); // DCDC OFF
    OLED_WR_Byte(0XAE, OLED_CMD); // 显示OFF
}

// 清屏函数，将整屏清空（全黑）
void OLED_Clear(void)
{
    u8 i, n;
    for (i = 0; i < 8; i++) {
        OLED_WR_Byte(0xb0 + i, OLED_CMD);                     // 设置页地址
        OLED_WR_Byte(0x02, OLED_CMD);                         // 设置列低地址
        OLED_WR_Byte(0x10, OLED_CMD);                         // 设置列高地址
        for (n = 0; n < 128; n++) OLED_WR_Byte(0, OLED_DATA); // 写0清屏
    }
}

// 全屏点亮
void OLED_On(void)
{
    u8 i, n;
    for (i = 0; i < 8; i++) {
        OLED_WR_Byte(0xb0 + i, OLED_CMD);                     // 设置页地址
        OLED_WR_Byte(0x02, OLED_CMD);                         // 设置列低地址
        OLED_WR_Byte(0x10, OLED_CMD);                         // 设置列高地址
        for (n = 0; n < 128; n++) OLED_WR_Byte(1, OLED_DATA); // 写1点亮
    }
}

// 在指定位置显示一个字符
// x:0~127, y:0~63, Char_Size:字体大小(16/12)
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 Char_Size)
{
    unsigned char c = 0, i = 0;
    c = chr - ' '; // 计算字符在字库中的偏移
    if (x > Max_Column - 1) {
        x = 0;
        y = y + 2;
    }
    if (Char_Size == 16) {
        OLED_Set_Pos(x, y);
        for (i = 0; i < 8; i++)
            OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA); // 上半部分
        OLED_Set_Pos(x, y + 1);
        for (i = 0; i < 8; i++)
            OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA); // 下半部分
    } else {
        OLED_Set_Pos(x, y);
        for (i = 0; i < 6; i++)
            OLED_WR_Byte(F6x8[c][i], OLED_DATA); // 6x8字体
    }
}

// 幂运算函数，返回m的n次方
u32 oled_pow(u8 m, u8 n)
{
    u32 result = 1;
    while (n--) result *= m;
    return result;
}

// 显示数字
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size)
{
    u8 t, temp;
    u8 enshow = 0;
    for (t = 0; t < len; t++) {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                OLED_ShowChar(x + (size / 2) * t, y, ' ', size); // 前导空格
                continue;
            } else
                enshow = 1;
        }
        OLED_ShowChar(x + (size / 2) * t, y, temp + '0', size); // 显示数字
    }
}

// 显示字符串
void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 Char_Size)
{
    unsigned char j = 0;
    while (chr[j] != '\0') {
        OLED_ShowChar(x, y, chr[j], Char_Size); // 显示单个字符
        x += 8;
        if (x > 120) {
            x = 0;
            y += 2;
        }
        j++;
    }
}

// 显示汉字
void OLED_ShowCHinese(u8 x, u8 y, u8 no)
{
    u8 t, adder = 0;
    OLED_Set_Pos(x, y);
    for (t = 0; t < 16; t++) {
        OLED_WR_Byte(Hzk[2 * no][t], OLED_DATA); // 显示汉字上半部分
        adder += 1;
    }
    OLED_Set_Pos(x, y + 1);
    for (t = 0; t < 16; t++) {
        OLED_WR_Byte(Hzk[2 * no + 1][t], OLED_DATA); // 显示汉字下半部分
        adder += 1;
    }
}


// 初始化SSD1306 OLED
void OLED_Init(void)
{
    OLED_WR_Byte(0xAE, OLED_CMD); //--display off
    OLED_WR_Byte(0x02, OLED_CMD); //---set low column address
    OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
    OLED_WR_Byte(0x40, OLED_CMD); //--set start line address
    OLED_WR_Byte(0xB0, OLED_CMD); //--set page address
    OLED_WR_Byte(0x81, OLED_CMD); // contract control
    OLED_WR_Byte(0xFF, OLED_CMD); //--128
    OLED_WR_Byte(0xA1, OLED_CMD); // set segment remap
    OLED_WR_Byte(0xA6, OLED_CMD); //--normal / reverse
    OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3F, OLED_CMD); //--1/32 duty
    OLED_WR_Byte(0xC8, OLED_CMD); // Com scan direction
    OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset
    OLED_WR_Byte(0x00, OLED_CMD); //
    OLED_WR_Byte(0xD5, OLED_CMD); // set osc division
    OLED_WR_Byte(0x80, OLED_CMD); //
    OLED_WR_Byte(0xD8, OLED_CMD); // set area color mode off
    OLED_WR_Byte(0x05, OLED_CMD); //
    OLED_WR_Byte(0xD9, OLED_CMD); // Set Pre-Charge Period
    OLED_WR_Byte(0xF1, OLED_CMD); //
    OLED_WR_Byte(0xDA, OLED_CMD); // set com pin configuartion
    OLED_WR_Byte(0x12, OLED_CMD); //
    OLED_WR_Byte(0xDB, OLED_CMD); // set Vcomh
    OLED_WR_Byte(0x30, OLED_CMD); //
    OLED_WR_Byte(0x8D, OLED_CMD); // set charge pump enable
    OLED_WR_Byte(0x14, OLED_CMD); //
    OLED_WR_Byte(0xAF, OLED_CMD); //--turn on oled panel
}

// 在指定位置画点
void OLED_DrawPoint(unsigned char x, unsigned char y, unsigned char t)
{
    unsigned char page, bit_mask;
    if (x > 127 || y > 63) return; // 越界保护

    page     = y / 8;        // 计算页地址
    bit_mask = 1 << (y % 8); // 计算位掩码

    OLED_Set_Pos(x, page);

    if (t) {
        // 点亮该点（注意：此方法会清除同一页其它点）
        OLED_WR_Byte(bit_mask, OLED_DATA);
    } else {
        // 熄灭该点（会清除同一页所有点）
        OLED_WR_Byte(0x00, OLED_DATA);
    }
}