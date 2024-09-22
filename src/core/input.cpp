#include "input.h"

bool editor::input::kbhit() {
    int ch = getch();
    // If the char doesnt equal to ERR it means its pressed
    bool isPressed = ch != ERR;

    // If pressed we restore the previous state
    if (isPressed) {
        ungetch(ch);
    }

    return isPressed;
}

bool editor::input::moveUp() {
    // Min height
    if (y - 1 < 0) return false;

    // Min rel height
    if (relY - 1 >= 0) relY--;
    else if (relY - 2 < 0 && pageY > 0) pageY--;

    y--;
    return true;
}

bool editor::input::moveDown() {
    // Max height
    if (y + 2 > _file->contents.size()) return false;

    // Max relative height
    if (relY + 1 < getmaxy(_window) - 1) relY++;
    else pageY++;

    y++;
    return true;
}

void editor::input::moveX() {
    // If the line is smaller then current x or smaller or equal we move to the end of the line
    if (x > _file->contents.at(y).size() || _file->contents.at(y).size() <= lastX) {
        x = _file->contents.at(y).size();
        return;
    }
    
    // If the line is larger then the last x we move to the lastX
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

        // If we reached the end of the line we move up or if we didnt we just move back
        if (x - 1 >= 0) {
            x--;
        } else {
            if (!moveUp()) break; // Moving up
            x = _file->contents.at(y).size(); // Last x
        }

        lastX = x;
        break;
    case KEY_RIGHT:
        if (editorMode == COMMAND) {
            commandX++;
            break;
        }

        // If we reached the end of the line we go down or if we didnt go right
        if (x < _file->contents.at(y).size()) {
            x++;
        } else {
            if (!moveDown()) break;
            x = 0; // Next line x
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

            // Removing the char before the cursor
            commandInput.erase(commandInput.begin() + commandX - 1, commandInput.begin() + commandX);
            // Move back
            commandX--;
            break;
        }

        // If its the last char and we can go up we move the line up
        if (x - 1 < 0) {
            if (y - 1 < 0) break;

            // If there is something
            if (_file->contents.at(y - 1).size() > 0) {
                _file->contents.at(y) = _file->contents.at(y - 1) + _file->contents.at(y);
                x = _file->contents.at(y - 1).size();
            }

            // Remove the line
            _file->contents.erase(_file->contents.begin() + y - 1, _file->contents.begin() + y);
            moveUp();
            break;
        }

        // Remiove the char before the cursor
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

        // If we are pressing enter in the middle of the line we split it to the new line
        if (x > 0) {
            // Split contents
            string nl = _file->contents.at(y);
            nl.erase(nl.begin(), nl.begin() + x);
            // Inserting it to the new line
            _file->contents.insert(_file->contents.begin() + y + 1, nl);
            // Removing the old contents from the line above
            _file->contents.at(y).erase(_file->contents.at(y).begin() + x, _file->contents.at(y).end());
            x = 0;
        } else {
            // New line
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

        string ch = utils::toString((char)pressed); // Convert to string so we can insert it

        // If we are in COMMAND mode we the char to the input
        if (editorMode == COMMAND) {
            commandInput.insert(commandX, ch);
            commandX++;
            break;
        }

        // Add to current line and col in file
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
    // Args for command and the command
    return utils::splitString(commandInput, " ");
}

int editor::input::getCommandX() {
    return commandX;
}
