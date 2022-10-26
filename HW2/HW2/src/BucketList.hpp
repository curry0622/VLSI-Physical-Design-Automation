#include <bits/stdc++.h>
#include "Cell.hpp"

class BucketList {
public:
    std::vector<std::vector<Cell*>> buckets;
    std::unordered_map<Cell*, std::pair<int, int>> cells;
    int maxPinNum;
    int maxGain;

    BucketList();
    void set_size(int mpn);
    void insert_cell(Cell* cell);
    void remove_cell(Cell* cell);
    void update_cell(Cell* cell, int gain);
    void update_max_gain();
    int get_bucket_index(int gain);
    int get_size(bool isSetA);
    void print(char setName);
};
