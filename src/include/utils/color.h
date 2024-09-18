#pragma once

#ifndef UTILS_COLOR_H
#define UTILS_COLOR_H

#include <ncurses.h>

using namespace std;

// Note: inline prevens from link errors with headers
namespace utils {
    inline void colorRGB(short colorId, uint8_t r, uint8_t g, uint8_t b) {
        short rs = r * 1000 / 256;
        short gs = g * 1000 / 256;
        short bs = b * 1000 / 256;
        init_color(colorId, rs, gs, bs);
    }

    inline void color(short id, short fgId, short bgId) {
        init_pair(id, fgId, bgId);
    }
}

#endif