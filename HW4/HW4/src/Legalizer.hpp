#include <bits/stdc++.h>

#ifndef NODE_HPP
#define NODE_HPP
#include "Node.hpp"
#endif

#ifndef ROW_HPP
#define ROW_HPP
#include "Row.hpp"
#endif

#ifndef SUBROW_HPP
#define SUBROW_HPP
#include "SubRow.hpp"
#endif

class Legalizer {
public:
    // Variables
    int max_displacement;
    int num_nodes;
    int num_cells;
    int num_blockages;
    int num_rows;
    std::vector<Node*> cells;
    std::vector<Node*> blockages;
    std::vector<Row*> rows;

    // Constructors
    Legalizer(std::string input_file, std::string output_file);

    // Functions
    void read_node(std::string node_file);
    void read_pl(std::string pl_file);
    void read_scl(std::string scl_file);
    void read_input(std::string input_file);
    void write_output(std::string output_file);
    void slice_rows();
    void abacus();
    int find_closest_row(Node* cell);
    int find_closest_subrow(Node* cell, Row* row);
    int place_row_trial(Node* cell, Row* row);
    void place_row_final(Node* cell, Row* row, int subrow_idx);
    void cells_alignment();

    // Utils
    void print_cells();
    void print_blockages();
    void print_rows();
    void pause();
};
