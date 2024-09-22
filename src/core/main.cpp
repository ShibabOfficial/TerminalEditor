#include <iostream>
#include <format>
#include <ncurses.h>
#include <ctime>
#include <string>

#include "fs/file.h"
#include "fs/fileHandler.h"
#include "utils/color.h"
#include "input.h"
#include "commands.h"
#include "os.h"

using namespace std;

int main(int argc, char* argv[]) {
    const char* path = "";

    if (argc > 1) {
        path = argv[1];
    }
    
    // Creating a file handle
    fileHandler::setFile(new file(path));

    // Init ncurses
    WINDOW* window = initscr();
    keypad(window, true); // Arrow keys
    nodelay(window, true);
    scrollok(window, false); // Disable cursor going of of the screen
    noecho();

    // Colors
    utils::colors::init();

    init_pair(1, utils::colors::WHITE, utils::colors::GRAY); // Top bar
    init_pair(2, utils::colors::WHITE, utils::colors::DEEP_BLUE); // Cursor pos
    init_pair(3, utils::colors::WHITE, utils::colors::DARK_GRAY); // Line
    init_pair(4, utils::colors::WHITE, utils::colors::MID_GRAY); // Line Highlight

    bool running = true;

    // time_t start = time(nullptr);

    // Initializing input
    editor::input input = editor::input(window, 0, 0);

    if (fileHandler::getFile()->contents.size() <= 0) {
        input.setMode(editor::COMMAND);
    }

    while (running) {
        file* file = fileHandler::getFile();
        time_t now = time(nullptr);
        
        bool isPressed = input.kbhit();
        int pressed = wgetch(window);

        // Process the input
        input.process(file, isPressed, pressed);

        // Cursor position
        int xCur = input.getCursorX();
        int yCur = input.getCursorY();

        // Rendering
        erase();

        // Top bar
        {
            string bar = std::format("{}", string(getmaxx(window), ' '));

            // Bar components
            string cursor = std::format(" Ln:{},Col:{} ", yCur + 1, xCur + 1);
            string fileName = std::format(" {} ", file->fullname);
            
            // OS
            string os = std::format(" {} ", os::getOS());

            // Time
            char buffer[9];
            strftime(buffer, 9, "%H:%M:%S", localtime(&now));
            string fTime = std::format(" {}", buffer);

            // Time spent

            // No background
            attron(COLOR_PAIR(1));
            mvaddstr(0, 0, bar.c_str());
            mvaddstr(0, 0, fileName.c_str());
            mvaddstr(0, getmaxx(window) - cursor.size() - os.size(), os.c_str());
            mvaddstr(0, getmaxx(window) - cursor.size() - fTime.size() - os.size(), fTime.c_str());
            attroff(COLOR_PAIR(1));

            // With background
            attron(COLOR_PAIR(2));
            mvaddstr(0, getmaxx(window) - cursor.size(), cursor.c_str());
            attroff(COLOR_PAIR(2));
        }

        // Editor
        {
            // Max margin for x
            int maxMargin = utils::toString(file->contents.size()).size();

            int page = input.getPageY();

            // Relative cursor position
            int xC = 0;
            int yC = 0;

            editor::mode mode = input.getMode();

            for (int i = page; i < file->contents.size(); i++) {
                // Line number
                int j = i + 1;

                // Line number with margin
                string spaces = string(maxMargin + 1 - utils::toString(j).size(), ' ');
                string ln = std::format("{}{} ", spaces, j);

                // Text line
                string line = std::format(" {}", file->contents.at(i).c_str());

                // Printing line

                if (i == yCur) {
                    attron(COLOR_PAIR(4));
                    mvaddstr(j - page, 0, ln.c_str());
                    attroff(COLOR_PAIR(4));
                } else {
                    attron(COLOR_PAIR(3));
                    mvaddstr(j - page, 0, ln.c_str());
                    attroff(COLOR_PAIR(3));
                }

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
