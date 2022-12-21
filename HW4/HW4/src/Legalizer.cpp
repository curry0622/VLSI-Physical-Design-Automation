#include "Legalizer.hpp"

Legalizer::Legalizer(std::string input_file, std::string output_file) {
    // Read input
    read_input(input_file);

    // Slice rows
    slice_rows();

    // Legalize by Abacus
    abacus();

    // Write output
    // write_output(output_file);

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
        double y, h, site_w, num_sites, x;

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
        rows[i]->row_sections.push_back(new SubRow(x, num_sites * site_w));
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
        file << cells[i]->name << " " << cells[i]->x << " " << cells[i]->y << std::endl;
    }

    // Write blockages
    for(int i = 0; i < num_blockages; i++) {
        file << blockages[i]->name << " " << blockages[i]->x << " " << blockages[i]->y << std::endl;
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
            if(row->y >= blockage->y && row->y < blockage->y + blockage->h) {
                row->slice_row(blockage);
            }
        }
    }
}

void Legalizer::abacus() {
    // Sort cells by x (ascending)
    std::sort(cells.begin(), cells.end(), [](Node* a, Node* b) {
        return a->x < b->x;
    });

    // For each cell, find the best position
    for(auto& cell : cells) {
        int row_idx = find_closest_row(cell);
        
    }
}

int Legalizer::find_closest_row(Node* cell) {
    double min_dist = DBL_MAX;
    for(int i = 0; i < num_rows; i++) {
        double new_dist = abs(cell->y - rows[i]->y);
        if(min_dist > new_dist) {
            min_dist = new_dist;
        } else {
            return i;
        }
    }
    return num_rows - 1;
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
