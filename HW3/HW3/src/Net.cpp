#include "Net.hpp"

Net::Net() {
    pins = std::vector<Pin>();
    hardblocks = std::vector<Hardblock>();
}

void Net::add_pin(Pin pin) {
    pins.push_back(pin);
}

void Net::add_hardblock(Hardblock hardblock) {
    hardblocks.push_back(hardblock);
}

void Net::print() {
    std::cout << "---" << std::endl;
    std::cout << "Net pins:" << std::endl;
    for(auto p : pins) {
        std::cout << p.name << std::endl;
    }
    if(pins.size() == 0) {
        std::cout << "None" << std::endl;
    }
    std::cout << "Net hardblocks:" << std::endl;
    for(auto hb : hardblocks) {
        std::cout << hb.name << std::endl;
    }
    if(hardblocks.size() == 0) {
        std::cout << "None" << std::endl;
    }
    std::cout << "---" << std::endl;
}