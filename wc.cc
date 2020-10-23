#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>     //for reading from file
#include <fstream>      //also reading from a file
#include "mapreduce.cc"

void Map(std::string file_name) {
    std::ifstream myfile;
    myfile.open(file_name);
    assert(myfile.is_open());
    std::string line;
    std::string word;
    //this while loop iterates thru each word, delimiting at spaces, tabs, and newlines
    while (getline(myfile, line,' ')) {
        MapReduce::MR_Emit(word, "1");
        
    }
    myfile.close();
}

void Reduce(std::string key, MapReduce::getter_t get_next, int partition_number) {
    int count = 0;
    std::string val = get_next(key, partition_number);
    while (!val.empty()) {
        count++;
        val = get_next(key, partition_number);
    }
    std::cout << key << count << std::endl;
}

int main(int argc, char *argv[]) {
    MapReduce::MR_Run(argc, argv, Map, 10, Reduce, 10, MapReduce::MR_DefaultHashPartition);
    return 0;
}
