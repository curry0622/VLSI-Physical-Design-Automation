#include <bits/stdc++.h>

#ifndef ROWSECTION_HPP
#define ROWSECTION_HPP
#include "RowSection.hpp"
#endif

class Row {
public:
    // Variables
    double x, y;
    int height;
    int num_sites;
    int site_width;
    std::vector<RowSection*> row_sections;

    // Constructors
    Row();
    Row(double x, double y, int height, int num_sites, int site_width);

    // Functions

    // Utils
    void print();
};
