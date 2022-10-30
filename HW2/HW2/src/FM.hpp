#include <bits/stdc++.h>
// #include <boost/sort/sort.hpp>
#include "Net.hpp"
#include "Cell.hpp"
#include "BucketList.hpp"

class FM {
public:
    std::unordered_map<std::string, Net> nets;
    std::unordered_map<std::string, Cell> cells;
    std::vector<Cell*> selectedBaseCells;
    std::vector<int> maxGains;
    BucketList setA, setB;
    Cell* baseCell;
    int maxPinNum = INT_MIN;
    int maxPartialSum;
    int maxPartialSumIndex;

    FM();
    FM(std::string cellFile, std::string netFile, std::string outFile);
    void print_cells();
    void print_nets();
    void write_result(std::string filename);
    void read_cells(std::string filename);
    void read_nets(std::string filename);
    void initial_partition();
    bool is_balanced(int sizeA, int sizeB);
    bool select_base_cell();
    bool select_base_cell_v2();
    void calc_max_partial_sum();
    void update_cells_gain();
    void run_pass();
    void reset_lock();
    void print_sets();
    void pause();
    void print_selected_base_cells();
    void roll_back_from(int index);
};
