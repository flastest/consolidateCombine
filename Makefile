CXX       = g++
CXX_STD   = -std=c++2a
CXX_W     = -Wall -Wextra -Wpedantic -g
CXX_GDB   = -ggdb3 -O0
# install libasan liblsan libubsan
CXX_SAN   = -fsanitize=address,leak,undefined
CXX_NODB  = $(CXX_STD) $(CXX_W) $(CXX_GDB) -pthread -lpthread
CXX_DB    = $(CXX_NODB) -DDEBUG
CXX_SAN   = $(CXX_DB)  $(CXX_DB)
SRC       = wc
CAP	  = wc-capture

.cc.o:
	$(CXX) $(CXX_NODB) -c -O3 $<

all: wc

eitan: 
	g++ -Wall -Wextra -pedantic --std=c++17 -g -pthread mapreduce.cc  wc-eitan.cc -o wc

wc: wc.o mapreduce.o
	$(CXX) $(CXX_NODB) -o $(SRC) $(SRC).o

wc-capture: wc-capture.o mapreduce.o
	$(CXX) $(CXX_NODB) -o $(CAP) $(CAP).o

mf: mf.o mapreduce.o
	$(CXX) $(CXX_NODB) -o $@ $<

mf-capture: mf-capture.o mapreduce.o # TO DOOOOOOOO
	$(CXX) $(CXX_NODB) -o mf-capture mf-capture.o

debug: 
	$(CXX) -o $(SRC) $(SRC).cc $(CXX_DB)

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(SRC) lotr.txt

helgrind: all
	valgrind --tool=helgrind ./$(SRC) lotr.txt

%.o: %.cc %.hh
	$(CXX) $(CXX_STD) $(CXX_NODB) -c -o $@ $<

clean:
	rm -fv *.o wc mf wc-capture
