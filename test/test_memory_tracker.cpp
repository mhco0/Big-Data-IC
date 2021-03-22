#include <gtest/gtest.h>
#include <stdio.h>
#include <memory_tracker/memory_tracker.h>
using namespace std;
using namespace qsbd;

class simple_object{
    int att1;
    double att2;
public:
    simple_object(int a, double b){
        this->att1 = a;
        this->att2 = b;
    }

    ~simple_object(){
    }
};

TEST(MemoryTrackerTest, TestSimpleNew){
    int * value = new int(83);
    int * array = new int[23];

    mem_track::track_list_memory_usage();
    
    cout << endl;
    cout << "deleting.." << endl;

    delete value;
    delete[] array;

    mem_track::track_list_memory_usage();
}


TEST(MemoryTrackerTest, TestSimpleStruct){
    simple_object * test = new simple_object(2, 0.4);

    mem_track::track_list_memory_usage();

    cout << endl;
    cout << "deleting.." << endl;

    delete test;

    mem_track::track_list_memory_usage();
}