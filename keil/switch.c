#include "switch.h"

bit EC11_A_Now         = 1; // 编码器A脚稳定值
bit EC11_B_Now         = 1; // 编码器B脚稳定值
signed int volume    = 60;
int Song_choose        = 1;
signed int EC11_A_Code = 1;  // EC11编码器正转一格键值
signed int EC11_B_Code = -1; // EC11编码器反转一格键值
bit Play_flag          = 0;  // 播放标志
signed int Loop_mode = 0;  // 循环模式

xdata const char code_table[16] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0}; // EC11状态转移表

/// @brief EC11检测
void EC11_Scan()
{
    static unsigned char last_state = 0;
    unsigned char ab;
    char step;

    ab         = ((unsigned char)EC11_A << 1) | EC11_B;
    last_state = (unsigned char)(((unsigned int)last_state << 2) | ab) & 0x0F;

    step = code_table[last_state];
    if (step == 1) {
        EC11_Action(EC11_A_Code); // 顺时针
    } else if (step == -1) {
        EC11_Action(EC11_B_Code); // 逆时针
    }
}

void EC11_Action(signed char num)
{
    volume -= num; //  编码器正旋+1反旋减1
    if (volume > 100) volume = 100;
    if (volume < 0) volume = 0;
}

/// @brief 按键检测
/// @param key 按键地址
/// @param close_state 按键闭合状态
/// @return 如果按键被按下返回1，否则返回0

bit KEY_pressed(bit key, bit close_state)
{
    if (key == close_state) {
        delay_ms(5);
        if (key == close_state)
            return 1;
        else
            return 0;
    } else
        return 0;
}

bit Play_flag_fun(void)
{
    static bit last_key_state = 0;
    bit now                   = EC11_Key;
    bit ret                   = Play_flag;
    if (now == 1 && last_key_state == 0) { // 只在按下瞬间翻转
        Play_flag = !Play_flag;
        ret       = Play_flag;
        delay_ms(5); // 简单消抖
    }
    last_key_state = now;
    return ret;
}

/// @brief 循环模式
/// @param
/// @return 0: 播放单曲 1: 单曲循环 2: 列表播放
signed char Loop_mode_fun(void)
{
    if (KEY_pressed(LOOP_KEY, 0)) {
        Loop_mode++;
        if (Loop_mode > 2) Loop_mode = 0;
    }
    return Loop_mode;
}

/// @brief 按键检测并进行选歌
/// @param max 歌曲总数
/// @param next 手动切歌
/// @return 如果歌曲选择发生变化 返回1
bit Cut_Scan(unsigned int max)
{
    static unsigned int last_song = 1;

    if (KEY_pressed(NEXT_KEY, 0)) {
        Song_choose++;
    }
    if (KEY_pressed(PREV_KEY, 0)) {
        Song_choose--;
    }
    if (Song_choose > max) Song_choose = 1;
    if (Song_choose < 1) Song_choose = max;
    if (Song_choose != last_song) {
        last_song = Song_choose;
        return 1;
    }
    return 0;
}
