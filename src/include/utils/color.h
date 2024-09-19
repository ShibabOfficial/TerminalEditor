#pragma once

#ifndef UTILS_COLOR_H
#define UTILS_COLOR_H

#include <ncurses.h>

using namespace std;

// Note: inline prevens from link errors with headers
namespace utils {
    namespace colors {
        short DARK_GRAY   = 10;
        short GRAY        = 11;
        short LIGHT_GRAY  = 12;
        short WHITE       = 13;
        short DEEP_BLUE   = 14;
        short AQUA        = 15;
        short LIGH_PURPLE = 16;
        short PURPLE      = 17;

        inline void colorRGB(short colorId, uint8_t r, uint8_t g, uint8_t b) {
            short rs = r * 1000 / 256;
            short gs = g * 1000 / 256;
            short bs = b * 1000 / 256;
            init_color(colorId, rs, gs, bs);
        }

        inline void color(short id, short fgId, short bgId) {
            init_pair(id, fgId, bgId);
        }

        inline void init() {
            start_color();
            utils::colors::colorRGB(10, 41, 41, 41);    // Dark gray
            utils::colors::colorRGB(11, 49, 53, 61);    // Gray
            utils::colors::colorRGB(12, 140, 140, 140); // Light gray
            utils::colors::colorRGB(13, 217, 217, 217); // White
            utils::colors::colorRGB(14, 41, 98, 255);   // Deep Blue
            utils::colors::colorRGB(15, 0, 146, 178);   // Aqua
            utils::colors::colorRGB(16, 146, 80, 188);  // Light purple
            utils::colors::colorRGB(17, 156, 39, 176);  // Purple
        }
    }
}

#endif