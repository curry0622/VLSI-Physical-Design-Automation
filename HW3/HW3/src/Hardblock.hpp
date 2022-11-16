#include <bits/stdc++.h>

#ifndef CORD_HPP
#define CORD_HPP
#include "Cord.hpp"
#endif

class Hardblock {
public:
    // Variables
    std::string name;
    Cord cord;
    int width, height;
    int rotated;

    // Constructors
    Hardblock();
    Hardblock(std::string name, Cord cord, int width, int height);

    // Functions
    Cord get_center();
    void rotate();
    void print();
};
