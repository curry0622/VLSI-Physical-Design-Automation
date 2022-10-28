#include "FM.hpp"

FM::FM(){};

FM::FM(std::string cellFile, std::string netFile) {
    maxPartialSum = 1;
    read_cells(cellFile);
    read_nets(netFile);
    setA.set_bucket_size(maxPinNum);
    setB.set_bucket_size(maxPinNum);
    initial_partition();
    while(maxPartialSum > 0) {
        run_pass();
        pause();
    }
    // std::cout << "Final result: " << std::endl;
    // setA.print('A');
    // setB.print('B');
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

void FM::print_sets() {
    setA.print('A');
    setB.print('B');
}

void FM::print_selected_base_cells() {
    std::cout << "Selected base cells: ";
    for(int i = 0; i < selectedBaseCells.size(); i++) {
        std::cout << selectedBaseCells[i]->name << "[" << maxGains[i] << "] ";
    }
    std::cout << std::endl;
}

void FM::pause() {
    std::cout << "----------------------Press Enter to continue---------------------" << std::endl;
    std::cin.ignore();
}

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
        it->second.calc_gain();
    }

    // Initialize bucket lists
    for(auto it = cells.begin(); it != cells.end(); it++) {
        if(it->second.inSetA) {
            setA.insert_cell(&it->second);
        } else {
            setB.insert_cell(&it->second);
        }
    }
}

bool FM::select_base_cell() {
    int ka = 1, kb = 1;
    bool found = false;
    while(!found && (ka <= setA.cells.size() || kb <= setB.cells.size())) {
        Cell* a, *b;
        if(ka <= setA.cells.size())
            a = setA.get_top_kth_cell(ka);
        else
            a = nullptr;
        if(kb <= setB.cells.size())
            b = setB.get_top_kth_cell(kb);
        else
            b = nullptr;
        if(a)
            std::cout << "Top " << ka << " cell in set A: " << a->name << "[" << a->gain << "]" << std::endl;
        else
            std::cout << "Top " << ka << " cell in set A: NULL" << std::endl;
        if(b)
            std::cout << "Top " << kb << " cell in set B: " << b->name << "[" << b->gain << "]" << std::endl;
        else    
            std::cout << "Top " << kb << " cell in set B: NULL" << std::endl;
        if(a && a->isLocked) {
            a = nullptr;
        }
        if(b && b->isLocked) {
            b = nullptr;
        }
        if(a && b) {
            if(a->gain > b->gain) {
                baseCell = a;
                ka++;
                std::cout << baseCell->name << std::endl;
                found = is_balanced(setA.size - baseCell->sizeA, setB.size + baseCell->sizeB);
            } else {
                baseCell = b;
                kb++;
                std::cout << baseCell->name << std::endl;
                found = is_balanced(setA.size + baseCell->sizeA, setB.size - baseCell->sizeB);
            }
        } else if(a) {
            baseCell = a;
            ka++;
            std::cout << baseCell->name << std::endl;
            found = is_balanced(setA.size - baseCell->sizeA, setB.size + baseCell->sizeB);
        } else if(b) {
            baseCell = b;
            kb++;
            std::cout << baseCell->name << std::endl;
            found = is_balanced(setA.size + baseCell->sizeA, setB.size - baseCell->sizeB);
        } else {
            baseCell = nullptr;
            ka++;
            kb++;
            std::cout << "No base cell found" << std::endl;
        }
    }
    return found;
}

void FM::calc_max_partial_sum() {
    int max = INT_MIN, sum = 0, index = -1;
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
        baseCell->inSetA = false;
        baseCell->calc_gain();
        baseCell->isLocked = true;
        setB.insert_cell(baseCell);
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
        baseCell->inSetA = true;
        baseCell->calc_gain();
        baseCell->isLocked = true;
        setA.insert_cell(baseCell);
        // std::cout << "Move " << baseCell->name << " from B to A" << std::endl;
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

void FM::reset_lock() {
    std::cout << "Reset lock" << std::endl;
    for(auto it = selectedBaseCells.begin(); it != selectedBaseCells.end(); it++) {
        (*it)->isLocked = false;
    }
}

bool FM::run_pass() {
    std::cout << "Running pass" << std::endl;
    while(select_base_cell()) {
        maxGains.push_back(baseCell->gain);
        print_sets();
        selectedBaseCells.push_back(baseCell);
        print_selected_base_cells();
        update_cells_gain();
        pause();
    }
    calc_max_partial_sum();
    std::cout << "maxPartialSum: " << maxPartialSum << ", index: " << maxPartialSumIndex << std::endl;
    reset_lock();
    if(maxPartialSum <= 0) {
        std::cout << "Roll back" << std::endl;
        print_sets();
        for(int i = selectedBaseCells.size() - 1; i >= 0; i--) {
            baseCell = selectedBaseCells[i];
            update_cells_gain();
            print_sets();
            pause();
        }
        return false;
    }
    if(maxPartialSumIndex < selectedBaseCells.size() - 1) {
        std::cout << "Roll back" << std::endl;
        print_sets();
        for(int i = selectedBaseCells.size() - 1; i >= maxPartialSumIndex + 1; i--) {
            baseCell = selectedBaseCells[i];
            update_cells_gain();
            print_sets();
            pause();
        }
    }
    reset_lock();
    std::cout << "Clear base cells" << std::endl;
    maxGains.clear();
    selectedBaseCells.clear();
    print_sets();
    return true;
}
