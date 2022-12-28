#include <bits/stdc++.h>

class Node {
public:
    // Variables
    std::string name;
    double x, y; // global placement coordinates
    double opt_x, opt_y; // optimal coordinates
    int w, h;
    int weight;

    // Constructors
    Node();
    Node(std::string name);

    // Functions
    void set_xy(double x, double y);
    void set_wh(int w, int h);
    double get_cost();

    // Utils
    void print();
};
