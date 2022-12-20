#include "Legalizer.hpp"

Legalizer::Legalizer() {
    // TODO
}

Legalizer::Legalizer(std::string input_file, std::string output_file) {
    // Read input
    read_input(input_file);
}

void Legalizer::read_input(std::string input_file) {
    // Variables
    std::ifstream file(input_file);
    std::stringstream ss;
    std::string line, buffer;
    std::string node_file, pl_file, scl_file;
    int max_displacement;

    // Read file
    std::getline(file, line);
    ss << line;
    ss >> buffer >> buffer >> node_file >> pl_file >> scl_file;
    ss.clear();
    std::getline(file, line);
    ss << line;
    ss >> buffer >> buffer >> max_displacement;

    std::cout << "node_file: " << node_file << std::endl;
    std::cout << "pl_file: " << pl_file << std::endl;
    std::cout << "scl_file: " << scl_file << std::endl;
    std::cout << "max_displacement: " << max_displacement << std::endl;
}
