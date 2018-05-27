
CC_FLAGS := -g -L/usr/local/lib/ -lmetis -std=gnu++0x -O2
CC := g++ 

all: TaxiSearching

TaxiSearching: src/TaxiSearching.cpp src/GPTree.cpp src/GPTree.h
	$(CC) -o $@ $^ $(CC_FLAGS)
