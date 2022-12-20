#include <bits/stdc++.h>

class Legalizer {
public:
    // Variables
    int max_displacement;
    int num_nodes;
    int num_cells;
    int num_blockages;

    // Constructors
    Legalizer();
    Legalizer(std::string input_file, std::string output_file);

    // Functions
    void read_node(std::string node_file);
    void read_pl(std::string pl_file);
    void read_scl(std::string scl_file);
    void read_input(std::string input_file);
    void write_result(std::string output_file);

    // Utils
};
