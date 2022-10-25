#include <bits/stdc++.h>

class BucketList {
public:
    std::vector<std::vector<std::string>> buckets;
    std::map<std::string, std::pair<int, int>> cells;
    int maxPinNum;
    int maxGain;

    BucketList();
    BucketList(int mpn);
    void set_size(int mpn);
    void insert_cell(std::string cell, int gain);
    void remove_cell(std::string cell);
    void update_cell(std::string cell, int gain);
    void update_max_gain();
    int get_bucket_index(int gain);
};
