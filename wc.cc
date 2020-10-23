#include <assert>
#include <stdio>
#include <stdlib>
#include <string>
#include <iostream>     //for reading from file
#include <fstream>      //also reading from a file
#include "mapreduce.hh"

void Map(std::string file_name) {
    ifstream myfile;
    myfile.open(file_name);
    assert(myfile.is_open());

    std::string word;
    //this while loop iterates thru each word, delimiting at spaces, tabs, and newlines
    while (getline(myfile, line," \t\n\r")) {
        MR_Emit(word, "1");
        
    }
    myfile.close();
}

void Reduce(std::string key, mapreduce::getter_t get_next, int partition_number) {
    int count = 0;
    char *value;
    while ((value = get_next(key, partition_number)) != NULL)
        count++;
    printf("%s %d\n", key, count);
}

int main(int argc, char *argv[]) {
    MR_Run(argc, argv, Map, 10, Reduce, 10, MR_DefaultHashPartition);
}
