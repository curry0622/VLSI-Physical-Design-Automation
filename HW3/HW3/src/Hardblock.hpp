#include <bits/stdc++.h>

#ifndef CORD_HPP
#define CORD_HPP
#include "Cord.hpp"
#endif

class Hardblock {
public:
    // Variables
    std::string name;
    Cord cord[4];
    int rotated;

    // Constructors
    Hardblock();
    Hardblock(std::string name, Cord cord[]);

    // Functions
    Cord get_center();
    void rotate();
    void print();
};
