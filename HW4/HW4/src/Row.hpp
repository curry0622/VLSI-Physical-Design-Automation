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
    int x, y;
    int height;
    int num_sites;
    int site_width;
    std::vector<SubRow*> subrows;

    // Constructors
    Row();
    Row(int x, int y, int h, int num_sites, int site_width);

    // Functions
    void slice(Node* blockage);

    // Utils
    void print();
};
