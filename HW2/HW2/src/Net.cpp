#include "Net.hpp"

Net::Net(){};

Net::Net(std::string n, std::vector<Cell*> cs) {
    name = n;
    cells = cs;
}

void Net::print() {
    std::cout << "NET: " << name << ", CELLS: ";
    for(auto it = cells.begin(); it != cells.end(); it++)
        std::cout << (*it)->name << " ";
    std::cout << std::endl;
}

void Net::calc_num_in_set() {
    numInSetA = 0;
    numInSetB = 0;
    for(auto it = cells.begin(); it != cells.end(); it++) {
        if((*it)->inSetA)
            numInSetA++;
        else
            numInSetB++;
    }
}
