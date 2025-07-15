#include "music.h"
#include "delay.h"

unsigned char timer0H, timer0L, timer1H, timer1L;
unsigned int beat = 1;
unsigned int beat_time; // 发出音符的周期
unsigned int speed     = 200;
bit note_play_flag     = 0;                            // 音符播放标志
unsigned int timer_1ms = 65536 - 11059200 / 12 / 1000; // 1ms
unsigned int timer_50ms = 65536 - 50*11059200 / 12 / 1000;
unsigned int note_duration_max;
unsigned int note_duration_count = 0;
bit wave_flag                     = 0;

code unsigned int Frequency[] = {
    0,
    63777, 63969, 64140, 64216, 64360, 64489, 64607, // 低音
    64655, 64751, 64837, 64876, 64948, 65012, 65070, // 中音
    65095, 65144, 65186, 65206, 65242, 65274, 65303, // 高音
};

// 播放一首歌，song为乐谱数组，index为索引
void Playnote(const unsigned char *song_array, int index)
{
    unsigned char k;

    k    = song_array[3 * index] + 7 * song_array[3 * index + 1]; // 计算音阶索引
    beat = song_array[3 * index + 2];
    if (k != 0) {
        timer0H   = Frequency[k] / 256; // 查表赋值
        timer0L   = Frequency[k] % 256;
        wave_flag = 1;
    } else {
        timer0H   = (unsigned char)(timer_1ms >> 8); // 休止符半周期为1ms
        timer0L   = (unsigned char)timer_1ms;
        wave_flag = 0;
    }
    startplay();
}

void startplay(void)
{
    note_play_flag      = 1;
    timer1H             = (unsigned char)(timer_50ms >> 8);
    timer1L             = (unsigned char)timer_50ms;
    note_duration_max   = speed * beat / 50;
    note_duration_count = 0;
    TR0                 = 1;
    TR1                 = 1;
}

void duration(void)
{
    note_duration_count++;
    if (note_duration_count >= note_duration_max) {
        note_duration_count = 0;
        TR0                 = 0;
        TR1                 = 0;
        note_play_flag      = 0;
    } else {
        TR1 = 1;
    }
}