#include "Node.hpp"

Node::Node() {
    name = "";
    x = 0;
    y = 0;
    width = 0;
    height = 0;
    weight = 0;
}

Node::Node(std::string name) {
    this->name = name;
    x = 0;
    y = 0;
    width = 0;
    height = 0;
    weight = 0;
}

void Node::set_xy(double x, double y) {
    this->x = x;
    this->y = y;
}

void Node::set_wh(int w, int h) {
    this->width = w;
    this->height = h;
    this->weight = w;
}

double Node::get_cost() {
    if(opt_x == DBL_MAX || opt_y == DBL_MAX)
        return DBL_MAX;
    return std::sqrt(pow(opt_x - x, 2) + pow(opt_y - y, 2));
}

void Node::print() {
    std::cout << "[Node]" << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "X: " << x << std::endl;
    std::cout << "Y: " << y << std::endl;
    std::cout << "W: " << width << std::endl;
    std::cout << "H: " << height << std::endl;
    std::cout << "Weight: " << weight << std::endl;
    std::cout << "Optimal X: " << opt_x << std::endl;
    std::cout << "Optimal Y: " << opt_y << std::endl;
}
