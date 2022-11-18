#include <bits/stdc++.h>

#ifndef NET_HPP
#define NET_HPP
#include "Net.hpp"
#endif

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
    int total_area;
    double dead_space_ratio;
    Coord max_coord;
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
    void calc_max_coord();
    void calc_total_area();
    int get_wirelength();
    int get_area(std::vector<std::string> sol);
    std::vector<std::string> init_sol();
    std::pair<int, int> get_min_area_comb(std::vector<std::pair<int, int>> a, std::vector<std::pair<int, int>> b, std::string type);
    std::vector<std::pair<int, int>> stockmeyer(
        std::vector<std::pair<int, int>> l,
        std::vector<std::pair<int, int>> r,
        std::string type
    );

    // Utils
    void print();
    void print_hardblocks();
    void print_pins();
    void print_nets();
};
