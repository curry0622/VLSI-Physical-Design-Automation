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
    int gain;
    bool inSetA;
    bool isLocked;

    Cell();
    Cell(std::string n, int sa, int sb);
    void add_net(Net* pNet);
    void add_gain();
    void sub_gain();
    void calc_gain();
    void print();
};
