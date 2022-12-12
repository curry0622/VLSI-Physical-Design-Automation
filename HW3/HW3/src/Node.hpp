#include <bits/stdc++.h>

#ifndef COORD_HPP
#define COORD_HPP
#include "Coord.hpp"
#endif

class Node {
public:
    // Variables
    std::string type;
    int index;
    int width, height;
    int left_from, left_at;
    int right_from, right_at;
    Coord coord;

    // Constructors
    Node();
    Node(std::string type, int index, int width, int height, int left_from, int left_at, int right_from, int right_at, Coord coord);

    // Utils
    void print();
};

