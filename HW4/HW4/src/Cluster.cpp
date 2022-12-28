#include "Cluster.hpp"

Cluster::Cluster() {
    x = 0;
}

Cluster::Cluster(double x) {
    this->x = x;
}

int Cluster::get_width() {
    int width = 0;
    for(const auto &cell : cells)
        width += cell->w;
    return width;
}
