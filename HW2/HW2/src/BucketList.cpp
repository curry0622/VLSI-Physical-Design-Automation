#include "BucketList.hpp"

BucketList::BucketList() {
    buckets = std::vector<std::vector<Cell*>>{};
    cells = std::unordered_map<Cell*, std::pair<int, int>>{};
    maxPinNum = 0;
    maxGain = INT_MIN;
    size = 0;
};

void BucketList::set_bucket_size(int mpn) {
    maxPinNum = mpn;
    buckets.resize(2 * mpn + 1);
}

void BucketList::insert_cell(Cell* cell) {
    // Insert cell to the bucket list and map
    int gain = cell->gain;
    int bIndex = gain_to_index(gain);
    buckets[bIndex].push_back(cell);
    cells[cell] = std::make_pair(bIndex, buckets[bIndex].size() - 1);

    // Update max gain
    if(!cell->isLocked && gain > maxGain)
        maxGain = gain;

    // Update set size
    if(cell->inSetA)
        size += cell->sizeA;
    else
        size += cell->sizeB;
}

void BucketList::remove_cell(Cell* cell) {
    // Remove cell from bucket list and map
    int bIndex = cells[cell].first;
    int cIndex = cells[cell].second;
    Cell* lastCell = buckets[bIndex].back();
    buckets[bIndex][cIndex] = lastCell;
    buckets[bIndex].pop_back();
    cells[lastCell] = std::make_pair(bIndex, cIndex);
    cells.erase(cell);

    // Update max gain
    while(buckets[gain_to_index(maxGain)].empty())
        maxGain--;

    // Update set size
    if(cell->inSetA)
        size -= cell->sizeA;
    else
        size -= cell->sizeB;
}

void BucketList::update_cell(Cell* cell) {
    remove_cell(cell);
    insert_cell(cell);
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
