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
    // std::cout << "---Calc gain of " << name << "---" << std::endl;
    gain = 0;
    for(auto it = nets.begin(); it != nets.end(); it++) {
        if(inSetA) {
            if((*it)->numInSetA == 1) {
                add_gain();
                // std::cout << "add_gain()" << std::endl;
            }
            if((*it)->numInSetB == 0) {
                sub_gain();
                // std::cout << "sub_gain()" << std::endl;
            }
        }
        else {
            if((*it)->numInSetB == 1) {
                add_gain();
                // std::cout << "add_gain()" << std::endl;
            }
            if((*it)->numInSetA == 0) {
                sub_gain();
                // std::cout << "sub_gain()" << std::endl;
            }
        }
        // std::cout << "Curr gain: " << gain << std::endl;
    }
    // std::cout << "---Final gain of " << name << " is " << gain << "---" << std::endl;
}

void Cell::add_gain() {
    gain++;
}

void Cell::sub_gain() {
    gain--;
}
