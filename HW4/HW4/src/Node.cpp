#include "Node.hpp"

Node::Node() {
    name = "";
    x = 0;
    y = 0;
    w = 0;
    h = 0;
}

Node::Node(std::string name) {
    this->name = name;
    x = 0;
    y = 0;
    w = 0;
    h = 0;
}

void Node::set_xy(double x, double y) {
    this->x = x;
    this->y = y;
}

void Node::set_wh(int w, int h) {
    this->w = w;
    this->h = h;
}

void Node::print() {
    std::cout << "[Node]" << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "X: " << x << std::endl;
    std::cout << "Y: " << y << std::endl;
    std::cout << "W: " << w << std::endl;
    std::cout << "H: " << h << std::endl;
}
