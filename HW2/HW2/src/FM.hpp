#include <bits/stdc++.h>
#include "Net.hpp"
#include "Cell.hpp"

class FM {
public:
    std::unordered_map<std::string, Net> nets;
    std::unordered_map<std::string, Cell> cells;
    std::set<std::string> setA, setB;

    FM();
    void read_cells(std::string filename);
    void read_nets(std::string filename);
    void print_cells();
    void print_nets();
    void write_result(std::string filename);
};
