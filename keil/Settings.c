#include "Settings.h"

const unsigned char code play_icon[]        = " >";
const unsigned char code pause_icon[]       = "||";
const unsigned char code loop_mode_str[][8] = {"Single", "Single", " List "};
const unsigned char code Loop_icon[]        = "LOOP";
const unsigned char code VOL_icon[]         = "VOL:";
const unsigned char code Loading_icon[]     = "Loading";

bit Init_flag = 1;
/**
 * @brief OLED音乐盒主界面
 * @param song_idx   当前歌曲编号（从0开始）
 * @param is_play    1=播放 0=暂停
 * @param volume     当前音量
 * @param loop_mode  0=单曲 1=单曲循环 2=列表循环
 * @param progress   当前进度（0~100）
 */
void OLED_MusicBox_UI(bit is_play, unsigned char volume, unsigned char loop_mode)
{
    OLED_ShowString(0, 0, VOL_icon, 8);
    OLED_ShowString(72, 3, loop_mode_str[loop_mode], 10);
    if (loop_mode == 0) {
        OLED_ShowString(80, 4, "    ", 16);
    } else {
        OLED_ShowString(80, 4, Loop_icon, 16);
    }
    if (is_play) {
        OLED_ShowString(96, 0, play_icon, 16);
    } else {
        OLED_ShowString(96, 0, pause_icon, 16);
    }
}

void UI_Update(bit is_play, unsigned char volume, unsigned char loop_mode, unsigned char index)
{
    unsigned int i, bar_len;
    static int last_volume             = 0;
    static unsigned int last_play_flag = 0;
    static unsigned int last_loop_mode = 0;
    static unsigned int last_progress  = 0;
    int progress                       = (index * 100) / song_len[Song_choose - 1];

    if (last_play_flag != is_play) {
        if (is_play)
            OLED_ShowString(96, 0, play_icon, 16);
        else
            OLED_ShowString(96, 0, pause_icon, 16);
        last_play_flag = is_play;
    }

    if (volume != last_volume) {
        OLED_ShowNum(30, 0, volume / 100, 1, 8);
        OLED_ShowNum(38, 0, volume / 10 % 10, 1, 8);
        OLED_ShowNum(46, 0, volume % 10, 1, 8);

        AT24C64_WriteOneByte(0x0001, (unsigned char)volume);
        last_volume = volume;
    }

    if (loop_mode != last_loop_mode) {
        OLED_ShowString(72, 3, loop_mode_str[loop_mode], 10);
        if (loop_mode) {
            OLED_ShowString(80, 4, Loop_icon, 16);
        } else {
            OLED_ShowString(80, 4, "    ", 16);
        }
        AT24C64_WriteOneByte(0x0002, (unsigned char)loop_mode);
        last_loop_mode = loop_mode;
    }
    // 进度条
    if (progress != last_progress) {
        bar_len = progress;
        for (i = 0; i < 100; i++)
            OLED_DrawPoint(12 + i, 63, i < bar_len ? 1 : 0);
        last_progress = progress;
    }
}
void DATA_update(unsigned char Song_choose)
{

    AT24C64_WriteOneByte(0x0000, (unsigned char)Song_choose);
}

void UI_Init()
{
    OLED_Init();
    OLED_Clear();
    OLED_ShowCHinese(18, 1, 0);
    OLED_ShowCHinese(36, 1, 1);
    OLED_ShowCHinese(54, 1, 2);
    OLED_ShowCHinese(72, 1, 3);
    OLED_ShowCHinese(90, 1, 4);
    OLED_ShowString(24, 5, Loading_icon, 8);
    OLED_ShowString(80, 5, ".", 8);
    delay_ms(200);
    OLED_ShowString(88, 5, ".", 8);
    delay_ms(200);
    OLED_ShowString(96, 5, ".", 8);
    delay_ms(200);
    OLED_Clear();
    OLED_MusicBox_UI(0, volume, Loop_mode);
}
void Data_Init()
{
    Song_choose = (unsigned int)AT24C64_ReadOneByte(0x0000);
    volume      = (unsigned int)AT24C64_ReadOneByte(0x0001);
    Loop_mode   = (signed int)AT24C64_ReadOneByte(0x0002);
}
void Setting_Init(void)
{
    Init_flag = 1;
    Sound_Timer_Init();
    Loop_Timer_Init();
    Beat_Timer_Init();
    LED_Init();
    DAC0832_Init();
    Data_Init();
    UI_Init();
    Init_flag = 0;
}