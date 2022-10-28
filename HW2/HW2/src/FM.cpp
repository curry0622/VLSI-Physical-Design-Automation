#include "FM.hpp"

FM::FM(){};

FM::FM(std::string cellFile, std::string netFile) {
    maxPartialSum = 1;
    read_cells(cellFile);
    read_nets(netFile);
    setA.set_bucket_size(maxPinNum);
    setB.set_bucket_size(maxPinNum);
    // std::cout << "Bucket set done." << std::endl;
    initial_partition();
    // std::cout << "Initial Partition done. " << std::endl;
    while(maxPartialSum > 0) {
        run_pass();
        pause();
    }
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
    std::cout << "Selected base cells(" << selectedBaseCells.size() << "): ";
    for(int i = 0; i < selectedBaseCells.size(); i++) {
        std::cout << selectedBaseCells[i]->name << "[" << maxGains[i] << "] ";
    }
    std::cout << std::endl;
}

void FM::pause() {
    // std::cout << "----------------------Press Enter to continue---------------------" << std::endl;
    // std::cin.ignore();
}

bool FM::is_balanced(int sizeA, int sizeB) {
    return abs(sizeA - sizeB) < 0.1 * abs(sizeA + sizeB);
}

void FM::initial_partition() {
    // Variables declaration
    int sizeA = 0, sizeB = 0;
    std::vector<Cell> sortedCells;
    // std::cout << "Variables declaration done." << std::endl;

    // Push all cells into a vector
    for(auto it = cells.begin(); it != cells.end(); it++) {
        sizeA += it->second.sizeA;
        sortedCells.push_back(it->second);
    }
    // std::cout << "Push all cells into a vector done." << std::endl;

    // Sort the vector by sizeB (small to large)
    std::sort(sortedCells.begin(), sortedCells.end(), [](Cell a, Cell b) {
        return a.sizeB > b.sizeB;
    });
    // std::cout << "Sort the vector by sizeB (small to large) done." << std::endl;

    // Assign cell with larger sizeB to setB until balanced
    // int it = 1;
    while(!is_balanced(sizeA, sizeB)) {
        // std::cout << it++ << std::endl;
        Cell c = sortedCells.back();
        sortedCells.pop_back();
        cells[c.name].inSetA = false;
        sizeA -= c.sizeA;
        sizeB += c.sizeB;
    }
    // std::cout << "Assign cell with larger sizeB to setB until balanced done." << std::endl;

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
    if(!found)
        std::cout << "No base cell found" << std::endl;
    else
        std::cout << "Base cell found: " << baseCell->name << std::endl;
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
        // setA.remove_cell(baseCell);
        // baseCell->inSetA = false;
        // baseCell->calc_gain();
        baseCell->isLocked = true;
        // setB.insert_cell(baseCell);
        for(auto it = baseCell->nets.begin(); it != baseCell->nets.end(); it++) {
            (*it)->print();
            if((*it)->numInSetB == 0) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if(!(*it2)->isLocked) {
                        (*it2)->add_gain();
                        // std::cout << "Predicted gain of " << (*it2)->name << ": " << (*it2)->gain << std::endl;
                        // (*it2)->calc_gain();
                        // std::cout << "Actual gain of " << (*it2)->name << ": " << (*it2)->gain << std::endl;
                        if((*it2)->inSetA) {
                            setA.update_cell(*it2);
                        } else {
                            setB.update_cell(*it2);
                        }
                    }
                }
            } else if((*it)->numInSetB == 1) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if(!(*it2)->isLocked && !(*it2)->inSetA) {
                        (*it2)->sub_gain();
                        // std::cout << "Predicted gain of " << (*it2)->name << ": " << (*it2)->gain << std::endl;
                        // (*it2)->calc_gain();
                        // std::cout << "Actual gain of " << (*it2)->name << ": " << (*it2)->gain << std::endl;
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
            // std::cout << "Net " << (*it)->name << " now has " << (*it)->numInSetA << " cells in set A and " << (*it)->numInSetB << " cells in set B" << std::endl;
            if((*it)->numInSetA == 0) {
                std::cout << "Net " << (*it)->name << " is now empty in set A" << std::endl;
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    std::cout << "Cell1 " << (*it2)->name << std::endl;
                    if(!(*it2)->isLocked) {
                        (*it2)->sub_gain();
                        // std::cout << "Predicted gain of " << (*it2)->name << ": " << (*it2)->gain << std::endl;
                        // (*it2)->calc_gain();
                        // std::cout << "Actual gain of " << (*it2)->name << ": " << (*it2)->gain << std::endl;
                        if((*it2)->inSetA) {
                            std::cout << "Cell " << (*it2)->name << " is in set A" << std::endl;
                            setA.update_cell(*it2);
                            std::cout << "Cell " << (*it2)->name << " updated in set A" << std::endl;
                        } else {
                            std::cout << "Cell " << (*it2)->name << " is in set B" << std::endl;
                            setB.update_cell(*it2);
                            std::cout << "Cell " << (*it2)->name << " updated in set B" << std::endl;
                        }
                    }
                    std::cout << "Cell2 " << (*it2)->name << std::endl;
                }
            } else if((*it)->numInSetA == 1) {
                std::cout << "Net " << (*it)->name << " now has only one cell in set A" << std::endl;
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if(!(*it2)->isLocked && (*it2)->inSetA) {
                        (*it2)->add_gain();
                        // std::cout << "Predicted gain of " << (*it2)->name << ": " << (*it2)->gain << std::endl;
                        // (*it2)->calc_gain();
                        // std::cout << "Actual gain of " << (*it2)->name << ": " << (*it2)->gain << std::endl;
                        if((*it2)->inSetA) {
                            setA.update_cell(*it2);
                        } else {
                            setB.update_cell(*it2);
                        }
                    }
                }
            }
        }
        setA.remove_cell(baseCell);
        baseCell->inSetA = false;
        baseCell->calc_gain();
        baseCell->isLocked = true;
        setB.insert_cell(baseCell);
    } else {
        // setB.remove_cell(baseCell);
        // baseCell->inSetA = true;
        // baseCell->calc_gain();
        baseCell->isLocked = true;
        // setA.insert_cell(baseCell);
        for(auto it = baseCell->nets.begin(); it != baseCell->nets.end(); it++) {
            // (*it)->print();
            if((*it)->numInSetA == 0) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if(!(*it2)->isLocked) {
                        (*it2)->add_gain();
                        // std::cout << "Predicted gain of " << (*it2)->name << ": " << (*it2)->gain << std::endl;
                        // (*it2)->calc_gain();
                        // std::cout << "Actual gain of " << (*it2)->name << ": " << (*it2)->gain << std::endl;
                        if((*it2)->inSetA) {
                            setA.update_cell(*it2);
                        } else {
                            setB.update_cell(*it2);
                        }
                    }
                }
            } else if((*it)->numInSetA == 1) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if(!(*it2)->isLocked && (*it2)->inSetA) {
                        (*it2)->sub_gain();
                        // std::cout << "Predicted gain of " << (*it2)->name << ": " << (*it2)->gain << std::endl;
                        // (*it2)->calc_gain();
                        // std::cout << "Actual gain of " << (*it2)->name << ": " << (*it2)->gain << std::endl;
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
                        (*it2)->sub_gain();
                        // std::cout << "Predicted gain of " << (*it2)->name << ": " << (*it2)->gain << std::endl;
                        // (*it2)->calc_gain();
                        // std::cout << "Actual gain of " << (*it2)->name << ": " << (*it2)->gain << std::endl;
                        if((*it2)->inSetA) {
                            setA.update_cell(*it2);
                        } else {
                            setB.update_cell(*it2);
                        }
                    }
                }
            } else if((*it)->numInSetB == 1) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if(!(*it2)->isLocked && !(*it2)->inSetA) {
                        (*it2)->add_gain();
                        // std::cout << "Predicted gain of " << (*it2)->name << ": " << (*it2)->gain << std::endl;
                        // (*it2)->calc_gain();
                        // std::cout << "Actual gain of " << (*it2)->name << ": " << (*it2)->gain << std::endl;
                        if((*it2)->inSetA) {
                            setA.update_cell(*it2);
                        } else {
                            setB.update_cell(*it2);
                        }
                    }
                }
            }
        }
        setB.remove_cell(baseCell);
        baseCell->inSetA = true;
        baseCell->calc_gain();
        baseCell->isLocked = true;
        setA.insert_cell(baseCell);
    }
}

