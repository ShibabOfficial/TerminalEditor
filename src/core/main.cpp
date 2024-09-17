#include <iostream>
#include <format>
#include <ncurses.h>

#include "file.h"
#include "input.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        cout << "No files specified. Quitting..." << endl;
        return 0;
    }

    file f = file(argv[1]);

    string contents;
    int errcode = f.getContents(contents);

    if (errcode) {
        cout << "Error " << errcode << " occured" << endl;
        return 0;
    }

    WINDOW* window = initscr();
    keypad(window, true);
    nodelay(window, true);
    // noecho();

    bool running = true;

    int xCursor = 0;
    int yCursor = 1;

    while (running) {
        int pressed = wgetch(window);

        input::process(window, contents, pressed, xCursor, yCursor);

        // Rendering
        erase();
        mvaddstr(0, 0, std::format("{}:{}", yCursor, xCursor + 1).c_str());
        mvaddstr(1, 0, contents.c_str());

        // Cursor
        wmove(window, yCursor, xCursor);
        refresh();
    }
    
    endwin();
}
