#pragma once

#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <vector>
#include <ncurses.h>

#include "utils/string.h"

using namespace std;

bool moveDown(vector<string> lines, int& x, int& y) {
    if (y + 1 > lines.size()) return true;
    y++;
    return false;
}

bool moveUp(vector<string> lines, int& x, int& y) {
    if (y - 1 <= 0) return true;
    y--;
    return false;
}

void moveX(vector<string> lines, int& x, int& y) {
    if (x > lines.at(y - 1).size()) {
        x = lines.at(y - 1).size();
    }
}

namespace input {
    char process(WINDOW* window, string text, int pressed, int& x, int&y) {
        vector<string> lines = utils::splitString(text, "\n");

        switch (pressed) {
        // Cursor stuff
        case KEY_DOWN:
            if (moveDown(lines, x, y)) break;
            moveX(lines, x, y);
            break;
        case KEY_UP:
            if (moveUp(lines, x, y)) break;
            moveX(lines, x, y);
            break;
        case KEY_LEFT:
            if (x - 1 < 0) {
                if (moveUp(lines, x, y)) break;
                x = lines.at(y - 1).size();
                break;
            };
            x--;
            break;
        case KEY_RIGHT:
            if (x + 1 > lines.at(y - 1).size()) {
                if (moveDown(lines, x, y)) break;
                x = 0;
                break;
            }
            x++;
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