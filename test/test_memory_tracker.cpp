#include <gtest/gtest.h>
#include <stdio.h>
//#include <memory_tracker/memory_tracker.h>
#include <stl_memory_tracker/stl_memory_tracker.hpp>
#include <dcs_factory/dcs_factory.h>
#include <quantile_quadtree/quantile_quadtree.hpp>
//using namespace qsbd;
//using json = nlohmann::json;


class simple_object{
    int att1;
    double att2;
    std::vector<int> v;
public:
    simple_object(int a, double b){
        this->att1 = a;
        this->att2 = b;

        this->v.reserve(10);

        for(int i = 0; i < 10; i++){
            this->v.emplace_back(i);
        }
    }

    ~simple_object(){
    }
};

/*
TEST(MemoryTrackerTest, TestSimpleNew){
    json out_log;
    int * value = new int(83);
    int * array = new int[23];

    mem_track::track_list_memory_usage(out_log);
    
    cout << out_log.dump(4);
    cout << endl;
    cout << "deleting.." << endl;

    delete value;
    delete[] array;

    mem_track::track_list_memory_usage(out_log);

    cout << out_log.dump(4);
}


TEST(MemoryTrackerTest, TestSimpleStruct){
    json out_log;
    simple_object * test = new simple_object(2, 0.4);

    mem_track::track_list_memory_usage(out_log);

    cout << out_log.dump(4);
    cout << endl;
    cout << "deleting.." << endl;

    delete test;

    mem_track::track_list_memory_usage(out_log);

    cout << out_log.dump(4);
}
*/

TEST(MemoryTrackerTest, TestStlTrack){
    {
        std::vector<std::pair<std::pair<int, int>, std::pair<double, double>>> stream;

        std::cout << sizeof(simple_object) << std::endl;
        std::cout << sizeof(stream) << std::endl;
        std::cout << sizeof(std::pair<std::pair<int, int>, std::pair<double, double>>) << std::endl;
        /*double error = 0.3;
        int universe = 1024;
        int depth = 10;

        qsbd::aabb<int> resolution(0, 0, 1 << depth, 1 << depth);

        qsbd::dcs_factory factory(error, universe);
        qsbd::quantile_quadtree<int> qq_test(resolution, depth, &factory);

        for(int i = 0; i < (1 << depth); i++){
            for(int j = 0; j < (1 << depth); j++){
                std::uniform_int_distribution<int> v(0, universe - 1);		
                qsbd::point<int> coord(i, j);
                qq_test.update(coord, v(qsbd::generator), 1);
            }
        }*/
        

        stream.reserve((int) 1e8);

        for(int i = 0; i < (int) 1e8; i++){
            stream.emplace_back(std::make_pair(std::make_pair(i, 1), std::make_pair(0.0, 0.0)));
        }

        std::cout << "In scope" << std::endl; 

        for(auto& it : (*get_map())){
            std::cout << it.first << " : " << it.second << " bytes" << std::endl;
        }

    }

    std::cout << "Out of scope" << std::endl;
}