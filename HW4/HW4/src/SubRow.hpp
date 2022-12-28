#include <bits/stdc++.h>

#ifndef CLUSTER_HPP
#define CLUSTER_HPP
#include "Cluster.hpp"
#endif

class SubRow {
public:
    // Variables
    double min_x, max_x;
    int free_width;
    std::vector<Cluster*> clusters;

    // Constructors
    SubRow();
    SubRow(double min_x, double max_x);

    // Functions
    void set_xs(double min_x, double max_x);

    // Utils
    void print();
};
