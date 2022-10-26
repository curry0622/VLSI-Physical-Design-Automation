#include <bits/stdc++.h>
#include "Cell.hpp"

class BucketList {
public:
    std::vector<std::vector<Cell*>> buckets;
    std::unordered_map<Cell*, std::pair<int, int>> cells;
    int maxPinNum;
    int maxGain;
    int size;

    BucketList();
    void set_bucket_size(int mpn);
    void insert_cell(Cell* cell);
    void remove_cell(Cell* cell);
    int gain_to_index(int gain);
    void print(char setName);
    Cell* get_top_kth_cell(int k);
};
