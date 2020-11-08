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



int num_friends; 

void Map(const char* file_name) {
    FILE *fp = fopen(file_name, "r");
    assert(fp != NULL);
    char *line = NULL;
    size_t size = 0;
    while (getline(&line, &size, fp) != -1) {
        char *token = line;
        char *root_friend = strtok(line,";"); //Get the root friend.
        token = strtok(NULL,",");

        char* next;
        while (token != NULL && *token != '\r' && *token != '\n') {
            std::string s(token);
            next = strtok(NULL,",");
            //trim the newline off of s
            if (!next) 
            {
                //std::cout<<"length of "<<s<< " is "<<static_cast<int>(s.length())<<std::endl;
                s = s.substr(0,s.find('\n'));
                s = s.substr(0,s.find('\r'));
            }
            MapReduce::MR_Emit(s, root_friend);
            //std::cout <<s<<"<-"<<root_friend<<std::endl;
            token = next;
        }
    }
    free(line);
    fclose(fp);
}


void Reduce(std::string key, MapReduce::getter_t get_next, int partition_number) {
    std::vector<std::string> list_of_friends;
    std::string friend_name; //this is the name of all the people with the same mutual friend
    std::string val = get_next(key, partition_number);
    std::string first_val;
    if (!val.empty()) {
	first_val = val;
        val = get_next(key, partition_number);
	if (!val.empty()) {
	    friend_name.append(first_val);
            friend_name.append(" ");
            friend_name.append(val);
	    friend_name.append(" ");
	    val = get_next(key, partition_number);
	}
    }
    while (!val.empty()) {
        friend_name.append(val);
	friend_name.append(" ");
        val = get_next(key, partition_number);
    }
    if (!friend_name.empty()) {
        std::lock_guard<std::mutex> guard(mf_mutex);
        if (mutual_friends[friend_name].empty())
            mutual_friends[friend_name] = key;
        else {
            mutual_friends[friend_name].append(", ");
            mutual_friends[friend_name].append(key);
        }
    }
}

int main(int argc, char *argv[]) {
    num_friends = argc-1;
    
    MapReduce::MR_Run(argc, argv, Map, 2, Reduce, 2, MapReduce::MR_DefaultHashPartition);
    for(auto kv : mutual_friends) {
        std::cout << "[ " <<kv.first <<"] : " << kv.second << ", "<<std::endl;
    }
    //std::cout<<std::endl;
    
    return 0;
}