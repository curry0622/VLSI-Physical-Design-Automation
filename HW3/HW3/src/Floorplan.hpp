#include <bits/stdc++.h>
#include "Net.hpp"

#ifndef HARDBLOCK_HPP
#define HARDBLOCK_HPP
#include "Hardblock.hpp"
#endif

#ifndef PIN_HPP
#define PIN_HPP
#include "Pin.hpp"
#endif

class Floorplan {
public:
    // Variables
    int num_hardblocks;
    int num_terminals;
    int num_nets;
    double dead_space_ratio;
    std::unordered_map<std::string, Hardblock> hardblocks;
    std::unordered_map<std::string, Pin> pins;
    std::vector<Net> nets;

    // Constructors
    Floorplan();
    Floorplan(std::string hardblocks_file, std::string nets_file, std::string pins_file, std::string output, double ratio);

    // Functions
    void read_hardblocks(std::string filename);
    void read_pins(std::string filename);
    void read_nets(std::string filename);
    void write_floorplan(std::string filename);
    void print();
    void print_hardblocks();
    void print_pins();
    void print_nets();
};
