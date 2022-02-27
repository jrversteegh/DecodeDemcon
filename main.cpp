/**
 * DecodeDemcon - Cellular Automata
 *
 * @author Jaap Versteegh
 * @email jaap.versteegh@demcon.com
 */

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <bitset>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

#include "parser.h"


// std::vector<bool> is special: may want to try something else..
using Generation = std::vector<bool>;


/**
 * Fetch all input from stdin as a single string stripping new lines
 */
std::string get_input_string() {
    std::string result;
    std::string line;
    while (std::getline(std::cin, line)) {
        result += line;
    }
    return result;
}

/**
 * Create first generation from input
 */
Generation init_generation(const Input& input) {
    // This is guaranteed by the parser, but be pedantic
    if (input.size.width < 1) {
        throw std::out_of_range(
                fmt::format("Invalid generation size: {}", input.size.width));
    }
    Generation result(input.size.width);
    for (int i: input.init.cells) {
        // This is guaranteed by the parser, but be pedantic
        if (i < 1 || i > input.size.width) {
            throw std::out_of_range(
                    fmt::format("Initializer out of range: {}", i));
        }
        // -1 because init index is 1-based (??)
        result[i - 1] = true;
    }
    return result;
}


/**
 * Iterate a view of "Size" bits over a larger container of values
 * that can be cast to "bool"
 */
template <class Container, int Size=3>
struct BitSetIter {
    BitSetIter(Container const& container):
        i_(container.cbegin()), end_(container.cend()), bits_() {
        static_assert(Size / 2 == 1, "Size of BitSet should be odd");
        if (container.size() < Size) {
            throw std::out_of_range("Container too small");
        }
        for (int i = 0; i < Size / 2; ++i) {
            next();
        }
    }
    long next() {
        bits_ <<= 1;
        if (i_ != end_) {
            bits_[0] = *i_++;
        }
        return bits_.to_ulong();
    }
private:
    typename Container::const_iterator i_;
    typename Container::const_iterator end_;
    std::bitset<Size> bits_;
};


/**
 * Get next generation from provided generation and rule
 */
Generation next_generation(Generation& generation,
                           const int rule) {
    Generation result(generation.size());
    BitSetIter parent_bits(generation);
    for (auto&& cell: result) {
        cell = static_cast<bool>((1 << parent_bits.next()) & rule);
    }
    return result;
}


/**
 * Print one generation to stdout
 */
void print_generation(Generation const& generation) {
    for (Generation::value_type b: generation) {
        std::cout << (b ? '*': ' ');
    }
    std::cout << std::endl;
}


/**
 * Run automaton defined by input
 */
void run_automaton(const Input& input) {
    auto generation = init_generation(input);
    for (int i = 0; i < input.size.generations; ++i) {
        print_generation(generation);
        generation = next_generation(generation, input.rule.number);
    }
}


int main() {
    try {
        std::string input_string = get_input_string();
        auto input = parse_input_string(input_string);
        run_automaton(input);
        return 0;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

// vim: autoindent syntax=cpp expandtab tabstop=4 softtabstop=4 shiftwidth=4 filetype=cpp