void FM::reset_lock() {
    std::cout << "Reset lock" << std::endl;
    for(auto it = selectedBaseCells.begin(); it != selectedBaseCells.end(); it++) {
        (*it)->isLocked = false;
    }
}

void FM::roll_back_from(int index) {
    std::cout << "Roll back from " << index << " to " << selectedBaseCells.size() - 1 <<  std::endl;
    for(int i = selectedBaseCells.size() - 1; i >= index; i--) {
        baseCell = selectedBaseCells[i];
        std::cout << "Roll back base[" << i << "]: " << baseCell->name << std::endl;
        update_cells_gain();
        print_sets();
        pause();
    }
}

bool FM::run_pass() {
    std::cout << "Running pass" << std::endl;
    int it = 1;
    while(select_base_cell()) {
        maxGains.push_back(baseCell->gain);
        print_sets();
        selectedBaseCells.push_back(baseCell);
        print_selected_base_cells();
        update_cells_gain();
        pause();
        std::cout << "===============================Iteration " << it++ << std::endl;
    }
    calc_max_partial_sum();
    std::cout << "maxPartialSum: " << maxPartialSum << ", index: " << maxPartialSumIndex << std::endl;
    reset_lock();
    if(maxPartialSum <= 0) {
        print_sets();
        roll_back_from(0);
        return false;
    }
    if(maxPartialSumIndex < selectedBaseCells.size() - 1) {
        print_sets();
        roll_back_from(maxPartialSumIndex + 1);
    }
    reset_lock();
    std::cout << "Clear base cells" << std::endl;
    maxGains.clear();
    selectedBaseCells.clear();
    print_sets();
    return true;
}
