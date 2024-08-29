#pragma once

#include <windows.h>
#include <conio.h>

unsigned long long GetPixel_(int x, int y) {
    HDC dc = GetDC(NULL);
    COLORREF color = GetPixel(dc, x, y);
    unsigned long long clr = GetRValue(color) + GetGValue(color) * (1ull << 8ull) + GetBValue(color) * (1ull << 16ull);
    return clr;
}

int ScreenX = GetDeviceCaps(GetDC(0), HORZRES);
int ScreenY = GetDeviceCaps(GetDC(0), VERTRES);
BYTE* ScreenData = new BYTE[3 * ScreenX * ScreenY];

void ScreenCap() {
    HDC hdc = GetDC(GetDesktopWindow());
    HDC hdcMem = CreateCompatibleDC (hdc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, ScreenX, ScreenY);
    BITMAPINFOHEADER bmi = {0};
    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biPlanes = 1;
    bmi.biBitCount = 24;
    bmi.biWidth = ScreenX;
    bmi.biHeight = -ScreenY;
    bmi.biCompression = BI_RGB;
    bmi.biSizeImage = ScreenX * ScreenY;
    SelectObject(hdcMem, hBitmap);
    BitBlt(hdcMem, 0, 0, ScreenX, ScreenY, hdc, 0, 0, SRCCOPY);
    GetDIBits(hdc, hBitmap, 0, ScreenY, ScreenData, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdc);
}

inline int RColor(int x, int y) {
    return ScreenData[3 * ((y * ScreenX) + x) + 2];
}

inline int GColor(int x, int y) {
    return ScreenData[3 * ((y * ScreenX) + x) + 1];
}

inline int BColor(int x, int y) {
    return ScreenData[3 * ((y * ScreenX) + x)];
}

int GetPixelDIB(int x, int y, bool capture = true) {
    if (capture) {
        ScreenCap();
    }
    return RColor(x, y) + GColor(x, y) * (1 << 8) + BColor(x, y) * (1 << 16);
}

int GetCursorPixel(bool capture = true) {
    POINT p;
    GetCursorPos(&p);
    return GetPixelDIB(p.x, p.y, capture);
}


/*
void CaptureScreen(){
    // Определение контекстов
    HDC ScreenDC = GetDC(0);
    HDC MemoryDC = CreateCompatibleDC(ScreenDC);

    // Фиксация размеров экрана
    int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Создание и частичное заполнение структуры формата
    BITMAPINFO BMI;
    BMI.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    BMI.bmiHeader.biWidth = ScreenWidth;
    BMI.bmiHeader.biHeight = -ScreenHeight; // Отрицательное значение высоты, чтобы изображение не было перевёрнутым
    BMI.bmiHeader.biSizeImage = ScreenWidth * ScreenHeight * 3; // Ширина * Высота * Количество_цветов_на_пиксель
    BMI.bmiHeader.biCompression = BI_RGB;
    BMI.bmiHeader.biBitCount  = 24;
    BMI.bmiHeader.biPlanes = 1;
    int ScreenshotSize;
    ScreenshotSize = BMI.bmiHeader.biSizeImage; // ScreenshotSize - глобальная переменная типа int, может меняться в ходе выполнения программы
    unsigned char *ImageBuffer; // Указатель на блок данных BGR, управляемый HBITMAP (да, именно BGR - не RGB)
    HBITMAP hBitmap = CreateDIBSection(ScreenDC,&BMI,DIB_RGB_COLORS,(void**)&ImageBuffer,0,0);
    SelectObject(MemoryDC, hBitmap);
    BitBlt(MemoryDC, 0, 0, ScreenWidth, ScreenHeight, ScreenDC , 0, 0, SRCCOPY);

    // Контексты больше не нужны
    DeleteDC(MemoryDC);
    ReleaseDC(NULL, ScreenDC);

    // Если требуется RGB вместо BGR - следующий цикл перевернёт нужные байты
    for(int i = 0; i < ScreenshotSize; i += 3){
        unsigned char ColorValue = ImageBuffer[i];
        ImageBuffer[i] = ImageBuffer[i + 2];
        ImageBuffer[i + 2] = ColorValue;
    }

    // Используем ImageBuffer как нам хочется... после удаляем его хендлер (HBITMAP) для избежания утечки памяти
    DeleteObject(hBitmap);
}
*/

