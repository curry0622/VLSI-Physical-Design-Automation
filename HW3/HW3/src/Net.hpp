#include <bits/stdc++.h>

#ifndef PIN_HPP
#define PIN_HPP
#include "Pin.hpp"
#endif

#ifndef HARDBLOCK_HPP
#define HARDBLOCK_HPP
#include "Hardblock.hpp"
#endif

class Net {
public:
    // Variables
    std::vector<Pin> pins;
    std::vector<Hardblock> hardblocks;

    // Constructors
    Net();

    // Functions
    void add_pin(Pin pin);
    void add_hardblock(Hardblock hardblock);
    void print();
};
