#include <iostream>
#include <windows.h>
#include <conio.h>

MakeRedLine(int fromX, int fromY, int toX, int toY, int width=2) {
    HDC hdc = GetDC(GetDesktopWindow());
    HPEN pen = CreatePen(PS_SOLID, width, RGB(255, 0, 0));
    SelectObject(hdc, pen);
    MoveToEx(hdc, fromX, fromY, NULL);
    LineTo(hdc, toX, toY);
    ReleaseDC(NULL, hdc);
}
