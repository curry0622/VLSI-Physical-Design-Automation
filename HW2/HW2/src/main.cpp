#include <bits/stdc++.h>
#include "FM.hpp"

int main(int argc, char *argv[]) {
    // Dummy check
    if(argc != 4) {
        std::cout << "Invalid arguments." << std::endl;
        exit(-1);
    }

    // Fiduccia-Mattheyses algorithm
    FM fm(argv[1], argv[2], argv[3]);

    return 0;
}
