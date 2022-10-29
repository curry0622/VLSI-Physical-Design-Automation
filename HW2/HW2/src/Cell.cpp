#include "Cell.hpp"

Cell::Cell() {};

Cell::Cell(std::string n, int sa, int sb) {
    name = n;
    nets = std::vector<Net*>{};
    sizeA = sa;
    sizeB = sb;
    pinNum = 0;
    gain = 0;
    inSetA = true;
    isLocked = false;
}

void Cell::add_net(Net* pNet) {
    nets.push_back(pNet);
    pinNum = nets.size();
}

void Cell::add_gain() {
    gain++;
}

void Cell::sub_gain() {
    gain--;
}

void Cell::calc_gain() {
    gain = 0;
    for(auto net : nets) {
        if(inSetA) {
            if(net->numInSetA == 1)
                add_gain();
            if(net->numInSetB == 0)
                sub_gain();
        } else {
            if(net->numInSetB == 1)
                add_gain();
            if(net->numInSetA == 0)
                sub_gain();
        }
    }
}

void Cell::print() {
    std::cout << "CELL: " << name << " -> " << sizeA << " " << sizeB << ", NETS: ";
    for(auto net : nets) {
        std::cout << net->name << " ";
    }
    std::cout << std::endl;
}
