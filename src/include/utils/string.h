#pragma once

#ifndef UTILS_STRING_H
#define UTILS_STRING_H

#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <vector>

using namespace std;

// Note: inline prevens from link errors with headers
namespace utils {
    inline vector<string> splitString(string s, string delimiter) {
        size_t pos = 0;
        vector<string> sf;

        // While we can still find the delimiter we split
        while ((pos = s.find(delimiter)) != string::npos) {
            // Split
            sf.push_back(s.substr(0, pos));
            // Remove the split string
            s.erase(0, pos + delimiter.length());
        }

        // Add the rest
        sf.push_back(s);

        return sf;
    }

    template<typename T> inline string toString(const T& value) {
        ostringstream ss;
        ss << value;
        return ss.str();
    }

    template<typename T> inline string join(vector<T> lines, const char* delimiter) {
        std::ostringstream oss;
        std::copy(lines.begin(), lines.end() - 1, std::ostream_iterator<T>(oss, delimiter));
        oss << lines.back();
        return oss.str();
    }
}

#endif