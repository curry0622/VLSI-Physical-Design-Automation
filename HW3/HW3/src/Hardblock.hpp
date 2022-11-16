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

    // Constructors
    Hardblock();
    Hardblock(std::string name, Cord cord[]);

    // Functions
    void print();
};
