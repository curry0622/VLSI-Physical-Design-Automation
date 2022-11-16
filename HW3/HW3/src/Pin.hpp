#include <bits/stdc++.h>

#ifndef COORD_HPP
#define COORD_HPP
#include "Coord.hpp"
#endif

class Pin {
public:
    // Variables
    std::string name;
    Coord coord;

    // Constructors
    Pin();
    Pin(std::string n, int x, int y);

    // Functions
    void print();
};
