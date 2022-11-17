#include "Floorplan.hpp"

Floorplan::Floorplan() {
    dead_space_ratio = -1;
    num_hardblocks = -1;
    num_terminals = -1;
}

Floorplan::Floorplan(std::string hardblocks_file, std::string nets_file, std::string pins_file, std::string output, double ratio) {
    // Read inputs
    read_hardblocks(hardblocks_file);
    read_pins(pins_file);
    read_nets(nets_file);
    dead_space_ratio = ratio;

    // Calculate total area
    calc_total_area();

    // Calculate max coord
    calc_max_coord();

    // Initial solution
    std::vector<std::string> sol = init_sol();
    // std::vector<std::string> sol;
    // sol.push_back("sb0");
    // sol.push_back("sb1");
    // sol.push_back("H");
    // sol.push_back("sb2");
    // sol.push_back("H");
    int area = get_area(sol);
    std::cout << "Area of init sol: " << area << std::endl;

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

int Floorplan::get_wirelength() {
    int wirelength = 0;
    for(auto net : nets) {
        wirelength += net.HPWL();
    }
    return wirelength;
}

// int Floorplan::get_area(std::vector<std::string> sol) {
//     // Different from Stockmeyer, use greedy
//     std::stack<std::vector<int>> stk;
//     for(int i = 0; i < sol.size(); i++) {
//         if(sol[i] == "V" || sol[i] == "H") {
//             std::vector<int> b = stk.top(); // b: right or top
//             stk.pop();
//             std::vector<int> a = stk.top(); // a: left or bottom
//             stk.pop();
//             stk.push(get_min_area_comb(a, b, sol[i]));
//         } else {
//             Hardblock hardblock = hardblocks[sol[i]];
//             stk.push({hardblock.width, hardblock.height});
//         }
//     }
//     std::vector<int> result = stk.top();
//     return result[0] * result[1];
// }

int Floorplan::get_area(std::vector<std::string> sol) {
    // Different from Stockmeyer, use greedy
    std::stack<std::vector<std::pair<int, int>>> stk;
    for(int i = 0; i < sol.size(); i++) {
        if(sol[i] == "V" || sol[i] == "H") {
            std::vector<std::pair<int, int>> b = stk.top(); // b: right or top
            stk.pop();
            std::vector<std::pair<int, int>> a = stk.top(); // a: left or bottom
            stk.pop();
            stk.push({get_min_area_comb(a, b, sol[i])});
        } else {
            Hardblock hardblock = hardblocks[sol[i]];
            int width = hardblock.width;
            int height = hardblock.height;
            stk.push({{width, height}, {height, width}});
        }
    }
    std::cout << "stk size: " << stk.size() << std::endl;
    std::pair<int, int> result = stk.top()[0];
    return result.first * result.second;
}

std::vector<std::string> Floorplan::init_sol() {
    std::vector<std::string> sol;
    bool v_1st = true, h_1st = true;
    int curr_width = 0;
    for(auto hardblock : hardblocks) {
        // int width = std::max(hardblock.second.width, hardblock.second.height);
        int width = (hardblock.second.width + hardblock.second.height) / 2;
        if(curr_width + width <= max_coord.x) {
            std::cout << "<=" << std::endl;
            curr_width += width;
            sol.push_back(hardblock.first), std::cout << hardblock.first << std::endl;
            if(v_1st) {
                v_1st = false;
            } else {
                sol.push_back("V"), std::cout << "V" << std::endl;
            }
        } else {
            std::cout << ">" << std::endl;
            curr_width = width;
            if(h_1st) {
                h_1st = false;
                sol.push_back(hardblock.first), std::cout << hardblock.first << std::endl;
            } else {
                sol.push_back("H"), std::cout << "H" << std::endl;
                sol.push_back(hardblock.first), std::cout << hardblock.first << std::endl;
            }
        }
        std::cout << "curr_width: " << curr_width << std::endl;
    }
    sol.push_back("H"), std::cout << "H" << std::endl;
    return sol;
}

// std::vector<int> Floorplan::get_min_area_comb(std::vector<int> a, std::vector<int> b, std::string type) {
//     int width, height, area, min_area = INT_MAX;
//     std::vector<int> w = {0, 0}, h = {0, 0};
//     if(type == "V") {
//         for(int i = 0; i < 2; i++) {
//             for(int j = 0; j < 2; j++) {
//                 width = a[i] + b[j];
//                 height = std::max(a[1 - i], b[1 - j]);
//                 area = width * height;
//                 if(area < min_area) {
//                     min_area = area;
//                     w = {i, j};
//                     h = {1 - i, 1 - j};
//                 }
//             }
//         }
//         return {a[w[0]] + b[w[1]], std::max(a[h[0]], b[h[1]])};
//     } else {
//         for(int i = 0; i < 2; i++) {
//             for(int j = 0; j < 2; j++) {
//                 width = std::max(a[i], b[j]);
//                 height = a[1 - i] + b[1 - j];
//                 area = width * height;
//                 if(area < min_area) {
//                     min_area = area;
//                     w = {i, j};
//                     h = {1 - i, 1 - j};
//                 }
//             }
//         }
//         return {std::max(a[w[0]], b[w[1]]), a[h[0]] + b[h[1]]};
//     }
//     return {};
// }

std::pair<int, int> Floorplan::get_min_area_comb(std::vector<std::pair<int, int>> a, std::vector<std::pair<int, int>> b, std::string type) {
    int width, height, area, min_area = INT_MAX;
    std::pair<int, int> result = {0, 0};
    if(type == "V") {
        std::cout << "V" << std::endl;
        for(int i = 0; i < a.size(); i++) {
            for(int j = 0; j < b.size(); j++) {
                width = a[i].first + b[j].first;
                height = std::max(a[i].second, b[j].second);
                area = width * height;
                std::cout << width << " * " << height << " = " << area << std::endl;
                if(area < min_area) {
                    min_area = area;
                    result = {width, height};
                }
            }
        }
    } else {
        std::cout << "H" << std::endl;
        for(int i = 0; i < a.size(); i++) {
            for(int j = 0; j < b.size(); j++) {
                width = std::max(a[i].first, b[j].first);
                height = a[i].second + b[j].second;
                area = width * height;
                std::cout << width << " * " << height << " = " << area << std::endl;
                if(area < min_area) {
                    min_area = area;
                    result = {width, height};
                }
            }
        }
    }
    std::cout << "final: " << result.first << " * " << result.second << " = " << result.first * result.second << std::endl;
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
