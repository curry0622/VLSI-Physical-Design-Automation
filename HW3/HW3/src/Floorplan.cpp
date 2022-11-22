#include "Floorplan.hpp"

Floorplan::Floorplan() {
    dead_space_ratio = -1;
    num_hardblocks = -1;
    num_terminals = -1;
}

Floorplan::Floorplan(std::string hardblocks_file, std::string nets_file, std::string pins_file, std::string output, double ratio) {
    // Random seed
    srand(time(NULL));

    // Read inputs
    read_hardblocks(hardblocks_file);
    read_pins(pins_file);
    read_nets(nets_file);
    dead_space_ratio = ratio;

    // Calculate total area
    calc_total_area();

    // Calculate max coord
    calc_max_coord();

    // Simulated annealing
    std::vector<std::string> best_sol = simulated_annealing();
    std::vector<int> res = get_area(best_sol);
    std::cout << res[0] << " * " << res[1] << " = " << res[2] << std::endl;

    // Write output
    write_floorplan(output);

    // print_hardblocks();
    // print_pins();
    // print_nets();
    print();
}

void Floorplan::read_hardblocks(std::string filename) {
    // Variables
    std::ifstream file(filename);
    std::string line, buffer;
    std::stringstream ss;

    // Read num of hardblocks
    std::getline(file, line);
    ss = std::stringstream(line);
    ss >> buffer >> buffer >> num_hardblocks;

    // Read num of terminals
    std::getline(file, line);
    ss = std::stringstream(line);
    ss >> buffer >> buffer >> num_terminals;

    // Read the third line
    std::getline(file, line);
    ss = std::stringstream(line);
    ss >> buffer;

    // Read the hardblocks
    while(std::getline(file, line)) {
        if(line.length() > 0) {
            std::string name;
            Coord coord[4];
            ss = std::stringstream(line);
            ss >> name >> buffer >> buffer;
            for(int i = 0; i < 4; i++) {
                int x, y;
                ss >> buffer;
                buffer.erase(buffer.begin());
                buffer.erase(buffer.end() - 1);
                x = std::stoi(buffer);
                ss >> buffer;
                buffer.erase(buffer.end() - 1);
                y = std::stoi(buffer);
                coord[i] = Coord(x, y);
            }
            hardblocks[name] = Hardblock(name, coord[0], coord[2].x - coord[0].x, coord[2].y - coord[0].y);
        } else {
            break;
        }
    }
}

void Floorplan::read_pins(std::string filename) {
    std::ifstream fin(filename);
    std::string line;
    while(std::getline(fin, line)) {
        std::stringstream ss(line);
        std::string name;
        int x, y;
        ss >> name >> x >> y;
        pins[name] = Pin(name, Coord(x, y));
    }
}

void Floorplan::read_nets(std::string filename) {
    // Variables
    std::ifstream fin(filename);
    std::string line, buffer;
    std::stringstream ss;

    // Read num of nets
    std::getline(fin, line);
    ss = std::stringstream(line);
    ss >> buffer >> buffer >> num_nets;

    // Read the second line
    std::getline(fin, line);
    ss = std::stringstream(line);

    // Read the nets
    while(std::getline(fin, line)) {
        Net net;
        int degree;
        ss = std::stringstream(line);
        ss >> buffer >> buffer >> degree;
        for(int i = 0; i < degree; i++) {
            std::string name;
            std::getline(fin, line);
            ss = std::stringstream(line);
            ss >> name;
            if(pins.find(name) != pins.end()) {
                net.add_pin(&pins[name]);
            } else {
                net.add_hardblock(&hardblocks[name]);
            }
        }
        nets.push_back(net);
    }
}

void Floorplan::write_floorplan(std::string filename) {}

void Floorplan::calc_max_coord() {
    int max = sqrt(total_area * (1 + dead_space_ratio));
    max_coord = Coord(max, max);
}

void Floorplan::calc_total_area() {
    total_area = 0;
    for(auto hardblock : hardblocks) {
        total_area += hardblock.second.width * hardblock.second.height;
    }
}

