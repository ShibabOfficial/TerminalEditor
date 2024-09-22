#include <iostream>
#include <format>
#include <ncurses.h>

#include "file.h"
#include "input.h"
#include "utils/color.h"
#include "commands.h"

using namespace std;

int main(int argc, char* argv[]) {
    // If we didnt specify the file we quit
    if (argc <= 1) {
        cout << "No files specified. Quitting..." << endl;
        return 0;
    }

    // Reading the file
    file* f = new file(argv[1]);
    int errcode = f->read();

    // If something went wrong
    if (errcode) {
        cout << "Error " << errcode << " occured" << endl;
        return 0;
    }

    // Set the window name as the file name
    cout << "\033]0;" << f->fullname << "\007";

    // Init ncurses
    WINDOW* window = initscr();
    keypad(window, true); // Arrow keys
    nodelay(window, true);
    scrollok(window, false); // Disable cursor going of of the screen
    noecho();

    // Colors
    utils::colors::init();

    init_pair(1, utils::colors::WHITE, utils::colors::GRAY);
    init_pair(2, utils::colors::WHITE, utils::colors::DEEP_BLUE);
    init_pair(3, utils::colors::WHITE, utils::colors::DARK_GRAY);

    bool running = true;

    // Initializing input
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

            // Bar components
            string cursor = std::format(" {}:{} ", yCur + 1, xCur + 1);
            string file = std::format(" {} ", f->fullname);

            // No background
            attron(COLOR_PAIR(1));
            mvaddstr(0, 0, bar.c_str());
            mvaddstr(0, cursor.size(), file.c_str());
            attroff(COLOR_PAIR(1));

            // With background
            attron(COLOR_PAIR(2));
            mvaddstr(0, 0, cursor.c_str());
            attroff(COLOR_PAIR(2));
        }

        // Editor
        {
            // Max margin for x
            int maxMargin = utils::toString(f->contents.size()).size();

            int page = input.getPageY();

            // Relative cursor position
            int xC = 0;
            int yC = 0;

            editor::mode mode = input.getMode();

            for (int i = page; i < f->contents.size(); i++) {
                // Line number
                int j = i + 1;

                // Line number with margin
                string spaces = string(maxMargin + 1 - utils::toString(j).size(), ' ');
                string ln = std::format("{}{} ", spaces, j);

                // Text line
                string line = std::format(" {}", f->contents.at(i).c_str());

                // Printing line

                attron(COLOR_PAIR(3));
                mvaddstr(j - page, 0, ln.c_str());
                attroff(COLOR_PAIR(3));

                use_default_colors();
                mvaddstr(j - page, maxMargin + 2, line.c_str());
            }

            if (mode == editor::COMMAND) {
                // Propmt of COMMAND mode
                string prompt = std::format("> {}", string(getmaxx(window) - 2, ' '));
                // Current input
                string contents = utils::join(input.getCommand(), " ");

                // Printing
                attron(COLOR_PAIR(3));
                mvaddstr(getmaxy(window) - 1, 0, prompt.c_str());
                mvaddstr(getmaxy(window) - 1, 2, contents.c_str());
                attroff(COLOR_PAIR(3));

                // Cursor Y
                yC = getmaxy(window) - 1;

                // If returned then execure
                if (input.isCommandReturned()) {
                    editor::commandHandler::execute(input.getCommand(), &input, running);
                    input.clearCommand(); // also resets cursor X
                } else {
                    // Cursor X
                    xC = 2 + input.getCommandX();
                }

            }

            if (mode == editor::EDITING) {
                yC = input.getRelativeCursorY() + 1;
                xC = input.getRelativeCursorX() + maxMargin + 3;
            }

            use_default_colors();

            wmove(window, yC, xC);
        }

        refresh();
    }
    
    endwin();

    return 0;
}
