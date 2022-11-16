#include "Floorplan.hpp"

Floorplan::Floorplan() {
    dead_space_ratio = -1;
}

Floorplan::Floorplan(std::string hardblocks_file, std::string nets_file, std::string pins_file, std::string output, double ratio) {
    dead_space_ratio = ratio;
    read_hardblocks(hardblocks_file);
    read_nets(nets_file);
    read_pins(pins_file);
    // print();
}

void Floorplan::read_hardblocks(std::string filename) {}

void Floorplan::read_nets(std::string filename) {}

void Floorplan::read_pins(std::string filename) {
    std::ifstream fin(filename);
    std::string line;
    while(std::getline(fin, line)) {
        std::stringstream ss(line);
        std::string name;
        int x_cord, y_cord;
        ss >> name >> x_cord >> y_cord;
        Pin p = Pin(name, x_cord, y_cord);
        // p.print();
    }
}

void Floorplan::print() {
    std::cout << "---" << std::endl;
    std::cout << "Floorplan dead_space_ratio: " << dead_space_ratio << std::endl;
    std::cout << "---" << std::endl;
}
