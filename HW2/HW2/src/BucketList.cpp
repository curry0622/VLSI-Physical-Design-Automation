#include "BucketList.hpp"

BucketList::BucketList() {
    maxGain = INT_MIN;
    size = 0;
};

void BucketList::set_bucket_size(int mpn) {
    maxPinNum = mpn;
    buckets.resize(2 * mpn + 1);
}

void BucketList::insert_cell(Cell* cell) {
    int gain = cell->gain;
    int bucketIndex = gain_to_index(gain);
    buckets[bucketIndex].push_back(cell);
    cells[cell] = std::make_pair(bucketIndex, buckets[bucketIndex].size() - 1);
    if(gain > maxGain)
        maxGain = gain;
    if(cell->inSetA)
        size += cell->sizeA;
    else
        size += cell->sizeB;
}

void BucketList::remove_cell(Cell* cell) {
    int bucketIndex = cells[cell].first;
    int cellIndex = cells[cell].second;
    Cell* last = buckets[bucketIndex].back();
    buckets[bucketIndex][cellIndex] = last;
    buckets[bucketIndex].pop_back();
    cells[last] = std::make_pair(bucketIndex, cellIndex);
    cells.erase(cell);
    while(buckets[gain_to_index(maxGain)].empty())
        maxGain--;
    // if(cell->inSetA)
    //     size -= cell->sizeA;
    // else
    //     size -= cell->sizeB;
}

int BucketList::gain_to_index(int gain) {
    return gain + maxPinNum;
}

void BucketList::print(char setName) {
    std::cout << std::endl;
    std::cout << "Set[" << setName << "]" << std::endl;
    std::cout << "size: " << size << std::endl;
    std::cout << "Max gain: " << maxGain << std::endl;
    for(int i = 0; i < buckets.size(); i++) {
        std::cout << "gain " << i - maxPinNum << ": ";
        for(int j = 0; j < buckets[i].size(); j++) {
            std::cout << buckets[i][j]->name << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

Cell* BucketList::get_top_kth_cell(int k) {
    int gain = maxGain;
    int bIndex = gain_to_index(gain);
    int bSize = buckets[bIndex].size();
    while(k >= 1 && gain >= -maxPinNum) {
        if(bSize >= k) {
            return buckets[bIndex][bSize - k];
        } else {
            k -= bSize;
            gain--;
            bIndex = gain_to_index(gain);
            bSize = buckets[bIndex].size();
        }
    }
    return nullptr;
}
