#ifndef UTILS_HPP
#define UTILS_HPP

#include <bits/stdc++.h>
#include "Object.hpp"

void write_def(
    std::string def_name,
    Die die,
    std::vector<Component> component_list,
    std::vector<SpecialNet> specialnet_list
) {
    std::ofstream fout(def_name);
    fout << "VERSION 5.6 ;\n";
    fout << "DIVIDERCHAR \"/\" ;\n";
    fout << "BUSBITCHARS \"[]\" ;\n";

    fout << "DESIGN " << die.design_name << " ;\n\n";
    fout << "UNITS DISTANCE MICRONS 1000 ;\n\n";
    fout << "PROPERTYDEFINITIONS\n";
    fout << "  COMPONENTPIN text STRING ;\n";
    fout << "END PROPERTYDEFINITIONS\n\n";
    fout << "DIEAREA ( " << die.x1 << " " << die.y1 << " ) ( " << die.x2 << " " << die.y2 << " ) ;\n\n";

    fout << "COMPONENTS " << component_list.size() << " ;\n";
    for (auto component : component_list) {
        fout << "- " << component.inst_name << " " << component.lib_name << "\n";
        fout << "  + PLACED ( " << component.x << " " << component.y << " ) N ;\n";
    }
    fout << "END COMPONENTS\n\n";

    fout << "SPECIALNETS " << specialnet_list.size() << " ;\n";
    for (auto specialnet : specialnet_list) {
        if (specialnet.layer == "ME3") {
            std::string name = specialnet.inst_name;
            int x = (specialnet.x1 + specialnet.x2) / 2;
            int y1 = specialnet.y1;
            int y2 = specialnet.y2;
            fout << "- " << name << "\n";
            fout << "  + ROUTED ME3 440 ( " << x << " " << y1 << " ) ( * " << y2 << " ) ;\n";
        } else {
            std::string name = specialnet.inst_name;
            int x1 = specialnet.x1;
            int x2 = specialnet.x2;
            int y = (specialnet.y1 + specialnet.y2) / 2;
            fout << "- " << name << "\n";
            fout << "  + ROUTED ME4 1000 ( " << x1 << " " << y << " ) ( " << x2 << " * ) ;\n";
        }
    }
    fout << "END SPECIALNETS\n\n";

    fout << "END DESIGN\n";
}

#endif