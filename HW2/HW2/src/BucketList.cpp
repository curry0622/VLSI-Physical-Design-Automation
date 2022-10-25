#include "BucketList.hpp"

BucketList::BucketList() {};

BucketList::BucketList(int mpn) {
    set_size(mpn);
}

void BucketList::set_size(int mpn) {
    maxPinNum = mpn;
    buckets.resize(2 * mpn + 1);
}

void BucketList::insert_cell(std::string cell, int gain) {
    cells[cell] = std::make_pair(0, 0);
    buckets[get_bucket_index(gain)].push_back(cell);
}

void BucketList::remove_cell(std::string cell) {
    int gain = cells[cell].first;
    int index = cells[cell].second;
    std::vector<std::string> &bucket = buckets[get_bucket_index(gain)];
    bucket.erase(bucket.begin() + index);
    cells.erase(cell);
}

void BucketList::update_cell(std::string cell, int gain) {
    int oldGain = cells[cell].first;
    int index = cells[cell].second;
    std::vector<std::string> &oldBucket = buckets[get_bucket_index(oldGain)];
    oldBucket.erase(oldBucket.begin() + index);
    for(int i = index; i < oldBucket.size(); i++)
        cells[oldBucket[i]].second--;
    buckets[get_bucket_index(gain)].push_back(cell);
    cells[cell] = std::make_pair(gain, buckets[get_bucket_index(gain)].size() - 1);
}

void BucketList::update_max_gain() {
    for(int i = buckets.size() - 1; i >= 0; i--) {
        if(buckets[i].size() > 0) {
            maxGain = i - maxPinNum;
            return;
        }
    }
}

int BucketList::get_bucket_index(int gain) {
    return gain + maxPinNum;
}
