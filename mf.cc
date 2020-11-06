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
        /*char *token, *dummy = root_friend;
        while ((token = strsep(&dummy, " ")) != NULL) {
            std::cout<<token<<":"<<root_friend<<std::endl;
            //MapReduce::MR_Emit(token, "1");
            //dummy = token;
        }*/
        ///*
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
            
            //std::cout <<"friend is still" <<s<<std::endl;
            //std::cout <<"printing s again"<<s<<std::endl;
            std::cout <<s<<":"<<root_friend<<std::endl;
            token = next;
        }//*/
    }
    free(line);
    fclose(fp);
}


void Reduce(std::string key, MapReduce::getter_t get_next, int partition_number) {
    //std::cout << "wc:Reduce() Begin" << std::endl;
    int count = 0;
    std::vector<std::string> list_of_friends;
    std::string friend_name; //this is the name of all the people with the same mutual friend
    std::string val = get_next(key, partition_number);
    while (!val.empty()) {
        friend_name.append(val);
        val = get_next(key, partition_number);
    }
    std::lock_guard<std::mutex> guard(mf_mutex);
    if (mutual_friends[friend_name].empty())

        mutual_friends[friend_name] = key;
    else {
        mutual_friends[friend_name].append(", ");
        mutual_friends[friend_name].append(key);
    }
    //std::cout << key << count << std::endl;
}

int main(int argc, char *argv[]) {
    num_friends = argc-1;
    
    MapReduce::MR_Run(argc, argv, Map, 4, Reduce, 4, MapReduce::MR_DefaultHashPartition);
    for(auto kv : mutual_friends) {
        std::cout << "[" <<kv.first <<"] : " <<kv.second <<", "<<std::flush;
    }
    std::cout<<std::endl;
    
    Map(argv[1]);
    return 0;
}