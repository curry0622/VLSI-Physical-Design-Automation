#include "Row.hpp"

Row::Row() {
    x = 0;
    y = 0;
    height = 0;
    num_sites = 0;
    site_width = 0;
}

Row::Row(double x, double y, int height, int num_sites, int site_width) {
    this->x = x;
    this->y = y;
    this->height = height;
    this->num_sites = num_sites;
    this->site_width = site_width;
}

void Row::print() {
    std::cout << "[Row]" << std::endl;
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    std::cout << "height: " << height << std::endl;
    std::cout << "num_sites: " << num_sites << std::endl;
    std::cout << "site_width: " << site_width << std::endl;
}
