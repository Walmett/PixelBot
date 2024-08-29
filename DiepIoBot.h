#pragma once

#include "interact.h"
#include "PixelCapture.h"
#include "Draw.h"

#include <vector>
#include <set>
#include <math.h>

// ! DIEP IO CHEAT ZOOM - 0.25 / 0.3
//CURRENT CONSTANTS FOR 800x600

const int RED_TEAM_COLOR = 5525233;
const int BLUE_TEAM_COLOR = 14791168;
const int GREEN_TEAM_COLOR = 0;
const int PURPLE_TEAM_COLOR = 0;
const int GUN_MUZZLE_COLOR = 0;
const int YELLOW_SQUARE_COLOR = 6940927;
const int RED_TRIANGLE_COLOR = 7829244;
const int PURPLE_PENTAGON_COLOR = 16551286;
const int PURPLE_BOT_COLOR = 0;
const int MAP_TANK_COLOR = 0;

const int GRAY_CELL_COLOR = 12105912;

const int FIRST_CORNER_X = 1;
const int FIRST_CORNER_Y = 1;
const int SECOND_CORNER_X = ScreenX - 1;
const int SECOND_CORNER_Y = ScreenY - 1;

const int PIXEL_DISTANCE = 1;
const int SCREEN_SHOT_SLEEP = 25;

const int MID_COORDINATE_X = ScreenX / 2;
const int MID_COORDINATE_Y = ScreenY / 2;

const int MIN_TANK_SIZE = 15;
int BULLET_SPEED_COEFFICIENT = 100;

class DiepIo {

public:
    enum GetPixelMode {
        CAPTURE,
        NO_CAPTURE,
        FIRST,
        SECOND
    };

    std::vector<std::vector<int> > FirstScreenShot;
    std::vector<std::vector<int> > SecondScreenShot;

    DiepIo() : FirstScreenShot(ScreenX + 1, std::vector<int> (ScreenY + 1)), SecondScreenShot(ScreenX + 1, std::vector<int> (ScreenY + 1)) {

    }

    void test() {
        while (true) {
            makeScreenShots();
            auto circles = getAllCircleMids(BLUE_TEAM_COLOR, FIRST);
            for (auto mid: circles) {
                auto[tx, ty] = getClosest(BLUE_TEAM_COLOR, mid.first, mid.second, SECOND);
                if (tx == -1) {
                    auto[x, y] = getCircleMid(tx, ty, SECOND);
                    int dx = x - mid.first;
                    int dy = y - mid.second;
                    MakeRedLine(mid.first, mid.second, x + dx * 100, y + dy * 100);
                }
            }
        }
    }

    void simpleFarm(int EnemyColor, std::string kb = "") {
        Sleep(2000);
        while (!kbhit()) {
            int grey_cell_x = 0;
            int delta = 0;
            if (EnemyColor == RED_TEAM_COLOR) {
                grey_cell_x = 520;
                delta = -25;
            } else {
                grey_cell_x = 320;
                delta = 25;
            }

            Sleep(1000);

            keyboard(kb + kb + kb);
            //keyboard("56785678");
            //keyboard("45684568");
            //keyboard("568568");
            //keyboard("567567");
            //keyboard("88");

            HoldKeyboard('W', -1);
            // 320/520
            while (getPixel(grey_cell_x * ScreenX / 800, 200 * ScreenY / 600, CAPTURE) != GRAY_CELL_COLOR) {
                if (setCursorToClosest(PURPLE_PENTAGON_COLOR, MID_COORDINATE_X, MID_COORDINATE_Y, NO_CAPTURE) ||
                    setCursorToClosest(RED_TRIANGLE_COLOR, MID_COORDINATE_X, MID_COORDINATE_Y, NO_CAPTURE) ||
                    setCursorToClosest(YELLOW_SQUARE_COLOR, MID_COORDINATE_X, MID_COORDINATE_Y, NO_CAPTURE)) {
                    SetCursorPos(CursorX(), CursorY() + 20);
                } else {
                    MoveCursorSlowly(MID_COORDINATE_X + delta, MID_COORDINATE_Y + 100);
                }
                Sleep(10);
            }
            Sleep(100);
            HoldKeyboard('W', -2);
            HoldKeyboard('S', -1);
            while (getPixel(grey_cell_x * ScreenX / 800, 500 * ScreenY / 600, CAPTURE) != GRAY_CELL_COLOR) {
                if (setCursorToClosest(PURPLE_PENTAGON_COLOR, MID_COORDINATE_X, MID_COORDINATE_Y, NO_CAPTURE) ||
                    setCursorToClosest(RED_TRIANGLE_COLOR, MID_COORDINATE_X, MID_COORDINATE_Y, NO_CAPTURE) ||
                    setCursorToClosest(YELLOW_SQUARE_COLOR, MID_COORDINATE_X, MID_COORDINATE_Y, NO_CAPTURE)) {
                    SetCursorPos(CursorX(), CursorY() - 20);
                } else {
                    MoveCursorSlowly(MID_COORDINATE_X + delta, MID_COORDINATE_Y - 100);
                }
                Sleep(10);
            }
            HoldKeyboard('S', -2);
        }
    }

