#include "Hardblock.hpp"

Hardblock::Hardblock() {
    name = "default";
    coord = Coord();
    width = -1;
    height = -1;
    rotated = -1;
}

Hardblock::Hardblock(std::string name, Coord coord, int width, int height) {
    this->name = name;
    this->coord = coord;
    this->width = width;
    this->height = height;
    rotated = 0;
}

Coord Hardblock::get_center() {
    return Coord(coord.x + width / 2, coord.y + height / 2);
}

void Hardblock::rotate() {
    rotated = !rotated;
    int temp = width;
    width = height;
    height = temp;
}

void Hardblock::print() {
    std::cout << "---" << std::endl;
    std::cout << "Hardblock name: " << name << std::endl;
    std::cout << "Hardblock width: " << width << std::endl;
    std::cout << "Hardblock height: " << height << std::endl;
    std::cout << "Hardblock coord: ", coord.print();
    std::cout << "Hardblock center: ", get_center().print();
    std::cout << "Hardblock rotated: " << rotated << std::endl;
    std::cout << "---" << std::endl;
}
