#include "input.h"

bool editor::input::kbhit() {
    int ch = getch();
    bool isPressed = ch != ERR;

    if (isPressed) {
        ungetch(ch);
    }

    return isPressed;
}

bool editor::input::moveUp() {
    if (y - 1 < 0) return false;
    if (page > 0) page--;
    y--;
    return true;
}

bool editor::input::moveDown() {
    if (y + 2 > _file->contents.size()) return false;
    if (y > getmaxy(_window) - 3) page++;
    y++;
    return true;
}

void editor::input::moveX() {
    if (x > _file->contents.at(y).size() || _file->contents.at(y).size() <= lastX) {
        x = _file->contents.at(y).size();
        return;
    }
    
    if (_file->contents.at(y).size() > lastX) {
        x = lastX;
    }
}

editor::input::input(file* file, WINDOW* window, int startX, int startY) {
    this->_window = window;
    this->_file = file;

    this->x = startX;
    this->y = startY;
}

void editor::input::process(bool isPressed, int pressed) {
    switch (pressed) {
    // Cursor stuff
    case KEY_DOWN:
        if (!moveDown()) break;
        moveX();
        break;
    case KEY_UP:
        if (!moveUp()) break;
        moveX();
        break;
    case KEY_LEFT:
        if (x - 1 >= 0) {
            x--;
            lastX = x;
            break;
        }

        if (!moveUp()) {
            break;
        }

        x = _file->contents.at(y).size();
        lastX = x;
        break;
    case KEY_RIGHT:
        if (x < _file->contents.at(y).size()) {
            x++;
            lastX = x;
            break;
        }

        if (!moveDown()) {
            break;
        }

        x = 0;
        lastX = x;
        break;

    // Special keys

    // Backspace
    case '\b':
    case 127:
    case KEY_BACKSPACE:
        if (x - 1 < 0) {
            if (y - 1 < 0) break;

            if (_file->contents.at(y - 1).size() > 0) {
                _file->contents.at(y) = _file->contents.at(y - 1) + _file->contents.at(y);
                x = _file->contents.at(y - 1).size();
            }

            _file->contents.erase(_file->contents.begin() + y - 1, _file->contents.begin() + y);
            moveUp();
            break;
        }

        _file->contents.at(y).erase(_file->contents.at(y).begin() + x - 1, _file->contents.at(y).begin() + x);
        x--;
        break;
    
    case '\n':
    case KEY_ENTER:
        if (x > 0) {
            string nl = _file->contents.at(y);
            nl.erase(nl.begin(), nl.begin() + x);
            _file->contents.insert(_file->contents.begin() + y + 1, nl);
            _file->contents.at(y).erase(_file->contents.at(y).begin() + x, _file->contents.at(y).end());
            x = 0;
        } else {
            _file->contents.insert(_file->contents.begin() + y, "");
        }

        moveDown();
        break;

    case KEY_COMMAND:
    case KEY_OPTIONS:
        break;

    // Normal
    default:
        if (!isPressed) break;

        _file->contents.at(y).insert(x, utils::toString((char)pressed));
        x++;
        break;
    }
}

int editor::input::getCursorX() {
    return x;
}

int editor::input::getActualCursorX() {
    return clamp(x + 1, 1, getmaxx(_window) - 1);
}

int editor::input::getCursorY() {
    return y;
}

int editor::input::getActualCursorY() {
    return clamp(y + 1, 1, getmaxy(_window) - 1);
}

int editor::input::getPage() {
    return page;
}
