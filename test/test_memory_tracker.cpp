#include <gtest/gtest.h>
#include <stdio.h>
#include <memory_tracker/memory_tracker.h>
using namespace qsbd;
using json = nlohmann::json;


class simple_object{
public:
    int att1;
    double att2;
    std::vector<int> v;

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
