#include <bits/stdc++.h>
#include "FM.hpp"

int main(int argc, char *argv[]) {
    // Dummy check
    if(argc != 3) {
        std::cout << "Invalid arguments." << std::endl;
        exit(-1);
    }

    FM fm;

    fm.read_cells(argv[1]);
    fm.read_nets(argv[2]);

    return 0;
}
