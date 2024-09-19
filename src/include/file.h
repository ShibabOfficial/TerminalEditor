#pragma once

#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <format>

#include "utils/string.h"

using namespace std;

class file {
public:
    string path;
    string fullname;
    string name;
    string ext;
    vector<string> contents;

    file();
    file(char* url);

    int read();
};

#endif