void Floorplan::update_coord(std::vector<std::vector<Node>>& record, int index, int min_at) {
    record[index] = {record[index][min_at]};

    Node* n = &record[index][0];
    Node* left = &record[n->left_from][n->left_at];
    Node* right = &record[n->right_from][n->right_at];

    if(n->type == "V") {
        left->coord = n->coord;
        right->coord = Coord(n->coord.x + left->width, n->coord.y);
        update_coord(record, n->left_from, n->left_at);
        update_coord(record, n->right_from, n->right_at);
    } else if(n->type == "H") {
        left->coord = n->coord;
        right->coord = Coord(n->coord.x, n->coord.y + left->height);
        update_coord(record, n->left_from, n->left_at);
        update_coord(record, n->right_from, n->right_at);
    } else {
        hardblocks[n->type].coord = n->coord;
        if(n->width != hardblocks[n->type].width) {
            hardblocks[n->type].rotated = true;
        }
    }
}

void Floorplan::swap_operand(std::vector<std::string>& sol) {
    int l = rand() % sol.size();
    int r = l + 1;
    bool flag = false;

    while(sol[l] == "V" || sol[l] == "H") {
        l = rand() % sol.size();
        r = l + 1;
    }
    while(sol[r] == "V" || sol[r] == "H") {
        r++;
        if(r >= sol.size()) {
            r = l - 1;
            flag = true;
            break;
        }
    }
    if(flag) {
        while(sol[r] == "V" || sol[r] == "H") {
            r--;
            if(r < 0) {
                return;
            }
        }
    }

    std::swap(sol[l], sol[r]);
}

void Floorplan::invert_chain(std::vector<std::string>& sol) {
    int index = rand() % sol.size();
    int l = index - 1, r = index + 1;

    while(sol[index] != "V" && sol[index] != "H") {
        index = rand() % sol.size();
        l = index - 1, r = index + 1;
    }
    while(l >= 0 && (sol[l] == "V" || sol[l] == "H")) {
        sol[l] = sol[l] == "V" ? "H" : "V";
        l--;
    }
    while(r < sol.size() && (sol[r] == "V" || sol[r] == "H")) {
        sol[r] = sol[r] == "V" ? "H" : "V";
        r++;
    }
    sol[index] = sol[index] == "V" ? "H" : "V";
}

void Floorplan::swap_operand_operator(std::vector<std::string>& sol) {
    int index = rand() % (sol.size() - 1);
    bool flag = true;

    while(flag) {
        if((sol[index] == "V" || sol[index] == "H") && (sol[index + 1] != "V" && sol[index + 1] != "H")) {
            // (operator, operand) -> (operand, operator)
            // Check: no consecutive operators
            if(index + 2 < sol.size() && sol[index] == sol[index + 2]) {
                index = rand() % (sol.size() - 1);
                continue;
            }
            std::swap(sol[index], sol[index + 1]);
            flag = false;
        } else if((sol[index + 1] == "V" || sol[index + 1] == "H") && (sol[index] != "V" && sol[index] != "H")){
            // (operand, operator) -> (operator, operand)
            // Check: no consecutive operators
            if(index - 1 >= 0 && sol[index + 1] == sol[index - 1]) {
                index = rand() % (sol.size() - 1);
                continue;
            }
            // Check: balloting property
            int opr_num = 0;
            for(int i = 0; i <= index + 1; i++) {
                if(sol[i] == "V" || sol[i] == "H") {
                    opr_num++;
                }
            }
            if(2 * opr_num <= index) {
                std::swap(sol[index], sol[index + 1]);
                flag = false;
            } else {
                index = rand() % (sol.size() - 1);
                continue;
            }
        } else {
            index = rand() % (sol.size() - 1);
            continue;
        }
    }
}

int Floorplan::get_wirelength() {
    int wirelength = 0;
    for(auto net : nets) {
        wirelength += net.HPWL();
    }
    return wirelength;
}

