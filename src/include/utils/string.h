#pragma once

#ifndef UTILS_STRING_H
#define UTILS_STRING_H

#include <iostream>
#include <memory>
#include <string>
#include <sstream>

#include <stdexcept>
#include <vector>

using namespace std;

// Note: inline prevens from link errors with headers
namespace utils {
    inline vector<string> splitString(string s, string delimiter) {
        size_t pos = 0;
        vector<string> sf;

        while ((pos = s.find(delimiter)) != string::npos) {
            sf.push_back(s.substr(0, pos));
            s.erase(0, pos + delimiter.length());
        }

        sf.push_back(s);

        return sf;
    }

    template <typename T> inline string toString(const T& value) {
        ostringstream ss;
        ss << value;
        return ss.str();
    }
}

#endif