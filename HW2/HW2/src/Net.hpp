#include <bits/stdc++.h>
#pragma once
#include "Cell.hpp"

class Cell;

class Net {
public:
    std::string name;
    std::vector<Cell*> cells;
    int numInSetA, numInSetB;

    Net();
    Net(std::string n, std::vector<Cell*> cs);
    void print();
    void calc_num_in_set();
};
