#include <bits/stdc++.h>

#ifndef COORD_HPP
#define COORD_HPP
#include "Coord.hpp"
#endif

class Hardblock {
public:
    // Variables
    std::string name;
    Coord coord;
    int width, height;
    int rotated;

    // Constructors
    Hardblock();
    Hardblock(std::string name, Coord coord, int width, int height);

    // Functions
    Coord get_center();

    // Utils
    void print();
};
