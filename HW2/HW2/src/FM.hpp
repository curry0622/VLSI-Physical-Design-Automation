#include <bits/stdc++.h>
// #include <boost/sort/sort.hpp>
#include "Net.hpp"
#include "Cell.hpp"
#include "BucketList.hpp"

class FM {
public:
    std::unordered_map<std::string, Net> nets;
    std::unordered_map<std::string, Cell> cells;
    BucketList setA, setB;
    int maxPinNum = INT_MIN;
    Cell* baseCell;

    FM();
    FM(std::string cellFile, std::string netFile);
    void print_cells();
    void print_nets();
    void write_result(std::string filename);
    void read_cells(std::string filename);
    void read_nets(std::string filename);
    void initial_partition();
    bool is_balanced(int sizeA, int sizeB);
    void select_base_cell();
};
