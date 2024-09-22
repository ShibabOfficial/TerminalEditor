#include "file.h"

file::file() {
    this->path = "";
    this->name = "";
    this->fullname = "";
    this->ext = "";
}

file::file(char* url) {
    this->path = url;

    // Every folder name and the file
    vector<string> fpath = utils::splitString(url, "/");
    // Last index of the 'fpath' is the fullname
    this->fullname = fpath.at(fpath.size() - 1);

    // Split the fullname at '.'
    vector<string> fname = utils::splitString(fullname, ".");
    // Name is the first index
    this->name = fname.at(0);
    // File type is the second
    this->ext = fname.at(fname.size() - 1);
}

int file::read() {
    // Setup file stream with the path
    std::ifstream f(this->path, std::ios::in | std::ios::binary);
    // Size of the file
    const auto sz = filesystem::file_size(this->path);

    // Get the contents
    string result(sz, '\0');
    f.read(result.data(), sz);

    // Split them at '\n'
    contents = utils::splitString(result, "\n");

    return 0;
}
