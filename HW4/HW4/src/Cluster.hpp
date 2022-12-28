#include <bits/stdc++.h>

#ifndef NODE_HPP
#define NODE_HPP
#include "Node.hpp"
#endif

class Cluster {
public:
    // Variables
    double x;
    std::vector<Node*> cells;

    // Constructors
    Cluster();
    Cluster(double x);

    // Functions
    int get_width();

    // Utils
};
