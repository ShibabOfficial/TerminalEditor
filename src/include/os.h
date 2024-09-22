#pragma once

#ifndef OS_H
#define OS_H

#include <string>

namespace os {
    inline std::string getOS() {
        // Detect current OS
        #if defined(__linux__)
            return"Linux";
        #elif __FreeBSD__
            return "FreeBSD";
        #elif __ANDROID__
            return "Android";
        #elif __APPLE__
            return "Mac";
        #elif _WIN32
            return "Win";
        #else
            return "unknown";
        #endif
    }
}

#endif
