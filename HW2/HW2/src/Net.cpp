#include "Net.hpp"

Net::Net(){};

Net::Net(std::string n, std::vector<Cell*> cs) {
    name = n;
    cells = cs;
    numInSetA = 0;
    numInSetB = 0;
}

void Net::calc_num_in_set() {
    numInSetA = 0;
    numInSetB = 0;
    for(auto cell : cells) {
        if(cell->inSetA) {
            numInSetA++;
        } else {
            numInSetB++;
        }
    }
}

void Net::print() {
    std::cout << "NET: " << name << std::endl;
    for(auto cell : cells) {
        if(cell->inSetA) {
            std::cout << "A: " << cell->name << " ";
        }
    }
    std::cout << std::endl;
    for(auto cell : cells) {
        if(!cell->inSetA) {
            std::cout << "B: " << cell->name << " ";
        }
    }
    std::cout << std::endl;
}
