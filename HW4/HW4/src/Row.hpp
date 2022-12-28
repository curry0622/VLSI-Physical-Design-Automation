#include <bits/stdc++.h>

#ifndef SUBROW_HPP
#define SUBROW_HPP
#include "SubRow.hpp"
#endif

#ifndef NODE_HPP
#define NODE_HPP
#include "Node.hpp"
#endif

class Row {
public:
    // Variables
    double x, y;
    int h;
    int num_sites;
    int site_width;
    std::vector<SubRow*> subrows;

    // Constructors
    Row();
    Row(double x, double y, int h, int num_sites, int site_width);
    void slice(Node* blockage);

    // Functions

    // Utils
    void print();
};
