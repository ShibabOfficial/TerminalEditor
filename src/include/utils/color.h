#pragma once

#ifndef UTILS_COLOR_H
#define UTILS_COLOR_H

#include <ncurses.h>

using namespace std;

// Note: inline prevens from link errors with headers
namespace utils {
    namespace colors {
        short DARK_GRAY   = 10;
        short MID_GRAY    = 11;
        short GRAY        = 12;
        short LIGHT_GRAY  = 13;
        short WHITE       = 14;
        short DEEP_BLUE   = 15;
        short AQUA        = 16;
        short LIGH_PURPLE = 17;
        short PURPLE      = 18;

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
            utils::colors::colorRGB(11, 46, 51, 56);    // Mid gray
            utils::colors::colorRGB(12, 49, 53, 61);    // Gray
            utils::colors::colorRGB(13, 140, 140, 140); // Light gray
            utils::colors::colorRGB(14, 217, 217, 217); // White
            utils::colors::colorRGB(15, 41, 98, 255);   // Deep Blue
            utils::colors::colorRGB(16, 0, 146, 178);   // Aqua
            utils::colors::colorRGB(17, 146, 80, 188);  // Light purple
            utils::colors::colorRGB(18, 156, 39, 176);  // Purple
        }
    }
}

#endif