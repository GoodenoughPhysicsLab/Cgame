#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <signal.h>

#define KB_ESC 27

int upper(const int num) {
    return num & 0b1011111;
}

void snack_exit(int sig) {
    if (sig == SIGINT)
        exit(0);
}

int main() 
{
    signal(SIGINT, snack_exit);

    const int mapWide = 20,
              size = mapWide * mapWide;
    
    int *map,
        position[2] = {0}, 
        length = 3,
        i, 
        behavior = 'D', // 'A', 'W', 'S', 'D'
        behavior_cache, // 更新动作前的缓冲，获取输入，A, W, S, D中的一种 蛇的方向
        *temp_ptr, // 临时变量
        food;

    map = (int*)calloc(size, 4);
    time_t t; srand((unsigned int)time(&t));

    /*
    * 'A' : 65 : 0b1'000'001
    * 'D' : 68 : 0b1'000'100
    * 'S' : 83 : 0b1'010'011
    * 'W' : 87 : 0b1'010'111
    */

    for (behavior_cache = map[1] = -1; behavior_cache != KB_ESC; Sleep(100))
    {
        if (_kbhit()) {
            behavior_cache = upper(_getch());
            if (behavior_cache == 'A' || behavior_cache == 'D' || behavior_cache == 'S' || behavior_cache == 'W') {
                // if press 'A', you can't press 'D'; if press 'W', you can't press 'S';
                if ((behavior_cache ^ behavior) & 0b10100 ^ 4) {
                    behavior = behavior_cache;
                }
            }
       }
        temp_ptr = position + !!(behavior & 2);
        *temp_ptr += behavior / 3 & 2;
        *temp_ptr = (--*temp_ptr + mapWide) % mapWide;

        food = !system("cls");
        if (*(temp_ptr = map + *position + position[1] * mapWide) > 0) {
            behavior_cache = KB_ESC;
        }

        for (; *temp_ptr && (map[i = rand() % size] || (--map[i], ++length, --food)););

        for (i = 0, *temp_ptr = length; i < size; ++i % mapWide || _cputs("|\n")) {
            _cputs(map[i] > 0 ? map[i] -= food, "()" : map[i] ? "00" : "  ");
        }
    }
}
/*
14行贪吃蛇, 668字符, 实现了简单的功能;
ADSW移动, 吃到食物成长, 可穿墙, 吃到自己身体或按下Esc键时游戏结束;
可自定义蛇头位置(position[0], position[1]), 蛇长(length), 初始方向(behave), 地图尺寸(mapWide);
在15行的版本上加以改进, 增加了Esc键退出, 更方便的自定义地图大小;
注意: 初次食物生成是固定位置, 默认情况下会在第一轮循环后重新生成
(除非你手速特别快, 或者修改了默认参数, 一般都不用在意这点细节)
于2020.7.4上传, 2020.10.1补充说明

下面是之前的说明:

修改了一点点代码, 比之前少了许多代码...
VS出现4996号错误加上此代码可解决
#pragma warning(disable:4996)
2020.9.30

使用gcc编译通过(MinGW)(gcc version 8.2.0)
在15行的基础上更改了一些细节，以及
修改地图大小更方便了，Esc键退出
*/
/*
去光标&闪烁方法(在Maze使用过的技巧)： 
1行: 把 stdlib.h 改成 windows.h 
9行: 把 system("cls") 去掉
4~5行之间插入：
SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &(CONSOLE_CURSOR_INFO) { 25, 0 });
10~11行之间插入：
SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD) { 0, 0 });
*/
