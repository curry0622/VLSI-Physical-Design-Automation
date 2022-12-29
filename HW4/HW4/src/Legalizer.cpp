#include "Legalizer.hpp"

Legalizer::Legalizer(std::string input_file, std::string output_file) {
    // Read input
    read_input(input_file);

    // Slice rows
    slice_rows();

    // Legalize by Abacus
    abacus();

    // Write output
    write_output(output_file);

    // Check overlap
    // check_overlap();

    // Check alignment
    

    // Print
    // print_cells();
    // print_blockages();
    // print_rows();
}

void Legalizer::read_node(std::string node_file) {
    // Variables
    std::ifstream file(node_file);
    std::stringstream ss;
    std::string line, buffer;

    // Read info
    std::getline(file, line);
    ss << line;
    ss >> buffer >> buffer >> num_nodes;
    ss.clear();

    std::getline(file, line);
    ss << line;
    ss >> buffer >> buffer >> num_blockages;
    ss.clear();

    // Read cells
    num_cells = num_nodes - num_blockages;
    std::getline(file, line);
    for(int i = 0; i < num_cells; i++) {
        std::getline(file, line);
        std::string name;
        int w, h;
        ss << line;
        ss >> name >> w >> h;
        ss.clear();
        cells.push_back(new Node(name));
        cells[i]->set_wh(w, h);
    }

    // Read blockages
    for(int i = 0; i < num_blockages; i++) {
        std::getline(file, line);
        std::string name;
        int w, h;
        ss << line;
        ss >> name >> w >> h >> buffer;
        ss.clear();
        blockages.push_back(new Node(name));
        blockages[i]->set_wh(w, h);
    }
}

void Legalizer::read_pl(std::string node_file) {
    // Variables
    std::ifstream file(node_file);
    std::stringstream ss;
    std::string line, buffer;

    // Read cells
    for(int i = 0; i < num_cells; i++) {
        std::getline(file, line);
        std::string name;
        double x, y;
        ss << line;
        ss >> name >> x >> y >> buffer >> buffer;
        ss.clear();
        cells[i]->set_xy(x, y);
    }

    // Read blockages
    for(int i = 0; i < num_blockages; i++) {
        std::getline(file, line);
        std::string name;
        double x, y;
        ss << line;
        ss >> name >> x >> y >> buffer >> buffer >> buffer;
        ss.clear();
        blockages[i]->set_xy(x, y);
    }
}

void Legalizer::read_scl(std::string node_file) {
    // Variables
    std::ifstream file(node_file);
    std::stringstream ss;
    std::string line, buffer;

    // Read info
    std::getline(file, line);
    ss << line;
    ss >> buffer >> buffer >> num_rows;
    ss.clear();

    // Read rows
    std::getline(file, line);
    for(int i = 0; i < num_rows; i++) {
        // double y, h, site_w, num_sites, x;
        int y, h, site_w, num_sites, x;

        // Coordinate
        std::getline(file, line);
        std::getline(file, line);
        ss << line;
        ss >> buffer >> buffer >> y;
        ss.clear();

        // Height
        std::getline(file, line);
        ss << line;
        ss >> buffer >> buffer >> h;
        ss.clear();

        // Site width
        std::getline(file, line);
        ss << line;
        ss >> buffer >> buffer >> site_w;
        ss.clear();

        // Number of sites
        std::getline(file, line);
        ss << line;
        ss >> buffer >> buffer >> num_sites;
        ss.clear();

        // Subrow Origin
        std::getline(file, line);
        ss << line;
        ss >> buffer >> buffer >> x;
        ss.clear();
        std::getline(file, line);

        // Add row and initialize its row section
        rows.push_back(new Row(x, y, h, num_sites, site_w));
        rows[i]->subrows.push_back(new SubRow(x, x + num_sites * site_w));
    }
}

void Legalizer::read_input(std::string input_file) {
    // Parse file path
    std::string path = input_file.substr(0, input_file.find_last_of("/\\") + 1);
    std::cout << "path: " << path << std::endl;

    // Variables
    std::ifstream file(input_file);
    std::stringstream ss;
    std::string line, buffer;
    std::string node_file, pl_file, scl_file;

    // Read file
    std::getline(file, line);
    ss << line;
    ss >> buffer >> buffer >> node_file >> pl_file >> scl_file;
    ss.clear();

    std::getline(file, line);
    ss << line;
    ss >> buffer >> buffer >> max_displacement;
    ss.clear();

    read_node(path + node_file);
    read_pl(path + pl_file);
    read_scl(path + scl_file);
}

