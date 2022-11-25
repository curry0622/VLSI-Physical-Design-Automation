#include "Hardblock.hpp"

Hardblock::Hardblock() {
    name = "default";
    coord = Coord();
    width = -1;
    height = -1;
    rotated = -1;
    index_in_sol = -1;
}

Hardblock::Hardblock(std::string name, Coord coord, int width, int height) {
    this->name = name;
    this->coord = coord;
    this->width = width;
    this->height = height;
    rotated = 0;
    index_in_sol = -1;
}

Coord Hardblock::get_center() {
    if(rotated)
        return Coord(coord.x + height / 2, coord.y + width / 2);
    return Coord(coord.x + width / 2, coord.y + height / 2);
}

void Hardblock::set_index_in_sol(int index) {
    index_in_sol = index;
}

void Hardblock::print() {
    std::cout << "---" << std::endl;
    std::cout << "Hardblock name: " << name << std::endl;
    std::cout << "Hardblock width: " << width << std::endl;
    std::cout << "Hardblock height: " << height << std::endl;
    std::cout << "Hardblock coord: ", coord.print();
    std::cout << "Hardblock center: ", get_center().print();
    std::cout << "Hardblock rotated: " << rotated << std::endl;
    std::cout << "Hardblock index in solution: " << index_in_sol << std::endl;
    std::cout << "---" << std::endl;
}
