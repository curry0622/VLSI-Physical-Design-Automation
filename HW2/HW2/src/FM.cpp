#include "FM.hpp"

FM::FM(){};

FM::FM(std::string cellFile, std::string netFile, std::string outFile) {
    // Start timer
    startAll = clock();
    std::cout << "FM starts" << std::endl << std::endl;

    // Read inputs
    read_inputs(cellFile, netFile);

    // Set buckets' sizes
    setA.set_bucket_size(maxPinNum);
    setB.set_bucket_size(maxPinNum);

    // Initial partition
    initial_partition_v2();

    // Set runtime
    set_runtime();

    // Run FM
    maxPartialSum = 1;
    while(maxPartialSum > 0) {
        run_pass();
    }

    // Write result
    write_result(outFile);

    // Print time info
    print_time_info();
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

void FM::read_inputs(std::string cellFile, std::string netFile){
    // Start timer
    startIO = clock();

    // Read cells
    read_cells(cellFile);

    // Read nets
    read_nets(netFile);

    // End timer
    timeIO = (double)(clock() - startIO) / CLOCKS_PER_SEC;
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

bool FM::is_balanced(long sizeA, long sizeB) {
    return abs(sizeA - sizeB) < 0.1 * abs(sizeA + sizeB);
}

void FM::initial_partition() {
    // Variables declaration
    long sizeA = 0, sizeB = 0;
    std::vector<Cell> sortedCells;

    // Push all cells into a vector
    for(auto cp : cells) {
        sortedCells.push_back(cp.second);
        sizeA += cp.second.sizeA;
    }

    // Sort the vector by sizeB (large to small)
    std::sort(sortedCells.begin(), sortedCells.end(), [](Cell a, Cell b) { return a.sizeB < b.sizeB; });

    // Assign cell with smaller sizeB to setB until balanced
    while(!is_balanced(sizeA, sizeB)) {
        Cell c = sortedCells.back();
        sortedCells.pop_back();
        cells[c.name].inSetA = false;
        sizeA -= c.sizeA;
        sizeB += c.sizeB;
    }

    // Calculate number of cells in each set in each net
    for(auto& np : nets) {
        np.second.calc_num_in_set();
    }

    // Initialize gain of each cell
    for(auto& cp : cells) {
        cp.second.calc_gain();
    }

    // Initialize bucket lists
    for(auto& cp : cells) {
        if(cp.second.inSetA) {
            setA.insert_cell(&cp.second);
        } else {
            setB.insert_cell(&cp.second);
        }
    }

    std::cout << "INITIAL PARTITION CUT SIZE: " << calc_cut_size() << std::endl << std::endl;
}

void FM::initial_partition_v2() {
    // Variables declaration
    long sizeA = 0, sizeB = 0;
    std::vector<Cell> sortedCells;

    // Push all cells into a vector
    std::cout << "CELL NUM: " << cells.size() << std::endl;
    for(auto cp : cells) {
        sortedCells.push_back(cp.second);
        sizeA += cp.second.sizeA;
    }

    // Sort the vector by sizeB (large to small)
    std::sort(sortedCells.begin(), sortedCells.end(), [](Cell a, Cell b) { return a.sizeB < b.sizeB; });

    // Assign cell with smaller sizeB to setB until balanced
    while(!is_balanced(sizeA, sizeB)) {
        Cell c = sortedCells.back();
        sortedCells.pop_back();
        cells[c.name].inSetA = false;
        sizeA -= c.sizeA;
        sizeB += c.sizeB;
    }
    for(int i = 0; i < cells.size(); i++) {
        Cell c = sortedCells.back();
        sortedCells.pop_back();
        cells[c.name].inSetA = false;
        sizeA -= c.sizeA;
        sizeB += c.sizeB;
        if(!is_balanced(sizeA, sizeB)) {
            cells[c.name].inSetA = true;
            sizeA += c.sizeA;
            sizeB -= c.sizeB;
            break;
        }
    }

    // Calculate number of cells in each set in each net
    for(auto& np : nets) {
        np.second.calc_num_in_set();
    }

    // Initialize gain of each cell
    for(auto& cp : cells) {
        cp.second.calc_gain();
    }

    // Initialize bucket lists
    for(auto& cp : cells) {
        if(cp.second.inSetA) {
            setA.insert_cell(&cp.second);
        } else {
            setB.insert_cell(&cp.second);
        }
    }

    std::cout << "INITIAL PARTITION CUT SIZE: " << calc_cut_size() << std::endl << std::endl;
}

bool FM::select_base_cell() {
    int ka = 1, kb = 1;
    bool found = false;
    while(!found && (ka <= setA.cells.size() || kb <= setB.cells.size())) {
        Cell* a, *b;

        if(ka <= setA.cells.size()) {
            a = setA.get_top_kth_cell(ka);
        } else {
            a = nullptr;
        }
        if(kb <= setB.cells.size()) {
            b = setB.get_top_kth_cell(kb);
        } else {
            b = nullptr;
        }

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

bool FM::select_base_cell_v2() {
    // Reset base cell
    baseCell = nullptr;

    // Variables declaration
    int ka = 1, kb = 1;
    Cell *ca = nullptr, *cb = nullptr;
    bool found = false;

    // Find max gain cell of setA
    while(ka <= setA.cells.size()) {
        ca = setA.get_top_kth_cell(ka);
        if(ca && ca->isLocked)
            ka++;
        else
            break;
    }

    // Find max gain cell of setB
    while(kb <= setB.cells.size()) {
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

    // Return found
    return found;
}

bool FM::select_base_cell_v3() {
    // Reset base cell
    baseCell = nullptr;

    // Variables declaration
    Cell *ca = nullptr, *cb = nullptr;
    int ka = 1, kb = 1;
    bool found = false;
    
    // Find base cell
    while(!found && (ka <= setA.cells.size() || kb <= setB.cells.size())) {
        // Find max gain cell of setA
        while(ka <= setA.cells.size()) {
            ca = setA.get_top_kth_cell(ka);
            if(ca && ca->isLocked)
                ka++;
            else
                break;
        }

        // Find max gain cell of setB
        while(kb <= setB.cells.size()) {
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
                ka++;
                if(!found) {
                    baseCell = cb;
                    found = is_balanced(setA.size + baseCell->sizeA, setB.size - baseCell->sizeB);
                    kb++;
                }
            } else {
                baseCell = cb;
                found = is_balanced(setA.size + baseCell->sizeA, setB.size - baseCell->sizeB);
                kb++;
                if(!found) {
                    baseCell = ca;
                    found = is_balanced(setA.size - baseCell->sizeA, setB.size + baseCell->sizeB);
                    ka++;
                }
            }
        } else if(ca) {
            baseCell = ca;
            found = is_balanced(setA.size - baseCell->sizeA, setB.size + baseCell->sizeB);
            ka++;
        } else if(cb) {
            baseCell = cb;
            found = is_balanced(setA.size + baseCell->sizeA, setB.size - baseCell->sizeB);
            kb++;
        } else {
            ka++;
            kb++;
            baseCell = nullptr;
            found = false;
        }
    }

    // Return found
    return found;
}

bool FM::select_base_cell_v4() {
    // Reset base cell
    baseCell = nullptr;

    // Variables declaration
    Cell *ca = nullptr, *cb = nullptr;
    int ka = 1, kb = 1;
    bool found = false;
    bool fromA = true;

    // Find base cell
    while(!found && (ka <= setA.cells.size() || kb <= setB.cells.size())) {
        if(fromA) {
            ca = setA.get_top_kth_cell(ka++);
            if(ca && !ca->isLocked) {
                baseCell = ca;
                found = is_balanced(setA.size - baseCell->sizeA, setB.size + baseCell->sizeB);
            }
        } else {
            cb = setB.get_top_kth_cell(kb++);
            if(cb && !cb->isLocked) {
                baseCell = cb;
                found = is_balanced(setA.size + baseCell->sizeA, setB.size - baseCell->sizeB);
            }
        }
        fromA = !fromA;
    }

    // Return found
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
    // Variables declaration
    int iteration = 0;
    bool terminate = false;

    // Select base cell and update cells gain
    while(select_base_cell()) {
        iteration++;
        maxGains.push_back(baseCell->gain);
        selectedBaseCells.push_back(baseCell);
        update_cells_gain();
        if(((clock() - startAll) / (double) CLOCKS_PER_SEC) > runtime) {
            terminate = true;
            break;
        }
    }
    std::cout << "Iteration: " << iteration << std::endl;

    // Calculate max partial sum
    calc_max_partial_sum();
    std::cout << "Max partial sum index: " << maxPartialSumIndex << std::endl;
    std::cout << "Base cells size: " << selectedBaseCells.size() << std::endl;

    // Reset lock
    reset_lock();

    // Roll back
    if(maxPartialSum <= 0) {
        roll_back_from(0);
    } else if(maxPartialSumIndex < selectedBaseCells.size() - 1) {
        roll_back_from(maxPartialSumIndex + 1);
    }

    // Clear
    reset_lock();
    maxGains.clear();
    selectedBaseCells.clear();

    // Terminate
    if(terminate)
        maxPartialSum = INT_MIN;

    std::cout << "cut size: " << calc_cut_size() << std::endl;
    std::cout << "[Curr time] " << (clock() - startAll) / (double) CLOCKS_PER_SEC << std::endl << std::endl;
}

int FM::calc_cut_size() {
    int cutSize = 0;
    for(auto np: nets) {
        if(np.second.numInSetA > 0 && np.second.numInSetB > 0) {
            cutSize++;  
        }
    }
    return cutSize;
}

void FM::write_result(std::string filename) {
    // Start timer
    startIO = clock();

    // File operation
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

    // End timer
    timeIO += (double)(clock() - startIO) / CLOCKS_PER_SEC;

    std::cout << std::endl;
    std::cout << "[Cut size] " << cutSize << std::endl;
    std::cout << "[A size] " << setA.cells.size() << std::endl;
    std::cout << "[B size] " << setB.cells.size() << std::endl;
    std::cout << std::endl;
}

void FM::print_time_info() {
    // Calculate time
    timeTotal = (double)(clock() - startAll)/CLOCKS_PER_SEC;
    timeCPU = timeTotal - timeIO;

    // Print time
    std::cout << std::endl;
    std::cout << "[CPU time] " << timeCPU << " secs" << std::endl;
    std::cout << "[IO time] " << timeIO << " secs" << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "[Total time] " << timeTotal << " secs" << std::endl;
    std::cout << std::endl;
}

void FM::set_runtime() {
    // if(cells.size() <= 100000) {
    //     runtime = 2.0;
    // } else if(cells.size() <= 200000) {
    //     runtime = 5.5;
    // } else {
    //     runtime = 18.5;
    // }
    runtime = 285.0;
}
