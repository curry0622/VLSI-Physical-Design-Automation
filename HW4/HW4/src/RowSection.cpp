#include "RowSection.hpp"

RowSection::RowSection() {
    x = 0;
    w = 0;
}

RowSection::RowSection(double x, int w) {
    this->x = x;
    this->w = w;
}

void RowSection::print() {
    std::cout << "[RowSection]" << std::endl;
    std::cout << "x: " << x << std::endl;
    std::cout << "w: " << w << std::endl;
}
