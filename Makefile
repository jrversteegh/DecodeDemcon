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

parser_test: $(TEST_PARSER)
	@echo
	./$(TEST_PARSER)
	
rule_A: $(EXE)
	@echo
	echo "A 11 10 init_start 6 init_end" | ./$(EXE)

rule_B: $(EXE)
	@echo
	echo "B 61 20 init_start 20 40 init_end" | ./$(EXE)

rule_U: $(EXE)
	@echo
	echo "U 0 1 1 1 1 0 0 0 41 20 init_start 21 init_end" | ./$(EXE)

rule_%: $(EXE)
	@echo
	echo "R $* 41 20 init_start 21 init_end" | ./$(EXE)

invalid_rule:
	@echo
	@echo "Invalid rule input:"
	echo "Z 11 10 init_start 6 init_end" | ./$(EXE) || :

invalid_size:
	@echo
	@echo "Invalid size input:"
	echo "A 11 A init_start 6 init_end" | ./$(EXE) || :
	@echo "Invalid size input:"
	echo "A 11 -1 init_start 6 init_end" | ./$(EXE) || :

missing_init:
	@echo
	@echo "Missing init input:"
	echo "A 11 10" | ./$(EXE) || :

invalid_init_start:
	@echo
	@echo "Invalid init start:"
	echo "A 11 10 init_starX 6 init_end" | ./$(EXE) || :

invalid_init_end:
	@echo
	@echo "Invalid init end:"
	echo "A 11 10 init_start 6 init_enX" | ./$(EXE) || : 

invalid_r_rule:
	@echo
	@echo "Invalid R rule:"
	echo "R 256 11 10 init_start 6 init_end" | ./$(EXE) || : 

invalid_input: invalid_rule invalid_size missing_init invalid_init_start invalid_init_end invalid_r_rule

examples: rule_A rule_B

test: examples rule_U rule_30 parser_test invalid_input

clean:
	rm -rf decode_demcon_cellular_automata parser_test

.PHONY: all run test examples clean 
.PHONY: invalid_rule invalid_size missing_init invalid_init1 invalid_init2
