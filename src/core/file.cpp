#include "file.h"

file::file(char* url) {
    this->path = url;

    vector<string> fpath = utils::splitString(url, "/");
    this->fullname = fpath.at(fpath.size() - 1);

    vector<string> fname = utils::splitString(fullname, ".");
    this->name = fname.at(0);
    this->ext = fname.at(fname.size() - 1);
}

int file::getContents(string& out) {
    ifstream inpt(this->path);

    string buffer;

    if (!inpt.is_open()) { 
        cerr << format("Error opening the file '{}'", this->path) << endl;
        return 1; 
    }

    while(getline(inpt, buffer)) {
        out += buffer + "\n";
    }

    out.erase(out.length() - 1, out.length());

    inpt.close();

    return 0;
}
