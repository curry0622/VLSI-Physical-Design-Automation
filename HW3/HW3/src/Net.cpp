#include "Net.hpp"

Net::Net() {
    pins = std::vector<Pin*>();
    hardblocks = std::vector<Hardblock*>();
}

void Net::add_pin(Pin* pin) {
    pins.push_back(pin);
}

void Net::add_hardblock(Hardblock* hardblock) {
    hardblocks.push_back(hardblock);
}

int Net::HPWL() {
    int min_x = INT_MAX, max_x = INT_MIN, min_y = INT_MAX, max_y = INT_MIN;
    for(auto pin : pins) {
        Coord c = pin->coord;
        min_x = std::min(min_x, c.x);
        max_x = std::max(max_x, c.x);
        min_y = std::min(min_y, c.y);
        max_y = std::max(max_y, c.y);
    }
    for(auto hardblock : hardblocks) {
        Coord c = hardblock->get_center();
        min_x = std::min(min_x, c.x);
        max_x = std::max(max_x, c.x);
        min_y = std::min(min_y, c.y);
        max_y = std::max(max_y, c.y);
    }
    return (max_x - min_x) + (max_y - min_y);
}

void Net::print() {
    std::cout << "---" << std::endl;
    std::cout << "Net pins:" << std::endl;
    for(auto pin : pins) {
        Coord c = pin->coord;
        std::cout << pin->name << " (" << c.x << ", " << c.y  << ")" << std::endl;
    }
    if(pins.size() == 0) {
        std::cout << "None" << std::endl;
    }
    std::cout << "Net hardblocks:" << std::endl;
    for(auto hardblock : hardblocks) {
        Coord c = hardblock->get_center();
        std::cout << hardblock->name << " (" << c.x << ", " << c.y << ")" << std::endl;
    }
    if(hardblocks.size() == 0) {
        std::cout << "None" << std::endl;
    }
    std::cout << "Net HPWL: " << HPWL() << std::endl;
    std::cout << "---" << std::endl;
}