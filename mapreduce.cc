#include "mapreduce.hh"
#include <iostream>		//cout
#include <sstream>
#include <fstream>
#include <map>			//for unordered map
#include <unordered_map>        //for unordered map
#include <memory> 		//for unique ptr to mutex array
#include <mutex>		//for mutex
#include <vector>
#include <tuple>        //for key/value pair
#include <stack>  		//for stack in kv_t
#include <thread>			//include thread
#include <stdio.h>
#include <string.h>

using shard_t = std::unordered_map<std::string,std::stack<std::string>>; //shard of kv pairs
using shard_vector_t = std::vector<shard_t>;


using mutex_ptr = std::unique_ptr<std::mutex>; //was originally a shared ptr
using mutex_map_t = std::map<std::string, mutex_ptr>;
//this is an example of what a lock will look like std::lock_guard<std::mutex> guard(*MUTEXES[KEY]);

int num_partitions;
shard_vector_t emit_map;
mutex_map_t mutexes; //there is one mutex in here for every vector in the emit map

void initialize_emit_map()
{
	int x = 0;
	while ( x ++< num_partitions )
	{
		shard_t shard;
		emit_map.push_back(shard);
	}

}

void MapReduce::MR_Emit(const std::string& key, const std::string& value) {
	unsigned long shard_id = MR_DefaultHashPartition(key, num_partitions);
	shard_t::const_iterator got = emit_map[shard_id].find(key);

	if (got == emit_map[shard_id].end())
	{
		mutexes[key] = mutex_ptr(new std::mutex);
		std::stack<std::string> vals;
		vals.push(value);
		emit_map[shard_id][key] = vals;
	}


	//if the key exists, then we add to the vector
	std::lock_guard<std::mutex> guard(*mutexes[key]);
	emit_map[shard_id][key].push(value);

}

//this should return the size of each slice
unsigned long MapReduce::MR_DefaultHashPartition(const std::string& key, int num_partitions) {

	unsigned long hash = 5381;

	for(char c : key) //iterate thru the letters in key
	{
		hash = hash * 33 + (int) c;
	}
	return hash % num_partitions;
}

static std::string get_next(const std::string& key, int partition_number) {
		//we need to pop from the stack
		std::lock_guard<std::mutex> guard(*mutexes[key]);


		auto ret = emit_map[partition_number][key].top();
		emit_map[partition_number][key].pop();
		return ret;

}

void MapReduce::MR_Run(int argc, char* argv[],
            mapper_t map, int num_mappers,
            reducer_t reduce, int num_reducers,
            partitioner_t partition) {

		(void) num_mappers;
		initialize_emit_map();
		num_partitions = num_reducers;

		std::vector<std::string> filenames;

		for (int i = 0; i <= argc; i++) {
			map(argv[i]);
		}

		//keeping track of the reducer threads
		std::vector<std::thread> reducer_threads;
		//for each reducer, get what it needs to do, give it a thread
		for (int i = 0; i < num_reducers; i++) {
			//create a thread and add it to the vector
																						//= means by default, things that are accessed in the lambda (from outside) will be by value.
																						//     ^(in this case, that's only i)
																						//&emit_map means that emit_map is passed by reference
																						// this is because we want the value of i not to change when the thread is run
																						//but we don't want to copy emit map, which is why we access it by reference
			reducer_threads.push_back(std::thread ([=, &emit_map](){
				//this will store all the keys from the reducer's shard
				std::vector<std::string> keys;
				keys.reserve(emit_map[i].size());

				//for each key in this reducer's slice, reduce that key
				for(std::string k : keys) {
					//we call the reduce function that takes a key, and reduces it into one value
					reduce(k, get_next, i);
				}
			}));
		}

		for(int thread =0; thread< num_reducers; thread++){
			reducer_threads[thread].join();
		}


/* if we don't care about separate files.
		std::stringstream strstream;

		for (int i = 1; i < argc; i++) {
			std::ifstream in(argv[i]);       // Will throw if it fails
			strstream << in.rdbuf();    // Read entire file at once
		}
		strstream.str();
*/
//	delete[] emit_map;


    return;
}
