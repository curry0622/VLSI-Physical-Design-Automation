#include "Net.hpp"

Net::Net(){};

Net::Net(std::string n, std::vector<std::string> cs) {
    name = n;
    cells = cs;
}

void Net::print() {
    std::cout << "NET: " << name << ", CELLS: ";
    for (auto it = cells.begin(); it != cells.end(); it++){
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}
