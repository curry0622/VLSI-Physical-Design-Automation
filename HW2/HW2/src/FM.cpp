#include "FM.hpp"

FM::FM(){};

FM::FM(std::string cellFile, std::string netFile, std::string outFile) {
    maxPartialSum = 1;
    read_cells(cellFile);
    read_nets(netFile);
    setA.set_bucket_size(maxPinNum);
    setB.set_bucket_size(maxPinNum);
    initial_partition();
    while(maxPartialSum > 0) {
        run_pass();
    }
    write_result(outFile);
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
    // boost::sort::pdqsort(sortedCells.begin(), sortedCells.end(), [](const Cell& a, const Cell& b) { return a.sizeB > b.sizeB; });
    std::sort(sortedCells.begin(), sortedCells.end(), [](Cell a, Cell b) { return a.sizeB > b.sizeB; });

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
                found = is_balanced(setA.size - baseCell->sizeA, setB.size + baseCell->sizeB);
            } else {
                baseCell = b;
                kb++;
                found = is_balanced(setA.size + baseCell->sizeA, setB.size - baseCell->sizeB);
            }
        } else if(a) {
            baseCell = a;
            ka++;
            found = is_balanced(setA.size - baseCell->sizeA, setB.size + baseCell->sizeB);
        } else if(b) {
            baseCell = b;
            kb++;
            found = is_balanced(setA.size + baseCell->sizeA, setB.size - baseCell->sizeB);
        } else {
            baseCell = nullptr;
            ka++;
            kb++;
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
        baseCell->isLocked = true;
        for(auto it = baseCell->nets.begin(); it != baseCell->nets.end(); it++) {
            if((*it)->numInSetB == 0) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if(true) {
                        (*it2)->add_gain();
                        if((*it2)->inSetA) {
                            setA.update_cell(*it2);
                        } else {
                            setB.update_cell(*it2);
                        }
                    }
                }
            } else if((*it)->numInSetB == 1) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if(true && !(*it2)->inSetA) {
                        (*it2)->sub_gain();
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
                    if(true) {
                        (*it2)->sub_gain();
                        if((*it2)->inSetA) {
                            setA.update_cell(*it2);
                        } else {
                            setB.update_cell(*it2);
                        }
                    }
                }
            } else if((*it)->numInSetA == 1) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if(true && (*it2)->inSetA) {
                        (*it2)->add_gain();
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
        baseCell->isLocked = true;
        for(auto it = baseCell->nets.begin(); it != baseCell->nets.end(); it++) {
            if((*it)->numInSetA == 0) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if(true) {
                        (*it2)->add_gain();
                        if((*it2)->inSetA) {
                            setA.update_cell(*it2);
                        } else {
                            setB.update_cell(*it2);
                        }
                    }
                }
            } else if((*it)->numInSetA == 1) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if(true && (*it2)->inSetA) {
                        (*it2)->sub_gain();
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
                    if(true) {
                        (*it2)->sub_gain();
                        if((*it2)->inSetA) {
                            setA.update_cell(*it2);
                        } else {
                            setB.update_cell(*it2);
                        }
                    }
                }
            } else if((*it)->numInSetB == 1) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if(true && !(*it2)->inSetA) {
                        (*it2)->add_gain();
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
    for(auto it = selectedBaseCells.begin(); it != selectedBaseCells.end(); it++) {
        (*it)->isLocked = false;
    }
}

void FM::roll_back_from(int index) {
    for(int i = selectedBaseCells.size() - 1; i >= index; i--) {
        baseCell = selectedBaseCells[i];
        update_cells_gain();
    }
}

bool FM::run_pass() {
    while(select_base_cell()) {
        maxGains.push_back(baseCell->gain);
        selectedBaseCells.push_back(baseCell);
        update_cells_gain();
    }
    calc_max_partial_sum();
    reset_lock();
    if(maxPartialSum <= 0) {
        roll_back_from(0);
        return false;
    }
    if(maxPartialSumIndex < selectedBaseCells.size() - 1) {
        roll_back_from(maxPartialSumIndex + 1);
    }
    reset_lock();
    maxGains.clear();
    selectedBaseCells.clear();
    return true;
}

void FM::write_result(std::string filename) {
    std::ofstream fout;
    fout.open(filename);
    int cutSize = 0;
    for(auto n: nets) {
        if(n.second.numInSetA > 0 && n.second.numInSetB > 0) {
            cutSize++;
        }
    }
    fout << "cut_size " << cutSize << std::endl;
    std::cout << "cut_size " << cutSize << std::endl;
    fout << "A " << setA.cells.size() << std::endl;
    std::cout << "A " << setA.cells.size() << std::endl;
    for(auto s: setA.cells) {
        fout << s.first->name << std::endl;
    }
    fout << "B " << setB.cells.size() << std::endl;
    std::cout << "B " << setB.cells.size() << std::endl;
    for(auto s: setB.cells) {
        fout << s.first->name << std::endl;
    }
}