void Legalizer::write_output(std::string output_file) {
    // Variables
    std::ofstream file(output_file);

    // Write cells
    for(int i = 0; i < num_cells; i++) {
        file << cells[i]->name << " " << static_cast<int>(cells[i]->opt_x) << " " << static_cast<int>(cells[i]->opt_y) << std::endl;
    }

    // Write blockages
    for(int i = 0; i < num_blockages; i++) {
        file << blockages[i]->name << " " << static_cast<int>(blockages[i]->x) << " " << static_cast<int>(blockages[i]->y) << std::endl;
    }
}

void Legalizer::slice_rows() {
    // Sort blockages by x (ascending)
    std::sort(blockages.begin(), blockages.end(), [](Node* a, Node* b) {
        return a->x < b->x;
    });

    // For each blockage, if it occupies a row, slice the row
    for(auto& blockage : blockages) {
        for(auto& row : rows) {
            if(row->y >= blockage->y && row->y < blockage->y + blockage->height) {
                row->slice(blockage);
            }
        }
    }
}

void Legalizer::abacus() {
    // Sort cells by x (ascending)
    std::sort(cells.begin(), cells.end(), [](Node* a, Node* b) {
        return a->x < b->x;
    });

    // Iterate through the cells
    for(auto& cell : cells) {
        int opt_row_idx = find_closest_row(cell);
        int opt_subrow_idx = place_row_trial(cell, rows[opt_row_idx]);
        double opt_cost = cell->get_cost();
        int up_row_idx = opt_row_idx, down_row_idx = opt_row_idx;

        while(down_row_idx > 0 && std::abs(cell->y - rows[down_row_idx]->y) < opt_cost) {
            down_row_idx--;
            int subrow_idx = place_row_trial(cell, rows[down_row_idx]);
            double cost = cell->get_cost();
            if(cost < opt_cost) {
                opt_cost = cost;
                opt_row_idx = down_row_idx;
                opt_subrow_idx = subrow_idx;
            }
        }

        while(up_row_idx < num_rows - 1 && std::abs(cell->y - rows[up_row_idx]->y) < opt_cost) {
            up_row_idx++;
            int subrow_idx = place_row_trial(cell, rows[up_row_idx]);
            double cost = cell->get_cost();
            if(cost < opt_cost) {
                opt_cost = cost;
                opt_row_idx = up_row_idx;
                opt_subrow_idx = subrow_idx;
            }
        }

        place_row_final(cell, rows[opt_row_idx], opt_subrow_idx);
    }
    cells_alignment();
}

int Legalizer::find_closest_row(Node* cell) {
    // Initialize min_dist to the y distance between the cell and the first row
    double min_dist = abs(cell->y - rows[0]->y);

    // Iterate through the remaining rows and find the closest one
    for(int i = 1; i < num_rows; i++) {
        double curr_dist = abs(cell->y - rows[i]->y);
        if(min_dist > curr_dist) {
            min_dist = curr_dist;
        } else {
            return i - 1;
        }
    }
    return num_rows - 1;
}

int Legalizer::find_closest_subrow(Node* cell, Row* row) {
    // If it has no subrows, return -1
    if(row->subrows.empty()) {
        return -1;
    }

    if(cell->x >= row->subrows.back()->max_x) {
        if(cell->width <= row->subrows.back()->free_width)
            return row->subrows.size() - 1;
    } else {
        for(int i = 0; i < row->subrows.size(); i++) {
            if(cell->x >= row->subrows[i]->max_x) {
                continue;
            }

            if(cell->x >= row->subrows[i]->min_x) {
                if(cell->width <= row->subrows[i]->free_width)
                    return i;
                if(i + 1 < row->subrows.size()) {
                    if(cell->width <= row->subrows[i + 1]->free_width)
                        return i + 1;
                }
            } else {
                if(i > 0) {
                    if(std::abs(cell->x + cell->width - row->subrows[i-1]->max_x) < std::abs(cell->x - row->subrows[i]->min_x)) {
                        if(cell->width <= row->subrows[i-1]->free_width)
                            return i - 1;
                    }
                    if(cell->width <= row->subrows[i]->free_width)
                        return i;
                } else {
                    if(cell->width <= row->subrows[i]->free_width)
                        return 0;
                }
            }
        }
    }

    return -1;
}

