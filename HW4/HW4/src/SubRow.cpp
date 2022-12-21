#include "SubRow.hpp"

SubRow::SubRow() {
    x = 0;
    w = 0;
}

SubRow::SubRow(double x, int w) {
    this->x = x;
    this->w = w;
}

void SubRow::set_xw(double x, int w) {
    this->x = x;
    this->w = w;
}

void SubRow::print() {
    std::cout << "[SubRow]" << std::endl;
    std::cout << "x: " << x << std::endl;
    std::cout << "w: " << w << std::endl;
}
