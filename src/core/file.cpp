#include "file.h"

file::file(char* url) {
    this->path = url;

    vector<string> fpath = utils::splitString(url, "/");
    this->fullname = fpath.at(fpath.size() - 1);

    vector<string> fname = utils::splitString(fullname, ".");
    this->name = fname.at(0);
    this->ext = fname.at(fname.size() - 1);
}

int file::getContents(vector<string>& out) {
    std::ifstream f(this->path, std::ios::in | std::ios::binary);
    const auto sz = filesystem::file_size(this->path);

    string result(sz, '\0');
    f.read(result.data(), sz);

    out = utils::splitString(result, "\n");

    return 0;
}
