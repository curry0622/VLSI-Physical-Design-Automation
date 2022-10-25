#include "BucketList.hpp"

BucketList::BucketList() {};

BucketList::BucketList(int mpn) {
    maxPinNum = mpn;
    buckets.resize(2 * mpn + 1);
}

void BucketList::update_max_gain() {
    for (int i = buckets.size() - 1; i >= 0; i--) {
        if (buckets[i].size() > 0) {
            maxGain = i - maxPinNum;
            return;
        }
    }
}

int BucketList::get_bucket_index(int gain) {
    return gain + maxPinNum;
}
