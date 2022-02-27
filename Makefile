EXE := decode_demcon_cellular_automata
INCLUDES := -Ifmt/include $(patsubst %, -I%, $(wildcard boost/*/include))
CXXFLAGS := -Wall --std=c++17
LDFLAGS := 
TEST_PARSER := test_parser
 
all: examples

$(EXE): main.cpp parser.h
	echo $(INCLUDES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(EXE) $^ $(LDFLAGS)

$(TEST_PARSER): $(TEST_PARSER).cpp parser.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TEST_PARSER) $^ $(LDFLAGS)
	./$(TEST_PARSER)
	
rule_A: $(EXE)
	echo "A 11 10 init_start 6 init_end" | ./$(EXE)

rule_B: $(EXE)
	echo "B 61 20 init_start 20 40 init_end" | ./$(EXE)

rule_U: $(EXE)
	echo "U 0 1 1 1 1 0 0 0 41 20 init_start 21 init_end" | ./$(EXE)

rule_%: $(EXE)
	echo "R $* 41 20 init_start 21 init_end" | ./$(EXE)

test: rule_A rule_B rule_U rule_30 $(TEST_PARSER)

examples: rule_A rule_B

clean:
	rm -rf decode_demcon_cellular_automata parser_test

.PHONY: all run test examples clean 
