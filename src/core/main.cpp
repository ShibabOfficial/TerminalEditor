#include <iostream>
#include <format>
#include <ncurses.h>

#include "file.h"
#include "input.h"
#include "utils/color.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        cout << "No files specified. Quitting..." << endl;
        return 0;
    }

    file* f = new file(argv[1]);
    int errcode = f->read();

    if (errcode) {
        cout << "Error " << errcode << " occured" << endl;
        return 0;
    }

    cout << "\033]0;" << f->fullname << "\007";

    WINDOW* window = initscr();
    keypad(window, true);
    nodelay(window, true);
    scrollok(window, false);
    noecho();

    // Colors
    utils::colors::init();

    init_pair(1, utils::colors::WHITE, utils::colors::GRAY);
    init_pair(2, utils::colors::WHITE, utils::colors::DEEP_BLUE);
    init_pair(3, utils::colors::WHITE, utils::colors::DARK_GRAY);

    bool running = true;

    int page = 0;

    editor::input input = editor::input(f, window, 0, 0);

    while (running) {
        bool isPressed = input.kbhit();
        int pressed = wgetch(window);

        // Process the input
        input.process(isPressed, pressed);

        // Cursor position and "actual" cursor position
        int xCur = input.getCursorX();
        int xActCur = input.getActualCursorX();

        int yCur = input.getCursorY();
        int yActCur = input.getActualCursorY();

        // Rendering
        erase();

        // Top bar
        {
            string bar = std::format("{}", string(getmaxx(window), ' '));
            string cursor = std::format(" {}:{} ", yCur + 1, xCur + 1);
            string file = std::format(" {} ", f->fullname);

            attron(COLOR_PAIR(1));
            mvaddstr(0, 0, bar.c_str());
            mvaddstr(0, cursor.size(), file.c_str());
            attroff(COLOR_PAIR(1));

            attron(COLOR_PAIR(2));
            mvaddstr(0, 0, cursor.c_str());
            attroff(COLOR_PAIR(2));
        }

        // Text editing
        {
            int maxMargin = utils::toString(f->contents.size()).size();
            int afterMargin = maxMargin + 2;

            int page = input.getPage();

            for (int i = page; i < f->contents.size(); i++) {
                int j = i + 1;

                attron(COLOR_PAIR(3));
                string spaces = string(maxMargin + 1 - utils::toString(j).size(), ' ');
                string ln = std::format("{}{} ", spaces, j);

                mvaddstr(j - page, 0, ln.c_str());
                attroff(COLOR_PAIR(3));

                use_default_colors();
                string line = std::format(" {}", f->contents.at(i).c_str());
                mvaddstr(j - page, afterMargin, line.c_str());
            }

            wmove(window, yActCur, xActCur + afterMargin);
        }

        refresh();
    }
    
    endwin();

    return 0;
}
