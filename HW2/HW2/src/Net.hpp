#include <bits/stdc++.h>
#pragma once
#include "Cell.hpp"

class Cell;

class Net {
public:
    std::string name;
    std::vector<Cell*> cells;

    Net();
    Net(std::string n, std::vector<Cell*> cs);
    void print();
};
