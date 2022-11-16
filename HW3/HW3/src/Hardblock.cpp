#include "Hardblock.hpp"

Hardblock::Hardblock() {
    name = "default";
    for(int i = 0; i < 4; i++) {
        cord[i] = Cord();
    }
}

Hardblock::Hardblock(std::string name, Cord cord[]) {
    this->name = name;
    for(int i = 0; i < 4; i++) {
        this->cord[i] = cord[i];
    }
}

void Hardblock::print() {
    std::cout << "---" << std::endl;
    std::cout << "Hardblock name: " << name << std::endl;
    for(int i = 0; i < 4; i++) {
        std::cout << "Hardblock cord " << i << ": ";
        cord[i].print();
    }
    std::cout << "---" << std::endl;
}
