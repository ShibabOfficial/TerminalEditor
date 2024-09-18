#pragma once

#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <vector>
#include <ncurses.h>

#include "utils/string.h"

using namespace std;

int lastX = 0;

bool moveUp(int& x, int& y) {
    if (y - 1 < 0) return false;
    y--;
    return true;
}

bool moveDown(vector<string> lines, int& x, int& y) {
    if (y + 1 > lines.size()) return false;
    y++;
    return true;
}

void moveX(vector<string> lines, int& x, int& y) {
    if (x > lines.at(y).size() || lines.at(y).size() < lastX) {
        x = lines.at(y).size();
        return;
    }
    
    if (lines.at(y).size() > lastX) {
        x = lastX;
    }
}

namespace input {
    char process(WINDOW* window, vector<string> lines, int pressed, int& x, int&y) {
        switch (pressed) {
        // Cursor stuff
        case KEY_DOWN:
            moveDown(lines, x, y);
            moveX(lines, x, y);
            break;
        case KEY_UP:
            moveUp(x, y);
            moveX(lines, x, y);
            break;
        case KEY_LEFT:
            if (x - 1 >= 0) {
                x--;
                lastX = x;
                break;
            }

            if (!moveUp(x, y)) {
                break;
            }
            x = lines.at(y).size();
            lastX = x;
            break;
        case KEY_RIGHT:
            if (x < lines.at(y).size()) {
                x++;
                lastX = x;
                break;
            }

            if (!moveDown(lines, x, y)) {
                break;
            }
            x = 0;
            lastX = x;
            break;

        // Special keys

        // Normal
        default:
            return pressed;
        }

        return '\0';
    }
}

#endif