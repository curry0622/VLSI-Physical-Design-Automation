#include "Cluster.hpp"

Cluster::Cluster() {
    x = 0;
    q = 0;
    width = 0;
    weight = 0;
    pre = nullptr;
}

Cluster::Cluster(double x, double q, int width, int weight, Cluster* pre) {
    this->x = x;
    this->q = q;
    this->width = width;
    this->weight = weight;
    this->pre = pre;
}

void Cluster::add_cell(Node* cell) {
    cells.push_back(cell);
    width += cell->w;
    weight += cell->weight;
}
