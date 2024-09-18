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

    file f = file(argv[1]);

    vector<string> lines;
    int errcode = f.getContents(lines);

    if (errcode) {
        cout << "Error " << errcode << " occured" << endl;
        return 0;
    }

    WINDOW* window = initscr();
    keypad(window, true);
    nodelay(window, true);
    // noecho();

    // colors
    start_color();
    utils::colorRGB(10, 41, 41, 41); // Dark gray
    utils::colorRGB(11, 49, 53, 61); // Gray
    utils::colorRGB(12, 140, 140, 140); // Light gray
    utils::colorRGB(13, 217, 217, 217); // White
    utils::colorRGB(14, 41, 98, 255); // Deep Blue
    utils::colorRGB(15, 0, 146, 178); // Aqua
    utils::colorRGB(15, 146, 80, 188); // Light purple
    utils::colorRGB(15, 156, 39, 176); // Purple

    init_pair(1, 13, 11);
    init_pair(2, 13, 14);
    init_pair(3, 13, 10);

    bool running = true;

    int xCursor = 0;
    int yCursor = 0;

    while (running) {
        int pressed = wgetch(window);

        input::process(window, lines, pressed, xCursor, yCursor);

        // Rendering
        erase();

        // Top bar
        {
            string bar = std::format("{}", string(getmaxx(window), ' '));
            string cursor = std::format(" {}:{} ", yCursor + 1, xCursor + 1);
            string file = std::format(" {}.{} ", f.name, f.ext);

            attron(COLOR_PAIR(1));
            mvaddstr(0, 0, bar.c_str());
            mvaddstr(0, cursor.size(), file.c_str());
            attroff(COLOR_PAIR(1));

            attron(COLOR_PAIR(2));
            mvaddstr(0, 0, cursor.c_str());
            attroff(COLOR_PAIR(2));
        }

        // Lines
        int maxMargin = utils::toString(lines.size()).size();
        {
            for (int i = 0; i < lines.size(); i++) {
                attron(COLOR_PAIR(3));
                string num = utils::toString(i + 1);
                string ln = std::format("{}{} ", string(maxMargin + 1 - num.size(), ' '), num);

                mvaddstr(i + 1, 0, ln.c_str());
                attroff(COLOR_PAIR(3));

                use_default_colors();
                string line = std::format(" {}", lines.at(i).c_str());
                mvaddstr(i + 1, maxMargin + 2, line.c_str());
            }
        }

        // Cursor
        wmove(window, yCursor + 1, xCursor + maxMargin + 3);
        refresh();
    }
    
    endwin();

    return 0;
}
