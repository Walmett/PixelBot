#pragma once

#include <iostream>
#include <windows.h>
#include <chrono>

const int KEYBOARD_SLEEP = 2;
const int MOUSE_SLEEP = 2;

void keyboard(const std::string &s) {
    for (char c: s) {
        keybd_event(c, 0, 0, 0);
        Sleep(KEYBOARD_SLEEP);
        keybd_event(c, 0, KEYEVENTF_KEYUP, 0);
    }
}

void HoldKeyboard(char c, int time) {
    if (time != -2) {
        keybd_event(c, 0, 0, 0);
        if (time == -1) return;
        Sleep(time);
    }
    keybd_event(c, 0, KEYEVENTF_KEYUP, 0);
}

void HoldKeyboard(const std::string &s, int time) {
    for (char c: s) {
        keybd_event(c, 0, 0, 0);
    }
    Sleep(time);
    for (char c: s) {
        keybd_event(c, 0, KEYEVENTF_KEYUP, 0);
    }
}

void PressKey(char c, bool hold = true) {
    if (hold) {
        keybd_event(c, 0, 0, 0);
    } else {
        keybd_event(c, 0, KEYEVENTF_KEYUP, 0);
    }
}

void click() {
    POINT p;
    GetCursorPos(&p);
    mouse_event(MOUSEEVENTF_LEFTDOWN, p.x, p.y, 0, 0);
    Sleep(MOUSE_SLEEP);
    mouse_event(MOUSEEVENTF_LEFTUP, p.x, p.y, 0, 0);
}

void HoldMouse(int time) {
    POINT p;
    GetCursorPos(&p);
    mouse_event(MOUSEEVENTF_LEFTDOWN, p.x, p.y, 0, 0);
    Sleep(time);
    mouse_event(MOUSEEVENTF_LEFTUP, p.x, p.y, 0, 0);
}

void PressMouse(bool hold = true) {
    POINT p;
    GetCursorPos(&p);
    if (hold) {
        mouse_event(MOUSEEVENTF_LEFTDOWN, p.x, p.y, 0, 0);
    } else {
        mouse_event(MOUSEEVENTF_LEFTUP, p.x, p.y, 0, 0);
    }
}

void click_right() {
    POINT p;
    GetCursorPos(&p);
    mouse_event(MOUSEEVENTF_RIGHTDOWN, p.x, p.y, 0, 0);
    Sleep(MOUSE_SLEEP);
    mouse_event(MOUSEEVENTF_RIGHTUP, p.x, p.y, 0, 0);
}

void HoldMouseRight(int time) {
    POINT p;
    GetCursorPos(&p);
    mouse_event(MOUSEEVENTF_RIGHTDOWN, p.x, p.y, 0, 0);
    Sleep(time);
    mouse_event(MOUSEEVENTF_RIGHTUP, p.x, p.y, 0, 0);
}

void PressMouseRight(bool hold = true) {
    POINT p;
    GetCursorPos(&p);
    if (hold) {
        mouse_event(MOUSEEVENTF_RIGHTDOWN, p.x, p.y, 0, 0);
    } else {
        mouse_event(MOUSEEVENTF_RIGHTUP, p.x, p.y, 0, 0);
    }
}

int CursorX() {
    POINT p;
    GetCursorPos(&p);
    return p.x;
}

int CursorY() {
    POINT p;
    GetCursorPos(&p);
    return p.y;
}

long long GetNanoseconds() {
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
    return nanoseconds.count();
}

long long GetTime() {
    return GetNanoseconds() / 1000000;
}

void MoveCursorSlowly(int x, int y) {
    const int DELTA = 30;
    POINT p;
    GetCursorPos(&p);
    while(std::max(p.x - x, x - p.x) > DELTA) {
        p.x += DELTA * (2 * (p.x < x) - 1);
        SetCursorPos(p.x, p.y);
        Sleep(1);
    }
    GetCursorPos(&p);
    while(std::max(p.y - y, y - p.y) > DELTA) {
        p.y += DELTA * (2 * (p.y < y) - 1);
        SetCursorPos(p.x, p.y);
        Sleep(1);
    }
    SetCursorPos(x, y);
}


