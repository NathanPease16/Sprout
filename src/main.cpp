#include "./headers/commands.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::string argument;
    bool sFlag = false, cFlag = false, dFlag = false, vFlag = false, hFlag = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-s") {
            if (i + 1 < argc) {
                sFlag = true;
                argument = argv[++i];
            } else {
                std::cerr << "Error: -s option requires an argument" << std::endl;
            }
        } else if (arg == "-c") {
            if (i + 1 < argc) {
                cFlag = true;
                argument = argv[++i];
            } else {
                std::cerr << "Error: -c option requires an argument" << std::endl;
            }
        } else if (arg == "-d") {
            if (i + 1 < argc) {
                dFlag = true;
                argument = argv[++i];
            } else {
                std::cerr << "Error: -d option requires an argument" << std::endl;
            }
        } else if (arg == "-v") {
            vFlag = true;
        } else if (arg == "-h") {
            hFlag = true;
        } else {
            argument = arg;
        }
    }

    if (sFlag) {
        sprout(argument);
    } else if (cFlag) {
        create("./", argument);
    } else if (dFlag) {
        remove(argument);
    } else if (vFlag) {
        view();
    } else if (!argument.empty()) {
        sprout(argument);
    } else {
        help();
    }
}