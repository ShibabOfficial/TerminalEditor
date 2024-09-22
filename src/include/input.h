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
        int relX;
        int y;
        int relY;

        int lastX = 0;

        int pageY;
        int pageX;

        bool moveUp();
        bool moveDown();
        void moveX();
    public:
        input(file* file, WINDOW* window, int startX, int startY);

        void process(bool isPressed, int pressed);
        
        int getCursorX();
        int getRelativeCursorX();
        int getCursorY();
        int getRelativeCursorY();

        int getPageX();
        int getPageY();

        bool kbhit();
    };
}

#endif