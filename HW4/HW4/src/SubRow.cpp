#include "SubRow.hpp"

SubRow::SubRow() {
    min_x = 0;
    max_x = 0;
    free_width = 0;
}

SubRow::SubRow(double min_x, double max_x) {
    set_xs(min_x, max_x);
}

void SubRow::set_xs(double min_x, double max_x) {
    this->min_x = min_x;
    this->max_x = max_x;
    free_width = max_x - min_x;
}

void SubRow::print() {
    std::cout << "[SubRow]" << std::endl;
    std::cout << "min_x: " << min_x << std::endl;
    std::cout << "max_x: " << max_x << std::endl;
}
