CXX       = g++
CXX_STD   = -std=c++17
CXX_W     = -Wall -Wextra -Wpedantic
CXX_GDB   = -ggdb3 -Og
# install libasan liblsan libubsan
CXX_SAN   = -fsanitize=address,leak,undefined
CXX_NODB  = $(CXX_STD) $(CXX_W) $(CXX_GDB) -lpthread -pthread
CXX_DB    = $(CXX_NODB) -DDEBUG
CXX_SAN   = $(CXX_DB)  $(CXX_DB)
SRC       = pzip
EXE       = wzip
TESTS     = tests/1.in tests/4.in tests/5.in

.cc.o:
	$(CXX) $(CXX_NODB) -c -O3 $<

all: pzip

pzip: pzip.o
	$(CXX) $(CXX_STD) $(CXX_NODB) -o $(EXE) $(SRC).o

debug: 
	$(CXX) -o $(EXE) $(SRC).cc $(CXX_DB)

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXE)

%.o: %.cc %.hh
	$(CXX) $(CXX_STD) $(CXX_NODB) -c -o $@ $<

clean:
	rm -fv *.o $(EXE) $(SRC)