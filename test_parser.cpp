#include "parser.h"
#include <iostream>


int main() {
  Input input = parse_input_string("A 11 10 init_start 6 init_end");
  input = parse_input_string("B 61 20 init_start 20 40 init_end");
  input = parse_input_string("U 0 1 1 1 1 0 0 0 81 64 init_start 40 init_end");
  input = parse_input_string("R 30 81 64 init_start 40 init_end");
  return 0;
}

// vim: autoindent syntax=cpp expandtab tabstop=4 softtabstop=4 shiftwidth=4 filetype=cpp
