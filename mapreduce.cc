#include "mapreduce.hh"

void MapReduce::MR_Emit(const std::string& key, const std::string& value) {

    return;
}

unsigned long MapReduce::MR_DefaultHashPartition(const std::string& key, int num_partitions) {

    return 0;
}

void MapReduce::MR_Run(int argc, char* argv[],
            mapper_t map, int num_mappers,
            reducer_t reduce, int num_reducers,
            partitioner_t partition) {

    return;
}

int main(int argc, char **argv) {


    return 0;
}