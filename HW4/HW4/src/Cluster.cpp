#include "Cluster.hpp"

Cluster::Cluster() {
    x = 0;
    q = 0;
    width = 0;
    weight = 0;
    pre = nullptr;
}

Cluster::Cluster(double x, Cluster* pre) {
    this->x = x;
    this->q = 0;
    this->width = 0;
    this->weight = 0;
    this->pre = pre;
}

void Cluster::add_cell(Node* cell) {
    cells.push_back(cell);
    q += cell->weight * (cell->x - width);
    width += cell->width;
    weight += cell->weight;
}

void Cluster::print() {
    std::cout << "[Cluster]" << std::endl;
    std::cout << "X: " << x << std::endl;
    std::cout << "Q: " << q << std::endl;
    std::cout << "Width: " << width << std::endl;
    std::cout << "Weight: " << weight << std::endl;
}
