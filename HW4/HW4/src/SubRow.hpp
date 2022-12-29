#include <bits/stdc++.h>

#ifndef CLUSTER_HPP
#define CLUSTER_HPP
#include "Cluster.hpp"
#endif

class SubRow {
public:
    // Variables
    int min_x, max_x;
    int free_width;
    Cluster* last_cluster;

    // Constructors
    SubRow();
    SubRow(int min_x, int max_x);

    // Functions
    void set_xs(int min_x, int max_x);

    // Utils
    void print();
};
