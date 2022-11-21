#include "Node.hpp"

Node::Node() {
    type = "default";
    index = -1;
    width = -1, height = -1;
    left_from = -1, left_at = -1;
    right_from = -1, right_at = -1;
    coord = Coord(0, 0);
}

Node::Node(std::string type, int index, int width, int height, int left_from, int left_at, int right_from, int right_at, Coord coord) {
    this->type = type;
    this->index = index;
    this->width = width, this->height = height;
    this->left_from = left_from, this->left_at = left_at;
    this->right_from = right_from, this->right_at = right_at;
    this->coord = coord;
}

void Node::print() {
    std::cout << "---" << std::endl;
    std::cout << "Node type: " << type << std::endl;
    std::cout << "Node index: " << index << std::endl;
    std::cout << "Node width: " << width << std::endl;
    std::cout << "Node height: " << height << std::endl;
    std::cout << "Node left_from: " << left_from << std::endl;
    std::cout << "Node left_at: " << left_at << std::endl;
    std::cout << "Node right_from: " << right_from << std::endl;
    std::cout << "Node right_at: " << right_at << std::endl;
    std::cout << "Node coord: ", coord.print(), std::cout << std::endl;
    std::cout << "---" << std::endl;
}
