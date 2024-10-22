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

#ifndef COORD_HPP
#define COORD_HPP
#include "Coord.hpp"
#endif

#ifndef NODE_HPP
#define NODE_HPP
#include "Node.hpp"
#endif

#ifndef TIMER_HPP
#define TIMER_HPP
#include "Timer.hpp"
#endif

class Floorplan {
public:
    // Variables
    unsigned int seed;
    int num_hardblocks;
    int num_terminals;
    int num_nets;
    int total_area;
    int best_w, best_h;
    double dead_space_ratio;
    Coord max_coord;
    std::unordered_map<std::string, Hardblock> hardblocks;
    std::unordered_map<std::string, Pin> pins;
    std::vector<Net> nets;
    Timer prog_t, io_t, area_t, wire_t;

    // Constructors
    Floorplan();
    Floorplan(std::string hardblocks_file, std::string nets_file, std::string pins_file, std::string output, double ratio);

    // Functions
    void set_seed();
    void read_hardblocks(std::string filename);
    void read_pins(std::string filename);
    void read_nets(std::string filename);
    void write_floorplan(std::string filename);
    void calc_max_coord();
    void calc_total_area();
    void update_coord(std::vector<std::vector<Node>>& record, int index, int min_at);
    void swap_adjacent_operand(std::vector<std::string>& sol);
    void invert_chain(std::vector<std::string>& sol);
    void swap_operand_operator(std::vector<std::string>& sol);
    void swap_random_operand(std::vector<std::string>& sol);
    int get_wirelength();
    int get_cost(std::vector<std::string> sol);
    void get_area(std::vector<std::string> sol, int& w, int& h);
    std::vector<std::string> init_sol();
    std::vector<std::string> gen_neighbor(std::vector<std::string> sol, int r);
    std::vector<std::string> simulated_annealing();
    std::vector<Node> stockmeyer(std::vector<Node> l, std::vector<Node> r, std::string type, int index);

    // Utils
    void print();
    void print_hardblocks();
    void print_pins();
    void print_nets();
    void write_result_csv();
};
