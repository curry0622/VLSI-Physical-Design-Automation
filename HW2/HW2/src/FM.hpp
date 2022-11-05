#include <bits/stdc++.h>
#include "Net.hpp"
#include "Cell.hpp"
#include "BucketList.hpp"

class FM {
public:
    FM();
    FM(std::string cellFile, std::string netFile, std::string outFile);

private:
    std::unordered_map<std::string, Net> nets;
    std::unordered_map<std::string, Cell> cells;
    std::vector<Cell*> selectedBaseCells;
    std::vector<int> maxGains;
    BucketList setA, setB;
    Cell* baseCell;
    int maxPinNum = INT_MIN;
    int maxPartialSum;
    int maxPartialSumIndex;
    clock_t startAll, startIO;
    double timeTotal, timeCPU, timeIO;
    double runtime;
    int pass_num;

    void print_cells();
    void print_nets();
    void read_cells(std::string filename);
    void read_nets(std::string filename);
    void read_inputs(std::string cellFile, std::string netFile);
    void write_result(std::string filename);
    void initial_partition();
    void initial_partition_v2();
    bool is_balanced(long sizeA, long sizeB);
    bool select_base_cell();
    bool select_base_cell_v2();
    bool select_base_cell_v3();
    bool select_base_cell_v4();
    void calc_max_partial_sum();
    void update_cells_gain();
    void run_pass();
    void reset_lock();
    void print_sets();
    void pause();
    void print_selected_base_cells();
    void roll_back_from(int index);
    int calc_cut_size();
    void print_time_info();
    void set_runtime();
};
