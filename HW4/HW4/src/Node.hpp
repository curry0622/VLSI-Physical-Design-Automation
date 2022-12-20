#include <bits/stdc++.h>

class Node {
public:
    // Variables
    std::string name;
    double x, y;
    int w, h;

    // Constructors
    Node();
    Node(std::string name);

    // Functions
    void set_xy(double x, double y);
    void set_wh(int w, int h);

    // Utils
    void print();
};
