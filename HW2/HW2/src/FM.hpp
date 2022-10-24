#include <bits/stdc++.h>
#include "Net.hpp"

class FM {
public:
    std::vector<Net> nets;
    std::map<std::string, Cell> cells;
    FM();
    void read_cells(std::string filename);
    void read_nets(std::string filename);
    void print_cells();
    void print_nets();
    void write_result(std::string filename);
};
