CXX = g++
CXXFLAGS = -Wall -Werror -pthread -O

all: mapreduce
pzip: mapreduce.cc
	$(CXX) -std=c++2a -o mapreduce mapreduce.cc $(CXXFLAGS)
clean: 
	rm mapreduce
