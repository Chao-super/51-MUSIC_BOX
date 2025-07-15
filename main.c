#include <reg52.h>
#include "switch.h"
#include "LED.h"
#include "OLED.h"
#include "Delay.h"
#include "Timer.h"
#include "music.h"
#include "song.h"
#include "Settings.h"
#include "EEPROM.h"

#define song_num 5
unsigned int note_index = 0;

void main()
{
    Setting_Init();
    while (1) {
        UI_Update(Play_flag_fun(), volume, Loop_mode_fun(), note_index);    
        if (Cut_Scan(song_num)) {
            note_index = 0;  
            DATA_update(Song_choose);
        }

        if (Play_flag) {
            if (note_play_flag == 0) { // 只在上一个音符播放完后切换
                Playnote(song[Song_choose - 1], note_index);
                note_index++;
            }
            if (note_index >= song_len[Song_choose - 1]) {
                note_index = 0;
                if (Loop_mode == 0) Play_flag = 0;
                if (Loop_mode == 2) {
                    Song_choose++;
                    DATA_update(Song_choose);
                }
            }
        }
    }
}

// 定时器0中断服务程序
void Interrupt_Timer0() interrupt 1
{
    TR0 = 0;
    if (wave_flag) {
        Square_Wave(volume); // 方波
    } else {
        non_wave(); // 静音
    }
    TH0 = timer0H;
    TL0 = timer0L;
    TR0 = 1;
}

// 定时器1中断服务程序
void Interrupt_Timer1() interrupt 3
{
    TR1 = 0;
    TH1 = timer1H; // 重装高八位初值
    TL1 = timer1L; // 重装低八位初值
    duration();
}

// 定时器2中断服务程序
void Interrupt_Timer2() interrupt 5
{
    TF2 = 0; // 清除中断标志
    EC11_Scan();

    if (Init_flag) {
        LED_Init_Display();
    } else {
        LED_Display(Song_choose);
    }

    TR2 = 1; // 重新启动定时器2
}
