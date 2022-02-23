EXE := decode_demcon_cellular_automata
 
all: $(EXE)

$(EXE): main.cpp
	$(CXX) -o $(EXE) $^
	
rule_A: $(EXE)
	echo "A" | ./$(EXE)

rule_B: $(EXE)
	echo "B" | ./$(EXE)

rule_%: $(EXE)
	echo "R %" | ./$(EXE)

clean:
	rm -rf decode_demcon_cellular_automata
