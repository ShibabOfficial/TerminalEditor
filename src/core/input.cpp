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

bool editor::input::moveDown(int linesCount) {
    // Max height
    if (y + 2 > linesCount) return false;

    // Max relative height
    if (relY + 1 < getmaxy(_window) - 1) relY++;
    else pageY++;

    y++;
    return true;
}

void editor::input::moveX(int lineSize) {
    // If the line is smaller then current x or smaller or equal we move to the end of the line
    if (x > lineSize || lineSize <= lastX) {
        x = lineSize;
        relX = lineSize;
        return;
    }
    
    // If the line is larger then the last x we move to the lastX
    if (lineSize > lastX) {
        x = lastX;
        relX = lastX;
    }
}

editor::input::input(WINDOW* window, int startX, int startY) {
    this->_window = window;

    this->x = startX;
    this->y = startY;
}

void editor::input::process(file* file, bool isPressed, int pressed) {
    switch (pressed) {
    // Cursor stuff
    case KEY_DOWN:
        if (editorMode == COMMAND) break;

        if (!moveDown(file->contents.size())) break;

        moveX(file->contents.at(y).size());
        break;
    case KEY_UP:
        if (editorMode == COMMAND) break;

        if (!moveUp()) break;

        moveX(file->contents.at(y).size());
        break;
    case KEY_LEFT:
        if (editorMode == COMMAND) {
            if (commandX - 1 < 0) break;
            
            commandX--;
            break;
        }

        // If we reached the end of the line we move up or if we didnt we just move back
        if (x - 1 >= 0) {
            x--;
        } else {
            if (!moveUp()) break; // Moving up
            x = file->contents.at(y).size(); // Last x
        }

        lastX = x;
        relX = x;
        break;
    case KEY_RIGHT:
        if (editorMode == COMMAND) {
            if (commandX + 1 > commandInput.size()) break;

            commandX++;
            break;
        }

        // If we reached the end of the line we go down or if we didnt go right
        if (x < file->contents.at(y).size()) {
            x++;
        } else {
            if (!moveDown(file->contents.size())) break;
            x = 0; // Next line x
        }

        lastX = x;
        relX = x;
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
            if (file->contents.at(y - 1).size() > 0) {
                file->contents.at(y) = file->contents.at(y - 1) + file->contents.at(y);
                x = file->contents.at(y - 1).size();
                relX = x; // Add paging
            }

            // Remove the line
            file->contents.erase(file->contents.begin() + y - 1, file->contents.begin() + y);
            moveUp();
            break;
        }

        // Remiove the char before the cursor
        file->contents.at(y).erase(file->contents.at(y).begin() + x - 1, file->contents.at(y).begin() + x);
        x--;

        relX = x; // Add paging
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
            string nl = file->contents.at(y);
            nl.erase(nl.begin(), nl.begin() + x);
            // Inserting it to the new line
            file->contents.insert(file->contents.begin() + y + 1, nl);
            // Removing the old contents from the line above
            file->contents.at(y).erase(file->contents.at(y).begin() + x, file->contents.at(y).end());
            x = 0;

            relX = x; // Add paging
        } else {
            // New line
            file->contents.insert(file->contents.begin() + y, "");
        }

        moveDown(file->contents.size());
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
        file->contents.at(y).insert(x, ch);
        x++;

        relX = x; // Add paging
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

void editor::input::resetCursor() {
    x = 0;
    relX = 0;
    y = 0;
    relY = 0;

    pageY = 0;
    pageX = 0;
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
