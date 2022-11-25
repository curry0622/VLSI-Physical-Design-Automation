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
    int index_in_sol;

    // Constructors
    Hardblock();
    Hardblock(std::string name, Coord coord, int width, int height);

    // Functions
    Coord get_center();
    void set_index_in_sol(int index);

    // Utils
    void print();
};
