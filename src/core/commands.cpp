#include "commands.h"

void editor::commandHandler::execute(vector<string> command, editor::input* input, bool& running) {
    // Command quit
    if (command.at(0) == ":quit" ||
        command.at(0) == ":qu" ||
        command.at(0) == ":q") {
        running = false;
        return;
    }
    
    // Command edit
    if (command.at(0) == ":edit" ||
        command.at(0) == ":ed" ||
        command.at(0) == ":e") {
        
        if (fileHandler::getFile()->contents.size() < 0) return;
        
        input->setMode(editor::EDITING);
        return;
    }

    // Command open
    if (command.at(0) == ":open" ||
        command.at(0) == ":op" ||
        command.at(0) == ":o") {
        
        if (command.size() <= 1) return;

        fileHandler::setFile(new file(command.at(1).c_str()));
        input->resetCursor();
        return;
    }
}
