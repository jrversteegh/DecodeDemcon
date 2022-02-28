#include "parser.h"
#include <iostream>
#include <boost/core/lightweight_test.hpp>


int main() {
    Input input;

    input = parse_input_string("A 11 10 init_start 6 init_end");
    BOOST_TEST_EQ(input.rule.number, 122);
    BOOST_TEST_EQ(input.size.width, 11);
    BOOST_TEST_EQ(input.size.generations, 10);
    BOOST_TEST_EQ(input.init.cells.size(), 1);
    BOOST_TEST_EQ(input.init.cells[0], 6);

    input = parse_input_string("B 61 20 init_start 20 40 init_end");
    BOOST_TEST_EQ(input.rule.number, 86);
    BOOST_TEST_EQ(input.size.width, 61);
    BOOST_TEST_EQ(input.size.generations, 20);
    BOOST_TEST_EQ(input.init.cells.size(), 2);
    BOOST_TEST_EQ(input.init.cells[0], 20);
    BOOST_TEST_EQ(input.init.cells[1], 40);

    input = parse_input_string("U 0 1 1 1 1 0 0 0 81 64 init_start 40 init_end");
    BOOST_TEST_EQ(input.rule.number, 30);
    BOOST_TEST_EQ(input.size.width, 81);
    BOOST_TEST_EQ(input.size.generations, 64);
    BOOST_TEST_EQ(input.init.cells.size(), 1);
    BOOST_TEST_EQ(input.init.cells[0], 40);

    input = parse_input_string("R 30 81 64 init_start 30  60 init_end");
    BOOST_TEST_EQ(input.rule.number, 30);
    BOOST_TEST_EQ(input.size.width, 81);
    BOOST_TEST_EQ(input.size.generations, 64);
    BOOST_TEST_EQ(input.init.cells.size(), 2);
    BOOST_TEST_EQ(input.init.cells[0], 30);
    BOOST_TEST_EQ(input.init.cells[1], 60);

    return boost::report_errors();
}

// vim: autoindent syntax=cpp expandtab tabstop=4 softtabstop=4 shiftwidth=4 filetype=cpp
