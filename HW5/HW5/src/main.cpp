#include <bits/stdc++.h>
#include "Data.hpp"

int main(int argc, char *argv[]) {
    assert(argc == 3);

    Die d1("design1", 1, 2, 3, 4);
    std::cout << d1.design_name << std::endl;
    Component c1("lib1", "inst1", 1, 2);
    std::cout << c1.lib_name << std::endl;
    SpecialNet s1("inst1", "layer1", 1, 2, 3, 4);
    std::cout << s1.inst_name << std::endl;

    return 0;
}