int Legalizer::place_row_trial(Node* cell, Row* row) {
    int subrow_idx = find_closest_subrow(cell, row);
    if(subrow_idx == -1) {
        cell->opt_x = DBL_MAX;
        cell->opt_y = DBL_MAX;
        return -1;
    }

    SubRow* subrow = row->subrows[subrow_idx];
    double opt_x = cell->x;
    if(cell->x < subrow->min_x) {
        opt_x = subrow->min_x;
    } else if(cell->x > subrow->max_x - cell->width) {
        opt_x = subrow->max_x - cell->width;
    }

    Cluster* last_cluster = subrow->last_cluster;
    if(last_cluster == nullptr || last_cluster->x + last_cluster->width <= opt_x) {
        cell->opt_x = opt_x;
    } else {
        int trial_weight = last_cluster->weight + cell->weight;
        int trial_width = last_cluster->width + cell->width;
        double trial_q = last_cluster->q + cell->weight * (opt_x - last_cluster->width);

        double trial_x = 0;
        while(true) {
            trial_x = trial_q / trial_weight;

            if(trial_x < subrow->min_x) {
                trial_x = subrow->min_x;
            }
            if(trial_x > subrow->max_x - trial_width) {
                trial_x = subrow->max_x - trial_width;
            }

            Cluster* pre_cluster = last_cluster->pre;
            if(pre_cluster != nullptr && pre_cluster->x + pre_cluster->width > trial_x) {
                trial_q = pre_cluster->q + trial_q - trial_weight * pre_cluster->width;
                trial_weight += pre_cluster->weight;
                trial_width = pre_cluster->width + trial_width;
                last_cluster = pre_cluster;
            } else {
                break;
            }
        }
        cell->opt_x = trial_x + trial_width - cell->width;
    }
    cell->opt_y = row->y;

    return subrow_idx;
}

void Legalizer::place_row_final(Node* cell, Row* row, int subrow_idx) {
    SubRow* subrow = row->subrows[subrow_idx];
    subrow->free_width -= cell->width;

    double opt_x = cell->x;
    if(cell->x < subrow->min_x) {
        opt_x = subrow->min_x;
    } else if(cell->x > subrow->max_x - cell->width) {
        opt_x = subrow->max_x - cell->width;
    }

    Cluster* last_cluster = subrow->last_cluster;
    if(last_cluster == nullptr || last_cluster->x + last_cluster->width <= opt_x) {
        last_cluster = new Cluster(opt_x, cell->weight * opt_x, cell->width, cell->weight, last_cluster);
        last_cluster->cells.push_back(cell);
        subrow->last_cluster = last_cluster;
    } else {
        last_cluster->cells.push_back(cell);
        last_cluster->q += cell->weight * (opt_x - last_cluster->width);
        last_cluster->width += cell->width;
        last_cluster->weight += cell->weight;

        while(true) {
            last_cluster->x = last_cluster->q / last_cluster->weight;

            if(last_cluster->x < subrow->min_x) {
                last_cluster->x = subrow->min_x;
            }
            if(last_cluster->x > subrow->max_x - last_cluster->width) {
                last_cluster->x = subrow->max_x - last_cluster->width;
            }

            Cluster* pre_cluster = last_cluster->pre;
            if(pre_cluster != nullptr && pre_cluster->x + pre_cluster->width > last_cluster->x) {
                pre_cluster->cells.insert(pre_cluster->cells.end(), last_cluster->cells.begin(), last_cluster->cells.end());
                pre_cluster->weight += last_cluster->weight;
                pre_cluster->q += last_cluster->q - last_cluster->weight * pre_cluster->width;
                pre_cluster->width += last_cluster->width;
                delete last_cluster;
                last_cluster = pre_cluster;
            } else {
                break;
            }
        }
        subrow->last_cluster = last_cluster;
    }
}

void Legalizer::cells_alignment() {
    for(auto& row : rows) {
        int site_width = row->site_width;
        for(auto& subrow : row->subrows) {
            Cluster* last_cluster = subrow->last_cluster;
            while(last_cluster != nullptr) {
                double shift_x = last_cluster->x - subrow->min_x;
                if(shift_x - std::floor(shift_x / site_width) * site_width <= site_width / 2.0) {
                    last_cluster->x = std::floor(shift_x / site_width) * site_width + subrow->min_x;
                } else {
                    last_cluster->x = std::ceil(shift_x / site_width) * site_width + subrow->min_x;
                }

                int opt_x = last_cluster->x;
                for(auto& cell : last_cluster->cells) {
                    cell->opt_x = opt_x;
                    cell->opt_y = row->y;
                    opt_x += cell->width;
                }

                last_cluster = last_cluster->pre;
            }
        }
    }
}

void Legalizer::print_cells() {
    for(int i = 0; i < num_cells; i++) {
        cells[i]->print();
    }
}

void Legalizer::print_blockages() {
    for(int i = 0; i < num_blockages; i++) {
        blockages[i]->print();
    }
}

void Legalizer::print_rows() {
    for(int i = 0; i < num_rows; i++) {
        rows[i]->print();
    }
}

void Legalizer::pause() {
    std::cin.ignore();
}
