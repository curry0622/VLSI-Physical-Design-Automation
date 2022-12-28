#include <bits/stdc++.h>

#ifndef NODE_HPP
#define NODE_HPP
#include "Node.hpp"
#endif

class Cluster {
public:
    // Variables
    double x;
    double q;
    int width;
    int weight;
    std::vector<Node*> cells;
    Cluster* pre;

    // Constructors
    Cluster();
    Cluster(double x);

    // Functions
    void add_cell(Node* cell);

    // Utils
};