    void simpleShooting() {
        Sleep(2000);
        while(true) {
            //~15 for ranger, ~10 for destroyer
            std::cin >> BULLET_SPEED_COEFFICIENT;
            while (!kbhit()) {
                setToTarget(RED_TEAM_COLOR);
                //setCursorToClosest(PURPLE_PENTAGON_COLOR, MID_COORDINATE_X, MID_COORDINATE_Y, NO_CAPTURE) ||
                //setCursorToClosest(RED_TRIANGLE_COLOR, MID_COORDINATE_X, MID_COORDINATE_Y, NO_CAPTURE) ||
                //setCursorToClosest(YELLOW_SQUARE_COLOR, MID_COORDINATE_X, MID_COORDINATE_Y, NO_CAPTURE);
            }
        }
    }

    void OverLordBot(int EnemyColor) {
        Sleep(3000);
        while(true) {
            std::cin >> BULLET_SPEED_COEFFICIENT;
            while (!kbhit()) {
                if (!setToTarget(EnemyColor)) {
                    SetCursorPos(MID_COORDINATE_X, MID_COORDINATE_Y);
                }
            }
        }
    }

    bool setCursorToClosest(int TargetColor, int startX, int startY, GetPixelMode mode = CAPTURE) {
        getPixel(0, 0, CAPTURE);
        auto[closestX, closestY] = getClosest(TargetColor, startX, startY, NO_CAPTURE);
        if (closestX == -1) {
            return false;
        }
        MoveCursorSlowly(closestX, closestY);
        return true;
    }

    void botCursorToClosest(int TargetColor) {
        Sleep(2000);
        while (!kbhit()) {
            setCursorToClosest(TargetColor, MID_COORDINATE_X, MID_COORDINATE_Y);
            Sleep(5);
        }
    }

private:
    void makeScreenShot() {
        GetPixelDIB(0, 0, true);
        for (int x = FIRST_CORNER_X; x <= SECOND_CORNER_X; ++x) {
            for (int y = FIRST_CORNER_Y; y <= SECOND_CORNER_Y; ++y) {
                FirstScreenShot[x][y] = GetPixelDIB(x, y, false);
            }
        }
    }

    long long makeScreenShots() {
        long long time = GetTime();
        makeScreenShot();
        Sleep(SCREEN_SHOT_SLEEP);
        std::swap(FirstScreenShot, SecondScreenShot);
        time = GetTime() - time;
        makeScreenShot();
        std::swap(FirstScreenShot, SecondScreenShot);
        return time;
    }

    std::pair<int, int> getCircleMid(int x, int y, GetPixelMode mode = NO_CAPTURE) {
        int color = getPixel(x, y, mode);
        int leftX = x, rightX = x;
        int leftY = y, rightY = y;
        while (leftX > FIRST_CORNER_X && getPixel(--leftX, y, mode) == color);
        while (rightX < SECOND_CORNER_X && getPixel(++rightX, y, mode) == color);
        while (leftY > FIRST_CORNER_Y && getPixel(x, --leftY, mode) == color);
        while (rightY < SECOND_CORNER_Y && getPixel(x, ++rightY, mode) == color);
        return {(leftX + rightX) / 2 + (leftX + rightX) % 2, (leftY + rightY) / 2 + (leftY + rightY) % 2};
    }

    int getCircleDiameter(int midX, int midY, GetPixelMode mode = NO_CAPTURE) {
        int color = getPixel(midX, midY, mode);
        int diameter = 0;
        while (midX < SECOND_CORNER_X && getPixel(++midX, midY, mode) == color);
        while (midX > FIRST_CORNER_X && getPixel(--midX, midY, mode) == color) {
            ++diameter;
        }
        return diameter;
    }

