#include "Cell.hpp"

Cell::Cell() {};

Cell::Cell(std::string n, int sa, int sb) {
    name = n;
    sizeA = sa;
    sizeB = sb;
}

void Cell::print() {
    std::cout << "CELL: " << name << " -> " << sizeA << " " << sizeB << ", NETS: ";
    for(auto it = nets.begin(); it != nets.end(); it++)
        std::cout << (*it)->name << " ";
    std::cout << std::endl;
}

void Cell::add_net(Net* pNet) {
    nets.push_back(pNet);
    pinNum = nets.size();
}
