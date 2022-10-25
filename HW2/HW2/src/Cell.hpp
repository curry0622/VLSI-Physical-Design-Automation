#include <bits/stdc++.h>
#pragma once
#include "Net.hpp"

class Net;

class Cell {
public:
    std::string name;
    std::vector<Net*> nets;
    int sizeA;
    int sizeB;
    int pinNum;
    bool inSetA;

    Cell();
    Cell(std::string n, int sa, int sb);
    void print();
    void add_net(Net* pNet);
};
