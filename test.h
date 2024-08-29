#pragma once

#include <iostream>
#include <windows.h>
#include "interact.h"
#include "PixelCapture.h"

void printCursorInfo() {
    int SleepTime;
    std::cout << "Set sleep:";
    std::cin >> SleepTime;
    char c = '1';
    while (c != '0') {
        if (kbhit()) {
            c = getch();
            int x = CursorX();
            int y = CursorY();
            Sleep(SleepTime);
            std::cout << "Coordinate: " << x << ", " << y << " Pixel: " << GetPixelDIB(x, y, true) << "\n";
        }
    }
}

void testFPS() {
    long long tickCount = GetTickCount();
    int cnt = 0;
    while (!kbhit()) {
        if (GetTickCount() - tickCount > 1000) {
            std::cout << cnt << std::endl;
            cnt = 0;
            tickCount = GetTickCount();
        }
        Sleep(15);
        ScreenCap();
        ++cnt;
    }
}
