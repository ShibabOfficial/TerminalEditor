#pragma once

#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include <vector>
#include <ncurses.h>
#include <string>

#include "utils/string.h"
#include "input.h"

using namespace std;

namespace editor {
    class commandHandler {
    public:
        static void execute(vector<string> command, input *input, bool &running);
    };
}

#endif