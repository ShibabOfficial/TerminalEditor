#pragma once

#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <vector>
#include <ncurses.h>
#include <string>

#include "utils/string.h"
#include "fs/file.h"

using namespace std;

namespace editor {
    enum mode {
        EDITING = 0,
        READONLY = 1,
        COMMAND = 2,
    };

    class input {
    private:
        WINDOW* _window;

        mode editorMode = editor::EDITING;

        bool commandReturned = false;
        string commandInput = "";
        int commandX = 0;

        int x = 0;
        int relX = 0;
        int y = 0;
        int relY = 0;

        int lastX = 0;

        int pageY = 0;
        int pageX = 0;

        bool moveUp();
        bool moveDown(int linesCount);
        void moveX(int lineSize);
    public:
        input(WINDOW* window, int startX, int startY);

        void process(file* file, bool isPressed, int pressed);
        
        int getCursorX();
        int getRelativeCursorX();
        int getCursorY();
        int getRelativeCursorY();

        int getPageX();
        int getPageY();

        void resetCursor();

        void setMode(mode sMode);
        mode getMode();

        bool isCommandReturned();
        void clearCommand();
        vector<string> getCommand();
        int getCommandX();

        bool kbhit();
    };
}

#endif