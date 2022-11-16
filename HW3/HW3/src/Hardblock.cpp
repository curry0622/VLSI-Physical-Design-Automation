#include "Hardblock.hpp"

Hardblock::Hardblock() {
    name = "default";
    cord = Cord();
    width = 0;
    height = 0;
    rotated = 0;
}

Hardblock::Hardblock(std::string name, Cord cord, int width, int height) {
    this->name = name;
    this->cord = cord;
    this->width = width;
    this->height = height;
    rotated = 0;
}

Cord Hardblock::get_center() {
    return Cord(cord.x + width / 2, cord.y + height / 2);
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
    std::cout << "Hardblock cord: ", cord.print();
    std::cout << "Hardblock center: ", get_center().print();
    std::cout << "Hardblock rotated: " << rotated << std::endl;
    std::cout << "---" << std::endl;
}
