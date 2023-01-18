#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <bits/stdc++.h>

class Die {
public:
    // Variables
    std::string design_name;
    int x1, y1, x2, y2;

    // Constructors
    Die() {};
    Die(std::string design_name, int x1, int y1, int x2, int y2)
     : design_name(design_name), x1(x1), y1(y1), x2(x2), y2(y2) {};
};

class Component {
public:
    // Variables
    std::string lib_name, inst_name;
    int x, y;

    // Constructors
    Component() {};
    Component(std::string lib_name, std::string inst_name, int x, int y)
     : lib_name(lib_name), inst_name(inst_name), x(x), y(y) {};
};

class SpecialNet {
public:
    // Variables
    std::string inst_name, layer;
    int x1, y1, x2, y2;

    // Constructors
    SpecialNet() {};
    SpecialNet(std::string inst_name, std::string layer, int x1, int y1, int x2, int y2)
     : inst_name(inst_name), layer(layer), x1(x1), y1(y1), x2(x2), y2(y2) {};
};

#endif