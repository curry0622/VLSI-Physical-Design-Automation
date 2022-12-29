#include <bits/stdc++.h>
#include "Legalizer.hpp"

int main(int argc, char *argv[]) {
    // Dummy check
    if(argc > 3) {
        std::cout << "Invalid arguments." << std::endl;
        exit(-1);
    }

    // Legalizer
    Legalizer legalizer(argv[1], argv[2]);

    return 0;
}
