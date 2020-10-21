CXX       = g++
CXX_STD   = -std=c++2a
CXX_W     = -Wall -Wextra -Wpedantic
CXX_GDB   = -ggdb3 -Og
# install libasan liblsan libubsan
CXX_SAN   = -fsanitize=address,leak,undefined
CXX_NODB  = $(CXX_STD) $(CXX_W) $(CXX_GDB) -pthread
CXX_DB    = $(CXX_NODB) -DDEBUG
CXX_SAN   = $(CXX_DB)  $(CXX_DB)
SRC       = mapreduce

.cc.o:
	$(CXX) $(CXX_NODB) -c -O3 $<

all: pzip

pzip: pzip.o
	$(CXX) $(CXX_STD) $(CXX_NODB) -o $(SRC) $(SRC).o

debug: 
	$(CXX) -o $(SRC) $(SRC).cc $(CXX_DB)

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(SRC)

helgrind: all
	valgrind --tool=helgrind ./$(SRC)

%.o: %.cc %.hh
	$(CXX) $(CXX_STD) $(CXX_NODB) -c -o $@ $<

clean:
	rm -fv *.o $(SRC) $(SRC)