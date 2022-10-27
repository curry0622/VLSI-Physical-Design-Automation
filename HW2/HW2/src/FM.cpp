#include "FM.hpp"

FM::FM(){};

FM::FM(std::string cellFile, std::string netFile) {
    read_cells(cellFile);
    read_nets(netFile);
    setA.set_bucket_size(maxPinNum);
    setB.set_bucket_size(maxPinNum);
    initial_partition();
    setA.print('A');
    setB.print('B');
    select_base_cell();
    std::cout << "Base cell: " << baseCell->name << std::endl;
    update_cells_gain();
    setA.print('A');
    setB.print('B');
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

    // Calculate number of cells in each set in each net
    for(auto it = nets.begin(); it != nets.end(); it++) {
        it->second.calc_num_in_set();
    }

    // Initialize gain of each cell
    for(auto it = cells.begin(); it != cells.end(); it++) {
        it->second.init_gain();
    }

    // Initialize bucket lists
    for(auto it = cells.begin(); it != cells.end(); it++) {
        if(it->second.inSetA) {
            setA.insert_cell(&it->second);
        } else {
            setB.insert_cell(&it->second);
        }
    }

    // setA.print('A');
    // setB.print('B');
}

void FM::select_base_cell() {
    int ka = 1, kb = 1;
    bool found = false;
    while(!found) {
        Cell* a = setA.get_top_kth_cell(ka);
        Cell* b = setB.get_top_kth_cell(kb);
        if(a && b && !a->isLocked && !b->isLocked) {
            if(a->gain > b->gain) {
                baseCell = a;
                ka++;
                found = is_balanced(setA.size - baseCell->sizeA, setB.size + baseCell->sizeA);
            } else {
                baseCell = b;
                kb++;
                found = is_balanced(setA.size + baseCell->sizeB, setB.size - baseCell->sizeB);
            }
        } else if(a && !a->isLocked) {
            baseCell = a;
            ka++;
            found = is_balanced(setA.size - baseCell->sizeA, setB.size + baseCell->sizeA);
        } else if(b && !b->isLocked) {
            baseCell = b;
            kb++;
            found = is_balanced(setA.size + baseCell->sizeB, setB.size - baseCell->sizeB);
        } else {
            std::cout << "No base cell found." << std::endl;
            baseCell = nullptr;
            break;
        }
    }
}

void FM::calc_max_partial_sum() {
    int max = 0, sum = 0, index = 0;
    for(int i = 0; i < maxGains.size(); i++) {
        sum += maxGains[i];
        if(sum > max) {
            max = sum;
            index = i;
        }
    }
    maxPartialSum = max;
    maxPartialSumIndex = index;
}

void FM::update_cells_gain() {
    if(baseCell->inSetA) {
        setA.remove_cell(baseCell);
        setB.insert_cell(baseCell);
        baseCell->inSetA = false;
        baseCell->isLocked = true;
        for(auto it = baseCell->nets.begin(); it != baseCell->nets.end(); it++) {
            if((*it)->numInSetB == 0) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if(!(*it2)->isLocked) {
                        (*it2)->gain++;
                        if((*it2)->inSetA) {
                            setA.update_cell(*it2);
                        } else {
                            setB.update_cell(*it2);
                        }
                    }
                }
            } else if((*it)->numInSetB == 1) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if(!(*it2)->isLocked) {
                        (*it2)->gain--;
                        if((*it2)->inSetA) {
                            setA.update_cell(*it2);
                        } else {
                            setB.update_cell(*it2);
                        }
                    }
                }
            }
            (*it)->numInSetA--;
            (*it)->numInSetB++;
            if((*it)->numInSetA == 0) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if(!(*it2)->isLocked) {
                        (*it2)->gain--;
                        if((*it2)->inSetA) {
                            setA.update_cell(*it2);
                        } else {
                            setB.update_cell(*it2);
                        }
                    }
                }
            } else if((*it)->numInSetA == 1) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if(!(*it2)->isLocked) {
                        (*it2)->gain++;
                        if((*it2)->inSetA) {
                            setA.update_cell(*it2);
                        } else {
                            setB.update_cell(*it2);
                        }
                    }
                }
            }
        }
    } else {
        setB.remove_cell(baseCell);
        setA.insert_cell(baseCell);
        baseCell->inSetA = true;
        baseCell->isLocked = true;
        for(auto it = baseCell->nets.begin(); it != baseCell->nets.end(); it++) {
            if((*it)->numInSetA == 0) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if(!(*it2)->isLocked) {
                        (*it2)->gain++;
                        if((*it2)->inSetA) {
                            setA.update_cell(*it2);
                        } else {
                            setB.update_cell(*it2);
                        }
                    }
                }
            } else if((*it)->numInSetA == 1) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if(!(*it2)->isLocked) {
                        (*it2)->gain--;
                        if((*it2)->inSetA) {
                            setA.update_cell(*it2);
                        } else {
                            setB.update_cell(*it2);
                        }
                    }
                }
            }
            (*it)->numInSetB--;
            (*it)->numInSetA++;
            if((*it)->numInSetB == 0) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if(!(*it2)->isLocked) {
                        (*it2)->gain--;
                        if((*it2)->inSetA) {
                            setA.update_cell(*it2);
                        } else {
                            setB.update_cell(*it2);
                        }
                    }
                }
            } else if((*it)->numInSetB == 1) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if(!(*it2)->isLocked) {
                        (*it2)->gain++;
                        if((*it2)->inSetA) {
                            setA.update_cell(*it2);
                        } else {
                            setB.update_cell(*it2);
                        }
                    }
                }
            }
        }
    }
}
