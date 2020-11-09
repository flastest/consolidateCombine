#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>     //for reading from file
#include <fstream>      //also reading from a file
#include "mapreduce.cc"
#include <unordered_map>
#include <mutex>
#include <string.h>
#include <iterator>  //for std::size

using mf_t = std::unordered_map<std::string,std::string>; //shard of kv pairs

mf_t mutual_friends;
std::mutex mf_mutex;
//entry will be like RyanAriel:Eitan,Jim

std::mutex print_mutex;

int NUM_MAPPERS = 1;
int NUM_REDUCERS = 1;

int num_friends; 


//maps from a file name
//emits the lotr character and who they're friends with
void Map(const char* file_name) {
    if(DEBUG)
    {
        std::lock_guard<std::mutex> guard(print_mutex);
        std::cout << "Map() Begin" << std::endl;
    }

    FILE *fp = fopen(file_name, "r");
    assert(fp != NULL);
    char *line = NULL;
    size_t size = 0;
    
    //get all the lines
    while (getline(&line, &size, fp) != -1) {
        
        if(DEBUG)
        {
            std::lock_guard<std::mutex> guard(print_mutex);
            std::cout <<"current line in map: "<<line<<std::endl;
        }
        std::string emitted_vals = "";

        char *token = line;
        char* rest = line; 
        char* root_friend = strtok_r(rest,";",&rest);
        token = strtok_r(rest,",",&rest);
        char* next;

        //within each line, get every friend of the first character
        while (token != NULL && *token != '\r' && *token != '\n') {
	    
            std::string s(token);
            next = strtok_r(rest,",",&rest);
	    
            //trim the newline off of s if its the last character in the line
            if (!next) 
            {
                s = s.substr(0,s.find('\n'));
                s = s.substr(0,s.find('\r'));
            }
	    
            //emit a set of friends
            if(DEBUG)
            {
                std::lock_guard<std::mutex> guard(print_mutex);
                std::cout <<"emitting that these 2 are friends: " <<s<<"<-"<<root_friend<<std::endl;
            }
	    
            MapReduce::MR_Emit(s, root_friend);
            

            emitted_vals.append(s);
	    
            token = next;
        }
        
        if(DEBUG)
        {
            std::lock_guard<std::mutex> guard(print_mutex);
            std::cout <<"emitted people for "<<root_friend<<":"<<emitted_vals<<std::endl;
        }	
    }
    
    //releasing the memory we took
    free(line);
    fclose(fp);

    if(DEBUG)
    {
	std::lock_guard<std::mutex> guard(print_mutex);
	std::cout << "Map() End" << std::endl;
    }
    
}


//takes a key. the vals associated with that key are all the mutual friends of the key
// compiles all the vals, and associates that key with it.
void Reduce(std::string key, MapReduce::getter_t get_next, int partition_number) {
    if(DEBUG)
    {
	std::lock_guard<std::mutex> guard(print_mutex);
	std::cout << "Reduce() Begin: " << key << std::endl;
    }
    
    std::string friend_name; //this is the group of all the people with the same mutual friend
    
    //this is the first friend in that group
    std::string val = get_next(key, partition_number);
    int num_name = 0;
    while (!val.empty()) {
        friend_name.append(val);
	friend_name.append(" ");
	num_name++;
        val = get_next(key, partition_number);
    }
    //if friend isn't empty either add key to the existing group of people wiht mutual friends
    // or make a new list and add that new group with key to the list of mutual friends.
    if (num_name >= 2) {
        std::lock_guard<std::mutex> guard(mf_mutex);
        //this group isn't kept track of yet, add it to mutual_friends
        if (mutual_friends[friend_name].empty())
        {
            mutual_friends[friend_name] = key;
        }
        else {
            // if the group is kept track of, then append this friend to the names of the mfs of the group
            mutual_friends[friend_name].append(", ");
            mutual_friends[friend_name].append(key);
        }
        //end scope of lock
    }
    
    if(DEBUG)
    {
	std::lock_guard<std::mutex> guard(print_mutex);
	std::cout << "Reduce() End: " << key << std::endl;
    }
}

int main(int argc, char *argv[]) {
    num_friends = argc-1;
    
    MapReduce::MR_Run(argc, argv, Map, NUM_MAPPERS, Reduce, NUM_REDUCERS, MapReduce::MR_DefaultHashPartition);
    for(auto kv : mutual_friends) {
        std::cout << "[ " <<kv.first <<"] : " << kv.second << ", "<<std::endl;
    }
    
    return 0;
}
