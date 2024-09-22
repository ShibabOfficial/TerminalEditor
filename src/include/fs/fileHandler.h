#pragma once

#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <iostream>
#include <vector>
#include <ncurses.h>
#include <string>

#include "fs/file.h"
#include "utils/string.h"

using namespace std;


// Note: inline prevens from link errors with headers
namespace fileHandler {
    inline file* _file = new file();
    
    inline file* getFile() {
        return _file;
    }

    inline void setFile(file* f) {
        _file = f;
        _file->read();
    }
};

#endif