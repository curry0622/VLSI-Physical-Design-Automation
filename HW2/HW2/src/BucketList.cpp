#include "BucketList.hpp"

BucketList::BucketList() {
    maxGain = INT_MIN;
};

void BucketList::set_size(int mpn) {
    maxPinNum = mpn;
    buckets.resize(2 * mpn + 1);
}

void BucketList::insert_cell(Cell* cell) {
    int gain = cell->gain;
    int bucketIndex = get_bucket_index(gain);
    buckets[bucketIndex].push_back(cell);
    cells[cell] = std::make_pair(bucketIndex, buckets[bucketIndex].size() - 1);
    if(gain > maxGain) {
        maxGain = gain;
    }
}

void BucketList::remove_cell(Cell* cell) {
    int bucketIndex = cells[cell].first;
    int cellIndex = cells[cell].second;
    buckets[bucketIndex][cellIndex] = buckets[bucketIndex].back();
    buckets[bucketIndex].pop_back();
    cells[buckets[bucketIndex][cellIndex]] = std::make_pair(bucketIndex, cellIndex);
    cells.erase(cell);
}

void BucketList::update_cell(Cell* cell, int gain) {

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

int BucketList::get_size(bool isSetA) {
    int size = 0;
    for(auto it = cells.begin(); it != cells.end(); it++) {
        if(isSetA) {
            size += (*it->first).sizeA;
        } else {
            size += (*it->first).sizeB;
        }
    }
    return size;
}

void BucketList::print(char setName) {
    std::cout << "Set[" << setName << "]\n";
    std::cout << "Max gain: " << maxGain << std::endl;
    for(int i = 0; i < buckets.size(); i++) {
        std::cout << "gain " << i - maxPinNum << ": ";
        for(int j = 0; j < buckets[i].size(); j++) {
            std::cout << buckets[i][j]->name << " ";
        }
        std::cout << std::endl;
    }
}
