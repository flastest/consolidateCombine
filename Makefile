CXX       = g++
CXX_STD   = -std=c++2a
CXX_W     = -Wall -Wextra -Wpedantic -g
CXX_GDB   = -ggdb3 -Og
# install libasan liblsan libubsan
CXX_SAN   = -fsanitize=address,leak,undefined
CXX_NODB  = $(CXX_STD) $(CXX_W) $(CXX_GDB) -pthread -lpthread
CXX_DB    = $(CXX_NODB) -DDEBUG
CXX_SAN   = $(CXX_DB)  $(CXX_DB)
SRC       = wc

.cc.o:
	$(CXX) $(CXX_NODB) -c -O3 $<

all: wc

eitan: 
	g++ -Wall -Wextra -pedantic --std=c++17 -g -pthread mapreduce.cc  wc-eitan.cc -o wc

wc: wc.o mapreduce.o
	$(CXX) $(CXX_NODB) -o $(SRC) $(SRC).o

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