std::vector<int> Floorplan::get_area(std::vector<std::string> sol) {
    // Variables
    std::stack<std::vector<Node>> stk;
    std::vector<std::vector<Node>> record;
    std::vector<Node> result;

    // Stockmeyer
    for(int i = 0; i < sol.size(); i++) {
        if(sol[i] == "V" || sol[i] == "H") {
            std::vector<Node> r = stk.top();
            stk.pop();
            std::vector<Node> l = stk.top();
            stk.pop();
            std::vector<Node> res = stockmeyer(l, r, sol[i], i);
            stk.push(res);
            record.push_back(res);
        } else {
            Hardblock hardblock = hardblocks[sol[i]];
            int width = hardblock.width, height = hardblock.height;
            if(width != height) {
                std::vector<Node> res = {
                    Node(sol[i], i, width, height, -1, -1, -1, -1, Coord(0, 0)),
                    Node(sol[i], i, height, width, -1, -1, -1, -1, Coord(0, 0))
                };
                std::sort(res.begin(), res.end(), [](Node a, Node b) {
                    return a.width < b.width;
                });
                stk.push(res);
                record.push_back(res);
            } else {
                std::vector<Node> res = {
                    Node(sol[i], i, width, height, -1, -1, -1, -1, Coord(0, 0))
                };
                stk.push(res);
                record.push_back(res);
            }
        }
    }

    // Get min area
    int width, height, area, min_width, min_height, min_area = INT_MAX, min_index;
    result = stk.top();
    for(int i = 0; i < result.size(); i++) {
        width = result[i].width;
        height = result[i].height;
        area = width * height;
        if(area < min_area) {
            min_width = width;
            min_height = height;
            min_area = area;
            min_index = i;
        }
    }

    // Update coordinates
    update_coord(record, record.size() - 1, min_index);

    return {min_width, min_height, min_area};
}

int Floorplan::get_cost(std::vector<std::string> sol) {
    const double LAMBDA = 0.5;
    std::vector<int> res = get_area(sol); // {w, h, area}
    int wirelength = get_wirelength();
    int width = res[0], height = res[1];
    if(width < max_coord.x)
        width = max_coord.x;
    if(height < max_coord.y)
        height = max_coord.y;
    int cost = width * height + LAMBDA * wirelength;
    return cost;
}

std::vector<std::string> Floorplan::init_sol() {
    std::vector<std::string> sol;
    bool v_1st = true, h_1st = true;
    int curr_width = 0;
    double TOLERANCE = 1.3;

    for(auto hardblock : hardblocks) {
        int width = (hardblock.second.width + hardblock.second.height) / 2;
        if(curr_width + width <= max_coord.x * TOLERANCE) {
            curr_width += width;
            sol.push_back(hardblock.first);
            if(v_1st) {
                v_1st = false;
            } else {
                sol.push_back("V");
            }
        } else {
            curr_width = width;
            if(h_1st) {
                h_1st = false;
                sol.push_back(hardblock.first);
            } else {
                sol.push_back("H");
                sol.push_back(hardblock.first);
            }
        }
    }
    sol.push_back("H");

    // For n6
    // sol = {"sb1", "sb2", "H", "sb3", "sb4", "V", "sb5", "sb6", "V", "H", "V"};

    return sol;
}

std::vector<std::string> Floorplan::gen_neighbor(std::vector<std::string> sol) {
    std::vector<std::string> neighbor = sol;

    int r = rand() % 3;

    switch(r) {
        case 0:
            swap_operand(neighbor);
            break;
        case 1:
            invert_chain(neighbor);
            break;
        case 2:
            swap_operand_operator(neighbor);
            break;
    }

    return neighbor;
}

