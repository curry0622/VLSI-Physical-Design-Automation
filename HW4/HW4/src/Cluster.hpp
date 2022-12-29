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
    // double weight;
    std::vector<Node*> cells;
    Cluster* pre;

    // Constructors
    Cluster();
    Cluster(double x, double q, int width, int weight, Cluster* pre);

    // Functions

    // Utils
};
