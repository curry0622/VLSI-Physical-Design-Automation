#include <bits/stdc++.h>
#include "Net.hpp"
#include "Cell.hpp"
#include "BucketList.hpp"

class FM {
public:
    std::unordered_map<std::string, Net> nets;
    std::unordered_map<std::string, Cell> cells;
    BucketList setA, setB;

    FM();
    FM(std::string cellFile, std::string netFile);
    void print_cells();
    void print_nets();
    void write_result(std::string filename);

private:
    int maxPinNum = INT_MIN;

    void read_cells(std::string filename);
    void read_nets(std::string filename);
};
