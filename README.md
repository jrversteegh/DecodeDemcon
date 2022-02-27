Implementation of DecodeDemcon exercise/challenge by Jaap Versteegh, DAM Delft.
Challenge described here:
https://mailing.demcon.com/lp/decode-demcon-linear-cellular-automata

This is a C++ implementation.

Since I wasn't sure whether the evaluation of the cellular automaton task itself
would be very challenging, I decided to revisit
[Spirit X3](https://www.boost.org/doc/libs/develop/libs/spirit/doc/x3/html/index.html)
for parsing the input. A parser implementation like this is obviously overkill for such
a simple input, but it was nice as an exercise.
The automaton implementation itself is in `main.cpp`, while the X3 based parser is
in `parser.h`.

The code should "just work" on any system that has GNU Make and a C++17 capable
C++ compiler installed by running `make`, but I tested only on Linux Mint 20.3.
Let me know if this doesn't work for you, I'll be glad to test/make it work
on something else.
If you don't have boost and/or fmt installed, please clone with submodules:

`git clone --recurse-submodules https://github.com/jrversteegh/DecodeDemcon.git`

Running `make rule_<xxx>` evaluates a specific rule, where `rule_A` and `rule_B` will
work and `make rule_30` is most interesting for obvious reasons, while `make test`
will run a parser test along with some rule outputs.
