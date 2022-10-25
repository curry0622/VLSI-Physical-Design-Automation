#include "FM.hpp"

FM::FM(){};

FM::FM(std::string cellFile, std::string netFile) {
    read_cells(cellFile);
    read_nets(netFile);
    setA.set_size(maxPinNum);
    setB.set_size(maxPinNum);
}

void FM::read_cells(std::string filename){
    std::ifstream fin(filename);
    std::string line;
    while(std::getline(fin, line)) {
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
    while(std::getline(fin, line)) {
        std::stringstream ss(line);
        std::string name;
        std::vector<Cell*> netCells;
        std::string buf;
        ss >> buf >> name >> buf >> buf;
        while(buf != "}") {
            netCells.push_back(&cells[buf]);
            cells[buf].add_net(&nets[name]);
            if(cells[buf].pinNum > maxPinNum) {
                maxPinNum = cells[buf].pinNum;
            }
            ss >> buf;
        }
        nets[name] = Net(name, netCells);
    }
};

void FM::print_cells(){
    for(auto it = cells.begin(); it != cells.end(); it++)
        it->second.print();
};

void FM::print_nets(){
    for(auto it = nets.begin(); it != nets.end(); it++)
        it->second.print();
};
