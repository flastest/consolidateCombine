#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>     //for reading from file
#include <fstream>      //also reading from a file
#include "mapreduce.cc"
#include <unordered_map>
#include <mutex>
#include <stdlib.h>

const int NUM_MAPPERS = atoi(getenv("MAPS"));
const int NUM_REDUCERS = atoi(getenv("REDS"));

using wc_t = std::unordered_map<std::string,int>; //shard of kv pairs

wc_t counts;
std::mutex count_mutex;


void Map(const char* file_name) {
    //std::cout << "wc:Map() Begin" << std::endl;
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

void Reduce(std::string key, MapReduce::getter_t get_next, int partition_number) {
    //std::cout << "wc:Reduce() Begin" << std::endl;
    int count = 0;
    std::string val = get_next(key, partition_number);
    while (!val.empty()) {
        count++;
        val = get_next(key, partition_number);
    }
    std::lock_guard<std::mutex> guard(count_mutex);
    counts[key] = count;
    //std::cout << key << count << std::endl;
}

int main(int argc, char *argv[]) {
    //std::cout << "wc:main() Begin" << std::endl;
    MapReduce::MR_Run(argc, argv, Map, NUM_MAPPERS, Reduce, NUM_REDUCERS, MapReduce::MR_DefaultHashPartition);

    for(auto kv : counts) {
        std::cout << "[" <<kv.first <<"] : " << kv.second <<", "<<std::flush;
    }
    std::cout<<std::endl;

    return 0;
}
