#include "mapreduce.hh"
#include <iostream>		//cout 
#include <sstream>
#include <fstream>
#include <map>			//for unordered map
#include <unordered_map>        //for unordered map 
#include <memory> 		//for unique ptr to mutex array
#include <mutex>		//for mutex
#include <vector>
#include <tuple>                // for key/value pair


using kv_t = std::tuple<std::string,std::vector<std::string>>;
using shard_t = std::vector<kv_t>;
using shard_vector_t = std::vector<shard_t>;


using mutex_ptr = std::unique_ptr<std::mutex>; //was originally a shared ptr
using mutex_map_t = std::map<std::string, mutex_ptr>;

// this is an example of what a lock will look like std::lock_guard<std::mutex> guard(*MUTEXES[KEY]);

shard_vector_t emit_map[num_partitions];
mutex_map_t mutexes; //there is one mutex in here for every vector in the emit map


void MapReduce::MR_Emit(const std::string& key, const std::string& value) {
	int num_partitions = 4; 
	bool found = false;
	unsigned long shard_id = MR_DefaultHashPartition(key, num_partitions);
		


	//iterate through each shard
	for (kv_t kv : shard_vector_t[shard_id]) {
		if (kv.first.compare(key) == 0) {
			std::lock_guard<std::mutex> guard(*mutexes[key]);
			kv.second.push_back(value);
		}
	}
	

	if (!found) {
		mutexes[key] = mutex_ptr(new std::mutex);
		shard_vector_t[shard_id] = std::tuple<key, std::vector<std::string>()>;
	}
}

//this should return the size of each slice
unsigned long MapReduce::MR_DefaultHashPartition(const std::string& key, int num_partitions) {

	unsigned long hash = 5381;
	char c;
	for(c : key) //iterate thru the letters in key
	{
		hash = hash * 33 + (int) c;
	}
	return hash % num_partitions;
}


static std::string get_next(const std::string& key, int partition_number) {
	//this somehow has to keep track of where we are when we're getting...
}

void MapReduce::MR_Run(int argc, char* argv[],
            mapper_t map, int num_mappers,
            reducer_t reduce, int num_reducers,
            partitioner_t partition) {
	



    return;
}
