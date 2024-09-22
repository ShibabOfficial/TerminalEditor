#include <iostream>
#include <format>
#include <ncurses.h>

#include "file.h"
#include "input.h"
#include "utils/color.h"
#include "commands.h"

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

    editor::input input = editor::input(f, window, 0, 0);

    while (running) {
        bool isPressed = input.kbhit();
        int pressed = wgetch(window);

        // Process the input
        input.process(isPressed, pressed);

        // Cursor position
        int xCur = input.getCursorX();
        int yCur = input.getCursorY();

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

        // Editor
        {
            int maxMargin = utils::toString(f->contents.size()).size();
            int afterMargin = maxMargin + 2;

            int page = input.getPageY();

            int xC = 0;
            int yC = 0;

            editor::mode mode = input.getMode();

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

            if (mode == editor::COMMAND) {
                attron(COLOR_PAIR(3));
                mvaddstr(getmaxy(window) - 1, 0, std::format("> {}", string(getmaxx(window) - 2, ' ')).c_str());
                mvaddstr(getmaxy(window) - 1, 2, utils::join(input.getCommand(), " ").c_str());
                attroff(COLOR_PAIR(3));

                xC = 2 + input.getCommandX();
                yC = getmaxy(window) - 1;

                if (input.isCommandReturned()) {
                    editor::commandHandler::execute(input.getCommand(), &input, running);
                    input.clearCommand();
                }
            }

            if (mode == editor::EDITING) {
                yC = input.getRelativeCursorY() + 1;
                xC = input.getRelativeCursorX() + afterMargin + 1;
            }

            use_default_colors();

            wmove(window, yC, xC);
        }

        refresh();
    }
    
    endwin();

    return 0;
}
