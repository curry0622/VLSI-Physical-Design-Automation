#include <bits/stdc++.h>
#include "FM.hpp"

int main(int argc, char *argv[]) {
    // Dummy check
    if(argc != 4) {
        std::cout << "Invalid arguments." << std::endl;
        exit(-1);
    }

    FM fm(argv[1], argv[2]);

    // fm.print_cells();
    // fm.print_nets();

    // fm.write_result(argv[3]);

    return 0;
}
