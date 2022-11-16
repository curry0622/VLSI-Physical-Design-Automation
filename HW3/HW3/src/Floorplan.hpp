#include <bits/stdc++.h>
#include "Hardblock.hpp"
#include "Pin.hpp"

class Floorplan {
public:
    // Variables
    std::string output_file;
    double dead_space_ratio;
    int num_hardblocks;
    int num_terminals;

    // Constructors
    Floorplan();
    Floorplan(std::string hardblocks_file, std::string nets_file, std::string pins_file, std::string output, double ratio);

    // Functions
    void read_hardblocks(std::string filename);
    void read_nets(std::string filename);
    void read_pins(std::string filename);
    void print();
};
