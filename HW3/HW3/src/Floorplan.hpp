#include <bits/stdc++.h>
#include "Hardblock.hpp"
#include "Pin.hpp"

class Floorplan {
public:
    // Variables
    int num_hardblocks;
    int num_terminals;
    double dead_space_ratio;

    // Constructors
    Floorplan();
    Floorplan(std::string hardblocks_file, std::string nets_file, std::string pins_file, std::string output, double ratio);

    // Functions
    void read_hardblocks(std::string filename);
    void read_nets(std::string filename);
    void read_pins(std::string filename);
    void write_floorplan(std::string filename);
    void print();
};
