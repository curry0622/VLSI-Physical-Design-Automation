#include "Cell.hpp"

Cell::Cell() {};

Cell::Cell(std::string n, int sa, int sb) {
    name = n;
    sizeA = sa;
    sizeB = sb;
    inSetA = true;
    isLocked = false;
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

void Cell::calc_gain() {
    gain = 0;
    for(auto it = nets.begin(); it != nets.end(); it++) {
        if(inSetA) {
            if((*it)->numInSetA == 1)
                gain++;
            if((*it)->numInSetB == 0)
                gain--;
        }
        else {
            if((*it)->numInSetB == 1)
                gain++;
            if((*it)->numInSetA == 0)
                gain--;
        }
    }
}