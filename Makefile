CXX := g++
CXXFLAGS := -Wall -std=c++14
LDFLAGS :=
sources = LZW Dict LinkedList
objects = $(addsuffix .o,$(objects))
EXEC1 = LZW.x
INPUT_FILE = Romeo-and-Juliet.txt

.PHONY: clean run

all: $(EXEC1)

Dict.o: LZW.cpp Dict.hpp LinkedList.hpp

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.x: %.o
	$(CXX) $(LDFLAGS) -o $@ $^

run: $(EXEC1)
	./$(EXEC1) $(INPUT_FILE)

clean:
	$(RM) *.o *.x