std::vector<std::string> Floorplan::simulated_annealing() {
    // Initial solution
    std::vector<std::string> sol = init_sol();
    std::vector<std::string> best_sol = sol;

    // Parameters
    double T = 1000.0, T_MIN = 1.0, T_DECAY = 0.95;
    double REJECT_RATIO = 0.95;
    int K = 5;
    int N = num_hardblocks * K;

    // Variables
    int cost = get_cost(sol), min_cost = cost;
    int gen_cnt = 1, uphill_cnt = 0, reject_cnt = 0;

    // Simulated annealing
    while((double)reject_cnt / gen_cnt < REJECT_RATIO && T > T_MIN) {
        // Initialize
        gen_cnt = 0, uphill_cnt = 0, reject_cnt = 0;
        int rand_acc_cnt = 0;

        // Generate neighbor
        while(uphill_cnt <= N && gen_cnt <= 2 * N) {
            std::vector<std::string> neighbor = gen_neighbor(sol);
            int neighbor_cost = get_cost(neighbor);
            gen_cnt++;

            bool rand_accept = (double)rand() / RAND_MAX < exp(-1 * (neighbor_cost - cost) / T);
            if(rand_accept) rand_acc_cnt++;
            if(neighbor_cost <= cost || rand_accept) {
                if(neighbor_cost > cost) {
                    uphill_cnt++;
                }
                sol = neighbor;
                cost = neighbor_cost;
                if(cost < min_cost) {
                    min_cost = cost;
                    best_sol = sol;
                    std::cout << "update best solution: " << std::endl;
                    std::vector<int> res = get_area(best_sol);
                    std::cout << res[0] << " * " << res[1] << " = " << res[2] << std::endl;
                }
            } else {
                reject_cnt++;
            }
        }

        // Reduce temperature
        T *= T_DECAY;
    }

    return best_sol;
}

std::vector<Node> Floorplan::stockmeyer(
    std::vector<Node> l,
    std::vector<Node> r,
    std::string type,
    int index
) {
    std::vector<Node> result; // vector of (width, height)

    if(type == "V") { // width = w_l + w_r, height = max(h_l, h_r)
        int i = 0, j = 0;
        while(i < l.size() && j < r.size()) {
            Node n = Node(
                "V",
                index,
                l[i].width + r[j].width, std::max(l[i].height, r[j].height),
                l[i].index, i,
                r[j].index, j,
                Coord(0, 0)
            );
            result.push_back(n);
            if(l[i].height > r[j].height) {
                i++;
            } else if(l[i].height < r[j].height) {
                j++;
            } else {
                i++, j++;
            }
        }
    } else { // width = max(w_l, w_r), height = h_l + h_r
        int i = l.size() - 1, j = r.size() - 1;
        while(i >= 0 && j >= 0) {
            Node n = Node(
                "H",
                index,
                std::max(l[i].width, r[j].width), l[i].height + r[j].height,
                l[i].index, i,
                r[j].index, j,
                Coord(0, 0)
            );
            result.push_back(n);
            if(l[i].width > r[j].width) {
                i--;
            } else if(l[i].width < r[j].width) {
                j--;
            } else {
                i--, j--;
            }
        }
    }

    // Sort result by width (small to large)
    std::sort(result.begin(), result.end(), [](Node a, Node b) {
        return a.width < b.width;
    });

    return result;
}

void Floorplan::print() {
    std::cout << "---" << std::endl;
    std::cout << "Floorplan dead_space_ratio: " << dead_space_ratio << std::endl;
    std::cout << "Floorplan num_hardblocks: " << num_hardblocks << std::endl;
    std::cout << "Floorplan num_terminals: " << num_terminals << std::endl;
    std::cout << "Floorplan num_nets: " << num_nets << std::endl;
    std::cout << "Floorplan wirelength: " << get_wirelength() << std::endl;
    std::cout << "Floorplan total_area: " << total_area << std::endl;
    std::cout << "Floorplan max_coord: (" << max_coord.x  << ", " << max_coord.y << ")" << std::endl;
    std::cout << "---" << std::endl;
}

void Floorplan::print_hardblocks() {
    for(auto hardblock : hardblocks) {
        hardblock.second.print();
    }
}

void Floorplan::print_pins() {
    for(auto pin : pins) {
        pin.second.print();
    }
}

void Floorplan::print_nets() {
    for(auto net : nets) {
        net.print();
    }
}
