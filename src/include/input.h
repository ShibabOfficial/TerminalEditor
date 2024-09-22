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
    enum mode {
        EDITING = 0,
        READONLY = 1,
        COMMAND = 2,
    };

    class input {
    private:
        WINDOW* _window;

        file* _file;

        mode editorMode = editor::COMMAND;

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