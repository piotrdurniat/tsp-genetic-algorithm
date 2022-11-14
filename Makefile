CXX       := g++
CXX_FLAGS := -O3

BIN     := bin
SRC     := src
INCLUDE := include

LIBRARIES   :=
EXECUTABLE  := main


all: $(BIN)/$(EXECUTABLE)

gdb: CXX_FLAGS = -ggdb
gdb: all

valgrind: CXX_FLAGS = -Wall -pedantic -g
valgrind: all

run: clean all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I $(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	rm $(BIN)/*
