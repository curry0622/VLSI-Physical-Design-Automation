#include "Cluster.hpp"

Cluster::Cluster() {
    x = 0;
    width = 0;
    weight = 0;
}

Cluster::Cluster(double x) {
    this->x = x;
    width = 0;
    weight = 0;
}

void Cluster::add_cell(Node* cell) {
    cells.push_back(cell);
    width += cell->w;
    weight += cell->weight;
}
