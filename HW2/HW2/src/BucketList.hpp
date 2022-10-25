#include <bits/stdc++.h>

class BucketList {
public:
    std::vector<std::vector<std::string>> buckets;
    int maxPinNum;
    int maxGain;

    BucketList();
    BucketList(int mpn);
    void insert_cell(std::string cell);
    void remove_cell(std::string cell);
    void update_max_gain();
    int get_bucket_index(int gain);
};
