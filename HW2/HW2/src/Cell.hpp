#include <bits/stdc++.h>

class Cell {
public:
    std::string name;
    std::vector<std::string> nets;
    int sizeA;
    int sizeB;

    Cell();
    Cell(std::string n, int sa, int sb);
    void print();
};
