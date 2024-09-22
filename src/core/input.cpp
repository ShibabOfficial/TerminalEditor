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

    if (relY - 1 >= 0) relY--;
    else if (relY - 2 < 0 && pageY > 0) pageY--;

    y--;
    return true;
}

bool editor::input::moveDown() {
    if (y + 2 > _file->contents.size()) return false;

    if (relY + 1 < getmaxy(_window) - 1) relY++;
    else pageY++;

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
        if (editorMode == COMMAND) break;

        if (!moveDown()) break;

        moveX();
        break;
    case KEY_UP:
        if (editorMode == COMMAND) break;

        if (!moveUp()) break;

        moveX();
        break;
    case KEY_LEFT:
        if (editorMode == COMMAND) {
            commandX--;
            break;
        }

        if (x - 1 >= 0) {
            x--;
        } else {
            if (!moveUp()) break;
            x = _file->contents.at(y).size();
        }

        lastX = x;
        break;
    case KEY_RIGHT:
        if (editorMode == COMMAND) {
            commandX++;
            break;
        }

        if (x < _file->contents.at(y).size()) {
            x++;
        } else {
            if (!moveDown()) break;
            x = 0;
        }

        lastX = x;
        break;

    // Special keys

    // Backspace
    case '\b':
    case 127:
    case KEY_BACKSPACE:
        if (editorMode == READONLY) break;
        if (editorMode == COMMAND) {
            if (commandX - 1 < 0) break;

            commandInput.erase(commandInput.begin() + commandX - 1, commandInput.begin() + commandX);
            commandX--;
            break;
        }

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
        if (editorMode == READONLY) break;
        if (editorMode == COMMAND) {
            commandReturned = true;
            break;
        }

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
    
    // Insert
    case 330:
    case KEY_IC:
        if (editorMode == COMMAND) break;

        editorMode = COMMAND;
        break;

    case KEY_COMMAND:
    case KEY_OPTIONS:
        break;

    // Normal
    default:
        if (!isPressed) break;

        string ch = utils::toString((char)pressed);

        if (editorMode == COMMAND) {
            commandInput.insert(commandX, ch);
            commandX++;
            break;
        }

        _file->contents.at(y).insert(x, ch);
        x++;
        break;
    }
}

int editor::input::getCursorX() {
    return x;
}

int editor::input::getRelativeCursorX() {
    return relX;
}

int editor::input::getCursorY() {
    return y;
}

int editor::input::getRelativeCursorY() {
    return relY;
}

int editor::input::getPageX() {
    return pageX;
}

int editor::input::getPageY() {
    return pageY;
}

void editor::input::setMode(mode sMode) {
    editorMode = sMode;
}

editor::mode editor::input::getMode() {
    return editorMode;
}

bool editor::input::isCommandReturned() {
    return commandReturned;
}

void editor::input::clearCommand() {
    commandReturned = false;
    commandInput = "";
    commandX = 0;
}

vector<string> editor::input::getCommand() {
    return utils::splitString(commandInput, " ");
}

int editor::input::getCommandX() {
    return commandX;
}
