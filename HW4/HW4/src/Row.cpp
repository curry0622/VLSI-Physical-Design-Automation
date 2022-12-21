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

void Row::split_row(Node* blockage) {
    // Variables
    RowSection* last_section = row_sections.back();
    double lsMinX = last_section->x, lsMaxX = last_section->x + last_section->w;
    double bMinX = blockage->x, bMaxX = blockage->x + blockage->w;

    // Split row into sections
    if(lsMinX < bMinX) {
        if(lsMaxX > bMaxX) {
            row_sections.push_back(new RowSection(bMaxX, lsMaxX - bMaxX));
        }
        last_section->set_xw(lsMinX, bMinX - lsMinX);
    } else {
        if(lsMaxX > bMaxX) {
            last_section->set_xw(bMaxX, lsMaxX - bMaxX);
        } else {
            row_sections.pop_back();
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
    std::cout << "row_sections: " << std::endl;
    for(int i = 0; i < row_sections.size(); i++) {
        row_sections[i]->print();
    }
}
