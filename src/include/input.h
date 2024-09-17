#pragma once

#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <vector>
#include <ncurses.h>

#include "utils/string.h"

using namespace std;

namespace input {
    char process(WINDOW* window, string text, int pressed, int& x, int&y) {
        vector<string> lines = utils::splitString(text, "\n");

        switch (pressed) {
        // Cursor stuff
        case KEY_DOWN:
            if (y + 1 > lines.size()) break;
            y++;
            break;
        case KEY_UP:
            if (y - 1 < 0) break;
            y--;
            break;
        case KEY_LEFT:
            if (x - 1 < 0) {
                if (y - 1 < 0) break;
                x = lines.at(y - 2).size();
                y--;
                break;
            };
            x--;
            break;
        case KEY_RIGHT:
            if (x + 1 > lines.at(y - 1).size()) {
                x = 0;
                y++;
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