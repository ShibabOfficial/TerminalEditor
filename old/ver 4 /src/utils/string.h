#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <sstream>

#include <stdexcept>
#include <vector>

using namespace std;

namespace utils {
    vector<string> split(string s, string delimiter) {
        size_t pos = 0;
        vector<string> sf;

        while ((pos = s.find(delimiter)) != string::npos) {
            sf.push_back(s.substr(0, pos));
            s.erase(0, pos + delimiter.length());
        }

        sf.push_back(s);

        return sf;
    }

    template <typename T> string to_string(const T& value) {
        ostringstream ss;
        ss << value;
        return ss.str();
    }
}
