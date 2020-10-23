#include "mapreduce.hh"
#include <iostream>
#include <sstream>
#include <fstream>
#include <tuple>	//for tuple

#pragma once

using kv_t = std::tuple<std::string, std::string>;
//we need a shared data structure for the keys to be emitted to


void MapReduce::MR_Emit(const std::string& key, const std::string& value) {
	//make a kv pair

	// MAKE TUPLE HERE
	
	//then we put it in the big map that we have to reduce

	// I GUESS WE'LL HAVE A BIG MAP THAT WE PUT IT IN
}

unsigned long MapReduce::MR_DefaultHashPartition(const std::string& key, int num_partitions) {

    return 0;
}



void MapReduce::MR_Run(int argc, char* argv[],
            mapper_t map, int num_mappers,
            reducer_t reduce, int num_reducers,
            partitioner_t partition) {
	//we need to define a get_next for reducer_t

    return;
}