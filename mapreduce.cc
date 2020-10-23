#include "mapreduce.hh"
#include <iostream>		//cout 
#include <sstream>
#include <fstream>
#include <map>			//for unordered map
#include <unordered_map>//for unordered map 
#include <memory> 		//for unique ptr to mutex array
#include <mutex>		//for mutex
#include <vector>

#pragma once


using map_t = std::unordered_map<std::string, std::vector<std::string>>;

using mutex_ptr = std::unique_ptr<std::mutex>; //was originally a shared ptr
using mutex_map_t = std::map<std::string, mutex_ptr>;

// this is an example of what a lock will look like std::lock_guard<std::mutex> guard(*MUTEXES[KEY]);

map_t emit_map; //this is where we emit mutexes to
mutex_map_t mutexes; //there is one mutex in here for every vector in the emit map


void MapReduce::MR_Emit(const std::string& key, const std::string& value) {
	map_t::const_iterator found = emit_map.find(key);

	//need a mechanism to determine if we're adding a rod that's already been added
	if (found == emit_map.end()) {
		mutexes[key] = mutex_ptr(new std::mutex);
		//TODO create a new vector
		emit_map[key] = std::vector<std::string>();
	}

	std::lock_guard<std::mutex> guard(*mutexes[key]);
	map[key].push_back(value);
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