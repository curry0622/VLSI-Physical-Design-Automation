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

bool FM::is_balanced(int sizeA, int sizeB) {
    return abs(sizeA - sizeB) < 0.1 * abs(sizeA + sizeB);
}

int FM::get_set_size(bool isSetA) {
    int size = 0;
    for(auto it = cells.begin(); it != cells.end(); it++) {
        if(it->second.inSetA == isSetA)
            size++;
    }
    return size;
}

void FM::initial_partition() {
    // Variables declaration
    int sizeA = 0, sizeB = 0;
    std::vector<Cell> sortedCells;

    // Push all cells into a vector
    for(auto it = cells.begin(); it != cells.end(); it++) {
        sizeA += it->second.sizeA;
        sortedCells.push_back(it->second);
    }

    // Sort the vector by sizeB (small to large)
    std::sort(sortedCells.begin(), sortedCells.end(), [](Cell a, Cell b) {
        return a.sizeB > b.sizeB;
    });

    // Assign cell with larger sizeB to setB until balanced
    while(!is_balanced(sizeA, sizeB)) {
        Cell c = sortedCells.back();
        sortedCells.pop_back();
        cells[c.name].inSetA = false;
        sizeA -= c.sizeA;
        sizeB += c.sizeB;
    }
    for(auto it = cells.begin(); it != cells.end(); it++) {
        if(it->second.inSetA)
            std::cout << "A: ";
        else
            std::cout << "B: ";
        it->second.print();
    }
}
