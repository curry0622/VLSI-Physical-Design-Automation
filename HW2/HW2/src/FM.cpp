#include "FM.hpp"

FM::FM(){};

void FM::read_cells(std::string filename){
    std::cout << "Reading cells from " << filename << std::endl;
};

void FM::read_nets(std::string filename){
    std::cout << "Reading nets from " << filename << std::endl;
};

void FM::write_result(std::string filename){
    std::cout << "Writing result to " << filename << std::endl;
};
