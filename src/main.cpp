#include "parser.h"

int main(int argc, char* argv[]) {
    if (argc == 1) { // [0] = filename
        cout << "Not enough arguments. Needs 1 (filename)\n";
        return 1;
    }
    else {
        string fname = argv[1];
        if (argc >= 3) {
            string arg = argv[2];
            if (arg == "-pp") Parser scan(fname,true);
            else Parser scan(fname,false);
        }
        else Parser scan(fname,false);
        return 0;
    }
}

