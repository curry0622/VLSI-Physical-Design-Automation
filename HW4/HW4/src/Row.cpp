#include "Row.hpp"

Row::Row() {
    x = 0;
    y = 0;
    h = 0;
    num_sites = 0;
    site_width = 0;
}

Row::Row(double x, double y, int h, int num_sites, int site_width) {
    this->x = x;
    this->y = y;
    this->h = h;
    this->num_sites = num_sites;
    this->site_width = site_width;
}

void Row::slice(Node* blockage) {
    // Variables
    SubRow* lsr = subrows.back(); // last subrow
    double lsr_min_x = lsr->x, lsr_max_x = lsr->x + lsr->w;
    double blkg_min_x = blockage->x, blkg_max_x = blockage->x + blockage->w;

    // Slice row into subrows
    if(lsr_min_x < blkg_min_x) {
        if(lsr_max_x > blkg_max_x) {
            subrows.push_back(new SubRow(blkg_max_x, lsr_max_x - blkg_max_x));
        }
        lsr->set_xw(lsr_min_x, blkg_min_x - lsr_min_x);
    } else {
        if(lsr_max_x > blkg_max_x) {
            lsr->set_xw(blkg_max_x, lsr_max_x - blkg_max_x);
        } else {
            subrows.pop_back();
        }
    }
}

void Row::print() {
    std::cout << "[Row]" << std::endl;
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    std::cout << "h: " << h << std::endl;
    std::cout << "num_sites: " << num_sites << std::endl;
    std::cout << "site_width: " << site_width << std::endl;
    std::cout << "subrows: " << std::endl;
    for(int i = 0; i < subrows.size(); i++) {
        subrows[i]->print();
    }
}
