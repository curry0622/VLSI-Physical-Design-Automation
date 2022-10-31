#include "FM.hpp"

FM::FM(){};

FM::FM(std::string cellFile, std::string netFile, std::string outFile) {
    start = clock();
    maxPartialSum = 1;
    clock_t t;
    t = clock();
    read_cells(cellFile);
    read_nets(netFile);
    std::cout << "File read: " << (float)(clock() - t)/CLOCKS_PER_SEC << " secs" << std::endl;

    setA.set_bucket_size(maxPinNum);
    setB.set_bucket_size(maxPinNum);

    t = clock();
    initial_partition();
    std::cout << "Initial partition: " << (float)(clock() - t)/CLOCKS_PER_SEC << " secs" << std::endl;

    t = clock();
    while(maxPartialSum > 0) {
        run_pass();
    }
    std::cout << "Running passes: " << (float)(clock() - t)/CLOCKS_PER_SEC << " secs" << std::endl;
    std::cout << "Select base cells: " << selectBaseCellTime << " secs" << std::endl;
    std::cout << "Update cells gain: " << updateCellsGainTime << " secs" << std::endl;

    write_result(outFile);
    std::cout << "[Total time] " << (float)(clock() - start)/CLOCKS_PER_SEC << " secs" << std::endl;
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
    for(auto cell : cells) {
        cell.second.print();
    }
};

void FM::print_nets(){
    for(auto net : nets) {
        net.second.print();
    }
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
    for(auto cell : cells) {
        sizeA += cell.second.sizeA;
        sortedCells.push_back(cell.second);
    }

    // Sort the vector by sizeB (small to large)
    std::sort(sortedCells.begin(), sortedCells.end(), [](Cell a, Cell b) { return a.sizeB > b.sizeB; });

    // Assign cell with larger sizeB to setB until balanced
    // TODO: don't use pop back
    while(!is_balanced(sizeA, sizeB)) {
        Cell c = sortedCells.back();
        sortedCells.pop_back();
        cells[c.name].inSetA = false;
        sizeA -= c.sizeA;
        sizeB += c.sizeB;
    }

    // Calculate number of cells in each set in each net
    for(auto& net : nets) {
        net.second.calc_num_in_set();
    }

    // Initialize gain of each cell
    for(auto& cell : cells) {
        cell.second.calc_gain();
    }

    // Initialize bucket lists
    for(auto& cell : cells) {
        if(cell.second.inSetA) {
            setA.insert_cell(&cell.second);
        } else {
            setB.insert_cell(&cell.second);
        }
    }
}

// TODO: speedup
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

