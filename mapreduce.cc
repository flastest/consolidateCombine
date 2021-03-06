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
//          1. LOCK ALL OF EMIT AND MAKE SURE THE TIME IS GOOD
//          2. LOCK EMIT MAP!!!!
const bool DEBUG = false;

using shard_t = std::unordered_map<std::string,std::stack<std::string>>; //shard of kv pairs
using shard_vector_t = std::vector<shard_t>;


using mutex_ptr = std::unique_ptr<std::mutex>; //was originally a shared ptr
using mutex_map_t = std::map<std::string, mutex_ptr>;
//this is an example of what a lock will look like std::lock_guard<std::mutex> guard(*MUTEXES[KEY]);

using shard_mutex_vector_map_t = std::map<int, mutex_ptr>;


//mutex_map_ mutex locks all the mutexes. we can't create a new mutex without the creator mutex
std::mutex mutex_map_mutex;

//we're gonna lock emit! to make sure that nothing is going wrong in it
std::mutex emit_lock;

int num_partitions;
shard_vector_t emit_map;
mutex_map_t mutexes; //there is one mutex in here for every vector in the emit map

shard_mutex_vector_map_t shard_mutex_vector;


MapReduce::partitioner_t partitioner;

void initialize_emit_map()
{
	//std::cout<<"begin inititize"<<std::endl;
	for (int x = 0; x < num_partitions; x++)
	{
		shard_t shard;
		emit_map.push_back(shard);
		//vector of shard mutices
		//i know that's not the plural of mutex
		shard_mutex_vector[x] = mutex_ptr(new std::mutex);


	}
	//std::cout << "emit map is this siseze:  "<<emit_map.size();
	//std::cout<<" inititize mipmap is over\n"<<std::endl;
}

void MapReduce::MR_Emit(const std::string& key, const std::string& value) {
	
	//std::cout<<"EMITTING. emit map length is "<<emit_map.size()<<std::endl;
	if (DEBUG) std::cout << "MR:EMIT -> the key is [" << key << "] The value is ["<< value << "]" <<std::endl;
	unsigned long shard_id = partitioner(key, num_partitions);



	
	//might need to lock the emit map

	std::lock_guard<std::mutex> shard_guard(*shard_mutex_vector[shard_id]);

	//SHARD MUTEX???????????
	
	if (!emit_map[shard_id].contains(key))
	{
		
		//we can only make 1 mutex at a time
		//ok, we have the mutex_map_mutex now.
		//check again to make sure that we still need to make a new mutex
		
		std::lock_guard<std::mutex> mutex_map_guard(mutex_map_mutex);
		mutexes[key] = mutex_ptr(new std::mutex);

		std::stack<std::string> vals;
		emit_map[shard_id][key] = vals;
	}
	
	//if the key exists, then we add to the vector
	//std::cout <<" EMITTING adding [" <<value<<"] to ["<<key<<"] in " << "shard " << shard_id << "\n";
	emit_map[shard_id][key].push(value);

}

//this should return the size of each slice
unsigned long MapReduce::MR_DefaultHashPartition(const std::string& key, int num_partitions) {

	//std::cout<<"DEFAULT HASH PARTITON"<<std::endl;
	unsigned long hash = 5381;

	for(char c : key) //iterate thru the letters in key
	{
		hash = hash * 33 + (int) c;
	}
	//std::cout<<"hash is "<<hash%num_partitions<<std::endl;
	return hash % num_partitions;
}

static std::string get_next(const std::string& key, int partition_number) {
		if (emit_map[partition_number][key].empty())
		{
			return "";
		}

		//we need to pop from the stack
		std::lock_guard<std::mutex> guard(*mutexes[key]);
		//std::cout<<"mutex locked\n";
		//std::cout<<"partition number: "<<partition_number<<std::endl;

		//std::cout<<"the key is [" <<key<<"]\n";
		auto ret = emit_map[partition_number][key].top();
		emit_map[partition_number][key].pop();
		if(ret.empty()) ret = "";
		//std::cout << "returning " <<ret<<std::endl;
		return ret;

}

void MapReduce::MR_Run(int argc, char* argv[],
            mapper_t map, int num_mappers,
            reducer_t reduce, int num_reducers,
            partitioner_t partition) {
	//std::cout <<"MR_RUN\n";
	partitioner = partition;
	num_partitions = num_reducers;
	initialize_emit_map();

	std::vector<std::string> filenames;

	//keeping track of mapper_threads
	std::vector<std::thread> mapper_threads;

	for (int i = 0; i < num_mappers; i++) {
		//std::cout<<"FILENAME IS "<< argv[i]<<std::endl;
		if (DEBUG) std::cout <<"starting map thread "<<i<<std::endl;
		mapper_threads.push_back(
			std::thread([=](){
				for (int j = i+1; j < argc; j = j + num_mappers) {
					map(argv[j]);
				}
			}
		));
	}

	//join them
	for(int thread = 0; thread < num_mappers; thread++){
		if (DEBUG) std::cout<<"joining mapper "<<thread<<std::endl;
		mapper_threads[thread].join();
		if (DEBUG) std::cout<<"joined mapper "<<thread<<std::endl;
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
		if (DEBUG) std::cout <<"starting reduce thread "<<i<<std::endl;
		reducer_threads.push_back(
			std::thread ([=](){
			//this will store all the keys from the reducer's shard
				std::vector<std::string> keys;
				keys.reserve(emit_map[i].size());
				for(auto kv : emit_map[i]) {
				keys.push_back(kv.first);
				}
				//std::cout<<" REDUCE THREDA: there are "<<keys.size()<< " keys in shard " <<i<<"\n";


				//std::cout<<" REDUCE THREAD: list of keys in this shard is: "<<std::flush;
				/*
				for(std::string k : keys){
					std::cout<<"["<<k<<"], ";
				}
				std::cout<<std::endl;
				*/

				//for each key in this reducer's slice, reduce that key
				for(std::string k : keys) {
					//we call the reduce function that takes a key, and reduces it into one value
					//std::cout<<"REDUCING KEY ["<<k<<"] \n";
					reduce(k, get_next, i);
				}
			}
		));
	}

	for(int thread = 0; thread < num_reducers; thread++){
		if (DEBUG) std::cout<<"joining reducer "<<thread<<std::endl;
		reducer_threads[thread].join();
		if (DEBUG) std::cout<<"joined reducer "<<thread<<std::endl;
		//std::cout<<"joining threads for reducing"<<std::endl;
	}


    return;
}
