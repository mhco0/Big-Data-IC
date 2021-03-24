#include <gtest/gtest.h>
#include <logger/logger.h>
using namespace std;
using namespace qsbd;

TEST(LogTest, TestCreation){
    logger out("out.txt");
}

TEST(LogTest, TestSimpleDataOutput){
    logger out("out.txt");

    out << endl;
    out.setf(ios::ios_base::left);
    out.fill('*');
    out.width(4);
    out << "String test";
    out << 2;
    out << 0.3;
    out << true;
    out << 9LL << 1e-3;
}

TEST(LogTest, TestContainerOutput){
    logger out("out2.txt");
    vector<int> v = {2, 3, 5, 6};
    map<string, int> mp;
    pair<int, int> p = {1, 2};

    mp["test1"] = 0;
    mp["test2"] = 1;
    mp["test3"] = 2;

    out << v;
    out << mp;
    out << p;
}

TEST(LogTest, TestCombinedContainerOutput){
    logger out("out3.txt");
    vector<pair<int, int>> vp = {{1, 2}, {3, 4}, {5, 6}};

    out << vp;
}