    std::vector<std::pair<int, int>> getAllCircleMids(int TargetColor, GetPixelMode mode = NO_CAPTURE) {
        std::vector<std::vector<int>> used(ScreenX + 1, std::vector<int> (ScreenY + 1));
        for (int x = FIRST_CORNER_X; x <= SECOND_CORNER_X; ++x) {
            for (int y = FIRST_CORNER_Y; y <= SECOND_CORNER_Y; ++y) {
                if (getPixel(x, y, mode) == TargetColor) {
                    auto[midX, midY] = getCircleMid(x, y, mode);
                    for (int i = midX - 1; i <= midX + 1; ++i) {
                        for (int j = midY - 1; j <= midY + 1; ++j) {
                            if (used[i][j] == 1) {
                                midX = 0;
                                midY = 0;
                            }
                        }
                    }
                    used[midX][midY] = 1;
                }
            }
        }
        std::vector<std::pair<int, int>> result;
        for (int x = FIRST_CORNER_X; x <= SECOND_CORNER_X; ++x) {
            for (int y = FIRST_CORNER_Y; y <= SECOND_CORNER_Y; ++y) {
                if (used[x][y] == 1) {
                    result.push_back({x, y});
                }
            }
        }
        return result;
    }

    std::vector<std::pair<int, int>> getAllTanks(int TargetColor, GetPixelMode mode = NO_CAPTURE) {
        std::vector<std::pair<int, int>> result;
        auto circles = getAllCircleMids(TargetColor, mode);
        for (auto mid: circles) {
            int diameter = getCircleDiameter(mid.first, mid.second, mode);
            if (diameter >= MIN_TANK_SIZE) {
                result.emplace_back(mid);
            }
        }
        return result;
    }

    long long getL2DistanceSqr(int ax, int ay, int bx, int by) {
        return 1ll * (ax - bx) * (ax - bx) + 1ll * (ay - by) * (ay - by);
    }

    double getL2distance(double ax, double ay, double bx, double by) {
        return sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
    }

    std::pair<int, int> getClosest(int TargetColor, int startX, int startY, GetPixelMode mode = NO_CAPTURE) {
        int closestX = -2000;
        int closestY = -2000;
        for (int x = FIRST_CORNER_X; x <= SECOND_CORNER_X; ++x) {
            for (int y = FIRST_CORNER_Y; y <= SECOND_CORNER_Y; ++y) {
                if (getPixel(x, y, mode) == TargetColor &&
                    getL2DistanceSqr(x, y, startX, startY) < getL2DistanceSqr(closestX, closestY, startX, startY)) {
                    closestX = x;
                    closestY = y;
                }
            }
        }
        if (closestX == -2000) {
            return {-1, -1};
        }
        return {closestX, closestY};
    }

    bool setToTarget(int EnemyColor) {
        makeScreenShots();

        auto tanksFirst = getAllTanks(EnemyColor, FIRST);
        auto tanksSecond = getAllTanks(EnemyColor, SECOND);
        if (tanksFirst.empty() || tanksSecond.empty()) {
            return false;
        }
        int targetX = -2000;
        int targetY = -2000;
        for (auto tank: tanksFirst) {
            if (getL2DistanceSqr(MID_COORDINATE_X, MID_COORDINATE_Y, tank.first, tank.second) < getL2DistanceSqr(MID_COORDINATE_X, MID_COORDINATE_Y, targetX, targetY)) {
                targetX = tank.first;
                targetY = tank.second;
            }
        }
        int x = -2000;
        int y = -2000;
        for (auto tank: tanksSecond) {
            if (getL2DistanceSqr(targetX, targetY, tank.first, tank.second) < getL2DistanceSqr(targetX, targetY, x, y)) {
                x = tank.first;
                y = tank.second;
            }
        }

        int dx = x - targetX;
        int dy = y - targetY;

        int coefficient = getL2distance(MID_COORDINATE_X, MID_COORDINATE_Y, x, y) / BULLET_SPEED_COEFFICIENT;
        MoveCursorSlowly(x + coefficient * dx, y +  coefficient * dy);
        return true;
    }

    int getPixel(int x, int y, GetPixelMode mode) {
        if (mode == NO_CAPTURE) {
            return GetPixelDIB(x, y, false);
        }
        if (mode == CAPTURE) {
            return GetPixelDIB(x, y, true);
        }
        if (mode == FIRST) {
            return FirstScreenShot[x][y];
        }
        if (mode == SECOND) {
            return SecondScreenShot[x][y];
        }
        return 0;
    }
};
