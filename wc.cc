#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>     //for reading from file
#include <fstream>      //also reading from a file
#include "mapreduce.cc"

/*
void Map(const char* file_name) {
    std::cout << "wc:Map() Begin" << std::endl;
    FILE *fp = fopen(file_name, "r");
    assert(fp != NULL);
    char *line = NULL;
    size_t size = 0;
    while (getline(&line, &size, fp) != -1) {
        char *token, *dummy = line;
        while ((token = strsep(&dummy, " \t\n\r")) != NULL) {
	    //std::cout<<token<<std::endl;
            MapReduce::MR_Emit(token, "1");
        }
    }
    free(line);
    fclose(fp);
}
*/
void Map(std::string file_name) {
    std::ifstream myfile;
    myfile.open(file_name);
    assert(myfile.is_open());
    std::string line;
    //this while loop iterates thru each word, delimiting at spaces, tabs, and newlines
    while (getline(myfile, line,' ')) {
	std::cout << line << std::endl;
        //MapReduce::MR_Emit(line, "1");
    }
    myfile.close();
} 


void Reduce(std::string key, MapReduce::getter_t get_next, int partition_number) {
    std::cout << "wc:Reduce() Begin" << std::endl;
    int count = 0;
    std::string val = get_next(key, partition_number);
    while (!val.empty()) {
        count++;
        val = get_next(key, partition_number);
    }
    std::cout << key << count << std::endl;
}

int main(int argc, char *argv[]) {
    std::cout << "wc:main() Begin" << std::endl;
    MapReduce::MR_Run(argc, argv, Map, 1, Reduce, 1, MapReduce::MR_DefaultHashPartition);
    return 0;
}
