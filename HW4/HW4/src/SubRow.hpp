#include <bits/stdc++.h>

#ifndef CLUSTER_HPP
#define CLUSTER_HPP
#include "Cluster.hpp"
#endif

class SubRow {
public:
    // Variables
    double x;
    int w;
    std::vector<Cluster*> clusters;

    // Constructors
    SubRow();
    SubRow(double x, int w);

    // Functions
    void set_xw(double x, int w);

    // Utils
    void print();
};
