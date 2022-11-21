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
    int area = get_area(sol);
    std::cout << "Area of init sol: " << area << std::endl;

    // std::vector<std::vector<int>> l, r, result;
    // l.push_back({2, 3});
    // l.push_back({3, 2});
    // r.push_back({2, 2});
    // r.push_back({3, 2});
    // result = stockmeyer(l, r, "H");
    // for(auto i : result) {
    //     for(auto j : i) {
    //         std::cout << j << " ";
    //     }
    //     std::cout << std::endl;
    // }

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

int Floorplan::get_area(std::vector<std::string> sol) {
    std::stack<std::vector<Node>> stk;
    std::vector<Node> result;

    for(int i = 0; i < sol.size(); i++) {
        if(sol[i] == "V" || sol[i] == "H") {
            std::vector<Node> r = stk.top();
            stk.pop();
            std::vector<Node> l = stk.top();
            stk.pop();
            std::vector<Node> res = stockmeyer(l, r, sol[i], i);
            stk.push(res);
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
            } else {
                std::vector<Node> res = {
                    Node(sol[i], i, width, height, -1, -1, -1, -1, Coord(0, 0))
                };
                stk.push(res);
            }
        }
    }

    int width, height, area, min_width, min_height, min_area = INT_MAX, idx;
    result = stk.top();
    for(int i = 0; i < result.size(); i++) {
        width = result[i].width;
        height = result[i].height;
        area = width * height;
        if(area < min_area) {
            min_width = width;
            min_height = height;
            min_area = area;
            idx = i;
        }
    }
    std::cout << min_width << " * " << min_height << " = " << min_area << std::endl;
    result[idx].print();

    return min_area;
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

    return sol;
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
