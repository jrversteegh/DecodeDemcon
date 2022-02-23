Implementation of DecodeDemcon exercise/challenge by Jaap Versteegh, DAM Delft.
Challenge described here:
https://mailing.demcon.com/lp/decode-demcon-linear-cellular-automata

This is a C++ implementation.

Since I wasn't sure whether the evaluation of the cellular automaton task itself
would be very challenging, I decided to revisit "spirit" for parsing the input,
which was nice.

The code should "just work" on any system that has GNU Make and a C++17 capable
C++ compiler installed by running `make`, but I tested only on Linux Mint 20.3.
Let me know if this doesn't work for you: I'll be glad to test/make it work
on something else.

Running `make rule_xxx` evaluates a specific rule, where rule_A and rule_B will
work and `make rule_30` is most interesting for obvious reasons.
