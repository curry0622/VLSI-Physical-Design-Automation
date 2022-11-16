#include <bits/stdc++.h>
#include "Pin.hpp"

void read_pins(std::string filename) {
    std::ifstream fin(filename);
    std::string line;
    while(std::getline(fin, line)) {
        std::stringstream ss(line);
        std::string name;
        int x_cord, y_cord;
        ss >> name >> x_cord >> y_cord;
        Pin p = Pin(name, x_cord, y_cord);
        p.print();
    }
}

int main(int argc, char *argv[]) {
    // Dummy check
    if(argc != 6) {
        std::cout << "Invalid arguments." << std::endl;
        exit(-1);
    }

    // Read pins
    read_pins(argv[3]);

    return 0;
}