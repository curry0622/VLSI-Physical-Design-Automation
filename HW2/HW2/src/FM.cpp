#include "FM.hpp"

FM::FM(){};

void FM::read_cells(std::string filename){
    std::ifstream fin(filename);
    std::string line;
    while (std::getline(fin, line)){
        std::stringstream ss(line);
        std::string name;
        int sizeA;
        int sizeB;
        ss >> name >> sizeA >> sizeB;
        cells[name] = Cell(name, sizeA, sizeB);
    }
};

void FM::read_nets(std::string filename){
    std::ifstream fin(filename);
    std::string line;
    while (std::getline(fin, line)){
        std::stringstream ss(line);
        std::string name;
        std::vector<Cell> netCells;
        std::string buf;
        ss >> buf >> name >> buf >> buf;
        while(buf != "}") {
            netCells.push_back(cells[buf]);
            ss >> buf;
        }
        Net net(name, netCells);
        nets.push_back(net);
    }
};

void FM::print_cells(){
    for (auto it = cells.begin(); it != cells.end(); it++){
        std::cout << "CELL: " << it->first << " -> " << it->second.sizeA << " " << it->second.sizeB << std::endl;
    }
};

void FM::print_nets(){
    for (auto it = nets.begin(); it != nets.end(); it++){
        std::cout << "NET: " << it->name << " -> ";
        for (auto it2 = it->cells.begin(); it2 != it->cells.end(); it2++){
            std::cout << it2->name << " ";
        }
        std::cout << std::endl;
    }
};

void FM::write_result(std::string filename){
    std::cout << "Writing result to " << filename << std::endl;
};
