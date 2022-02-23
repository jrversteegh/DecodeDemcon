/**
 * DecodeDemcon - Cellular Automata

 * @author Jaap Versteegh
 * @email jaap.versteegh@demcon.com
 */

#include <iostream>
#include <vector>

#include "boost/spirit/home/x3.hpp"

void print_generation(std::vector<bool> const& generation) {
    for (bool b : generation) {
        std::cout << b ? '*': ' ';
    }
    std::cout << std::endl;
}

int main()
{
    std::vector<bool> generation;

    print_generation(generation);
    return 0;
}

