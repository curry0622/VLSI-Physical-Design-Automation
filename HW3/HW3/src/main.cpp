#include <bits/stdc++.h>
#include "Floorplan.hpp"

int main(int argc, char *argv[]) {
    // Dummy check
    if(argc > 7) {
        std::cout << "Invalid arguments." << std::endl;
        exit(-1);
    }

    // Floorplan
    Floorplan floorplan(argv[1], argv[2], argv[3], argv[4], std::stod(argv[5]));

    return 0;
}