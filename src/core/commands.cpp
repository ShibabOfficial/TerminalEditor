#include "commands.h"

void editor::commandHandler::execute(vector<string> command, editor::input *input, bool &running) {
    if (command.at(0) == ":quit" ||
        command.at(0) == ":qu" ||
        command.at(0) == ":q") {
        running = false;
        return;
    }
    
    if (command.at(0) == ":edit" ||
        command.at(0) == ":ed" ||
        command.at(0) == ":e") {
        input->setMode(editor::EDITING);
        return;
    }
}