bool FM::select_base_cell_v2(int iteration) {
    clock_t t = clock();
    bool found = false;

    if(iteration < 99999999) {
        // Reset base cell
        baseCell = nullptr;

        // Variables declaration
        int ka = 1, kb = 1;
        Cell *ca = nullptr, *cb = nullptr;
        // bool found = false;

        // Find max gain cell of setA
        while(ka <= setA.size) {
            ca = setA.get_top_kth_cell(ka);
            if(ca && ca->isLocked)
                ka++;
            else
                break;
        }

        // Find max gain cell of setB
        while(kb <= setB.size) {
            cb = setB.get_top_kth_cell(kb);
            if(cb && cb->isLocked)
                kb++;
            else
                break;
        }

        // Determine base cell
        if(ca && cb) {
            if(ca->gain > cb->gain) {
                baseCell = ca;
                found = is_balanced(setA.size - baseCell->sizeA, setB.size + baseCell->sizeB);
                if(!found) {
                    baseCell = cb;
                    found = is_balanced(setA.size + baseCell->sizeA, setB.size - baseCell->sizeB);
                }
            } else {
                baseCell = cb;
                found = is_balanced(setA.size + baseCell->sizeA, setB.size - baseCell->sizeB);
                if(!found) {
                    baseCell = ca;
                    found = is_balanced(setA.size - baseCell->sizeA, setB.size + baseCell->sizeB);
                }
            }
        } else if(ca) {
            baseCell = ca;
            found = is_balanced(setA.size - baseCell->sizeA, setB.size + baseCell->sizeB);
        } else if(cb) {
            baseCell = cb;
            found = is_balanced(setA.size + baseCell->sizeA, setB.size - baseCell->sizeB);
        } else {
            baseCell = nullptr;
            found = false;
        }
    }

    // Return found
    selectBaseCellTime += (clock() - t) / (double) CLOCKS_PER_SEC;
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
    clock_t t = clock();
    if(baseCell->inSetA) {
        baseCell->isLocked = true;
        for(auto it = baseCell->nets.begin(); it != baseCell->nets.end(); it++) {
            if((*it)->numInSetB == 0) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    (*it2)->add_gain();
                    if((*it2)->inSetA) {
                        setA.update_cell(*it2);
                    } else {
                        setB.update_cell(*it2);
                    }
                }
            } else if((*it)->numInSetB == 1) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if(!(*it2)->inSetA) {
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
                    (*it2)->sub_gain();
                    if((*it2)->inSetA) {
                        setA.update_cell(*it2);
                    } else {
                        setB.update_cell(*it2);
                    }
                }
            } else if((*it)->numInSetA == 1) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if((*it2)->inSetA) {
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
                    (*it2)->add_gain();
                    if((*it2)->inSetA) {
                        setA.update_cell(*it2);
                    } else {
                        setB.update_cell(*it2);
                    }
                }
            } else if((*it)->numInSetA == 1) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if((*it2)->inSetA) {
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
                    (*it2)->sub_gain();
                    if((*it2)->inSetA) {
                        setA.update_cell(*it2);
                    } else {
                        setB.update_cell(*it2);
                    }
                }
            } else if((*it)->numInSetB == 1) {
                for(auto it2 = (*it)->cells.begin(); it2 != (*it)->cells.end(); it2++) {
                    if(!(*it2)->inSetA) {
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
    updateCellsGainTime += (clock() - t) / (double) CLOCKS_PER_SEC;
}

void FM::reset_lock() {
    for(auto& cell : selectedBaseCells) {
        cell->isLocked = false;
    }
}

void FM::roll_back_from(int index) {
    for(int i = selectedBaseCells.size() - 1; i >= index; i--) {
        baseCell = selectedBaseCells[i];
        update_cells_gain();
    }
}

void FM::run_pass() {
    int iteration = 1;
    while(select_base_cell_v2(iteration++)) {
        maxGains.push_back(baseCell->gain);
        selectedBaseCells.push_back(baseCell);
        update_cells_gain();
    }
    std::cout << "Iteration: " << iteration << std::endl;
    calc_max_partial_sum();
    reset_lock();
    if(maxPartialSum <= 0) {
        roll_back_from(0);
    } else if(maxPartialSumIndex < selectedBaseCells.size() - 1) {
        roll_back_from(maxPartialSumIndex + 1);
    }
    reset_lock();
    maxGains.clear();
    selectedBaseCells.clear();
    std::cout << "cut size: " << calc_cut_size() << std::endl;
    std::cout << "[Curr time] " << (clock() - start) / (double) CLOCKS_PER_SEC << std::endl;
}

int FM::calc_cut_size() {
    int cutSize = 0;
    for(auto n: nets) {
        if(n.second.numInSetA > 0 && n.second.numInSetB > 0) {
            cutSize++;  
        }
    }
    return cutSize;
}

void FM::write_result(std::string filename) {
    clock_t t = clock();
    std::ofstream fout;
    fout.open(filename);
    int cutSize = calc_cut_size();
    fout << "cut_size " << cutSize << std::endl;
    fout << "A " << setA.cells.size() << std::endl;
    for(auto s: setA.cells) {
        fout << s.first->name << std::endl;
    }
    fout << "B " << setB.cells.size() << std::endl;
    for(auto s: setB.cells) {
        fout << s.first->name << std::endl;
    }
    fout.close();

    std::cout << "Writing result: " << (float)(clock() - t)/CLOCKS_PER_SEC << " secs" << std::endl;
    std::cout << "[Cut size] " << cutSize << std::endl;
}
