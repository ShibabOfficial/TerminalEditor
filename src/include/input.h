#pragma once

#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <vector>
#include <ncurses.h>
#include <string>

#include "utils/string.h"
#include "file.h"

using namespace std;

namespace editor {
    class input {
    private:
        WINDOW* _window;

        file* _file;

        int x;
        int y;
        int lastX = 0;
        int page = 0;

        bool moveUp();
        bool moveDown();
        void moveX();
    public:
        input(file* file, WINDOW* window, int startX, int startY);

        void process(bool isPressed, int pressed);
        
        int getCursorX();
        int getActualCursorX();
        int getCursorY();
        int getActualCursorY();

        int getPage();

        bool kbhit();
    };
}

#endif