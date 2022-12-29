#include "SubRow.hpp"

SubRow::SubRow() {
    min_x = 0;
    max_x = 0;
    free_width = 0;
    last_cluster = nullptr;
}

SubRow::SubRow(int min_x, int max_x) {
    set_xs(min_x, max_x);
    last_cluster = nullptr;
}

void SubRow::set_xs(int min_x, int max_x) {
    this->min_x = min_x;
    this->max_x = max_x;
    free_width = max_x - min_x;
}

void SubRow::print() {
    std::cout << "[SubRow]" << std::endl;
    std::cout << "min_x: " << min_x << std::endl;
    std::cout << "max_x: " << max_x << std::endl;
}
