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
        std::vector<std::string> netCells;
        std::string buf;
        ss >> buf >> name >> buf >> buf;
        while(buf != "}") {
            netCells.push_back(buf);
            cells[buf].nets.push_back(name);
            ss >> buf;
        }
        nets[name] = Net(name, netCells);
    }
};

void FM::print_cells(){
    for (auto it = cells.begin(); it != cells.end(); it++){
        std::cout << "CELL: " << it->first << " -> " << it->second.sizeA << " " << it->second.sizeB << ", NETS: ";
        for (auto it2 = it->second.nets.begin(); it2 != it->second.nets.end(); it2++){
            std::cout << *it2 << " ";
        }
        std::cout << std::endl;
    }
};

void FM::print_nets(){
    for (auto it = nets.begin(); it != nets.end(); it++){
        std::cout << "NET: " << it->first << " -> ";
        for (auto it2 = it->second.cells.begin(); it2 != it->second.cells.end(); it2++){
            std::cout << *it2 << " ";
        }
        std::cout << std::endl;
    }
};

void FM::write_result(std::string filename){
    std::cout << "Writing result to " << filename << std::endl;
};
