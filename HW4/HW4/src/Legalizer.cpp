#include "Legalizer.hpp"

Legalizer::Legalizer() {
    // TODO
}

Legalizer::Legalizer(std::string input_file, std::string output_file) {
    // Read input
    read_input(input_file);
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
    }

    // Read blockages
    for(int i = 0; i < num_blockages; i++) {
        std::getline(file, line);
        std::string name;
        int w, h;
        ss << line;
        ss >> name >> w >> h >> buffer;
        ss.clear();
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
    }

    // Read blockages
    for(int i = 0; i < num_blockages; i++) {
        std::getline(file, line);
        std::string name;
        double x, y;
        ss << line;
        ss >> name >> x >> y >> buffer >> buffer >> buffer;
        ss.clear();
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
        double coord, h, site_w, num_sites, subrow_origin;
        std::getline(file, line);
        std::getline(file, line);
        ss << line;
        ss >> buffer >> buffer >> coord;
        ss.clear();

        std::getline(file, line);
        ss << line;
        ss >> buffer >> buffer >> h;
        ss.clear();

        std::getline(file, line);
        ss << line;
        ss >> buffer >> buffer >> site_w;
        ss.clear();

        std::getline(file, line);
        ss << line;
        ss >> buffer >> buffer >> num_sites;
        ss.clear();

        std::getline(file, line);
        ss << line;
        ss >> buffer >> buffer >> subrow_origin;
        ss.clear();
        std::getline(file, line);
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
