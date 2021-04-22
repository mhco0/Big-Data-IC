#include <gtest/gtest.h>
#include <quadtree/quadtree.h>
#include <stream_maker/stream_maker.h>
using namespace std;
using namespace qsbd;
using namespace qsbd::stream_maker;

vector<int> brute_force_search(const vector<pair<int, pair<double, double>>>& stream, const aabb<double>& region){
    vector<int> ans;

    for(auto&it : stream){
        point<double> p(it.second.first, it.second.second);
        if (region.contains(p)){
            ans.push_back(it.first);
        }
    }

    return ans;
}

TEST(PointTest, TestAll){
    point<int> p1(12, 14);
    point<double> p2(0.0,  2.0);
    point<float> p3(0.0f, 1.0f);

    EXPECT_FLOAT_EQ(p3.x(), 0.0f);
    EXPECT_FLOAT_EQ(p3.y(), 1.0f);
    EXPECT_DOUBLE_EQ(p2.x(), 0.0);
    EXPECT_DOUBLE_EQ(p2.y(), 2.0);
    EXPECT_EQ(p1.x(), 12);
    EXPECT_EQ(p1.y(), 14);

    p1.x(18);

    EXPECT_EQ(p1.xy(), make_pair(18, 14));
}

TEST(AABBTest, TestConstruction){
    aabb<double> collider(0.0, 0.0, 4.0, 4.0);
    auto bounds = collider.bounds();
    point<double> minp = bounds.first;
    point<double> maxp = bounds.second;

    
    EXPECT_DOUBLE_EQ(minp.x(), 0.0);
    EXPECT_DOUBLE_EQ(minp.y(), 0.0);
    EXPECT_DOUBLE_EQ(maxp.x(), 4.0);
    EXPECT_DOUBLE_EQ(maxp.y(), 4.0);
}

TEST(AABBTest, TestOverlap){
    aabb<double> region1(0.0, 0.0, 6.0, 6.0);
    aabb<double> region2(1.0, 1.0, 3.0, 3.0);
    aabb<double> region3(5.0, 5.0, 10.0, 10.0);
    aabb<double> region4(0.5, 0.5, 6.0, 6.0);

    EXPECT_TRUE(region1.intersects(region1));
    EXPECT_TRUE(region1.intersects(region2));
    EXPECT_TRUE(region1.intersects(region3));
    EXPECT_TRUE(region1.intersects(region4));
    
    EXPECT_TRUE(region2.intersects(region2));
    EXPECT_TRUE(region2.intersects(region1));
    EXPECT_FALSE(region2.intersects(region3));
    EXPECT_TRUE(region2.intersects(region4));

    EXPECT_TRUE(region3.intersects(region3));
    EXPECT_TRUE(region3.intersects(region1));
    EXPECT_FALSE(region3.intersects(region2));
    EXPECT_TRUE(region3.intersects(region4));

    EXPECT_TRUE(region4.intersects(region4));
    EXPECT_TRUE(region4.intersects(region1));
    EXPECT_TRUE(region4.intersects(region2));
    EXPECT_TRUE(region4.intersects(region3));
}

TEST(AABBTest, TestIfIsInside){
    aabb<double> region1(0.0, 0.0, 5.0, 5.0);
    aabb<double> region2(1.0, 1.0, 3.0, 4.0);
    aabb<double> region3(3.0, 4.0, 7.0, 8.0);
    aabb<double> region4(1.0, 1.0, 3.0, 3.0);

    EXPECT_TRUE(region1.is_inside(region1));
    EXPECT_FALSE(region1.is_inside(region2));
    EXPECT_FALSE(region1.is_inside(region3));
    EXPECT_FALSE(region1.is_inside(region4));
    
    EXPECT_TRUE(region2.is_inside(region2));
    EXPECT_TRUE(region2.is_inside(region1));
    EXPECT_FALSE(region2.is_inside(region3));
    EXPECT_FALSE(region2.is_inside(region4));

    EXPECT_TRUE(region3.is_inside(region3));
    EXPECT_FALSE(region3.is_inside(region1));
    EXPECT_FALSE(region3.is_inside(region2));
    EXPECT_FALSE(region3.is_inside(region4));

    EXPECT_TRUE(region4.is_inside(region4));
    EXPECT_TRUE(region4.is_inside(region1));
    EXPECT_TRUE(region4.is_inside(region2));
    EXPECT_FALSE(region4.is_inside(region3));
}

TEST(QuadtreeTest, TestInsertion){
    quadtree<int> test;
    vector<point<double>> points = {{3.0, 3.0}, {0.0, 0.0}, {6.0, 6.0}, {1.0, 3.0}};
    vector<int> values = {14, 15, 17, 23};

    for(int i = 0; i < points.size(); i++){
        test.insert(points[i], values[i]);
    }

    auto ret_points = test.query(aabb<double>(0.0, 0.0, 6.0, 6.0));

    EXPECT_EQ(ret_points.size(), values.size());

    for(auto& it : ret_points){
        bool ok = false;

        for(auto& value : values){
            if (it == value) ok = true;
        }

        EXPECT_TRUE(ok);
    }
}

TEST(QuadtreeTest, TestQuery){
    vector<pair<int, pair<double, double>>> stream = random_stream_in_region(1000000, 0.0, 0.0, 100.0, 100.0);
    quadtree<int> test;
    aabb<double> region1(0.0, 0.0, 50.0, 50.0);
    aabb<double> region2(10.0, 20.0, 40.0, 70.0);
    aabb<double> region3(50.0, 60.0, 80.0, 70.0);

    for(auto&it : stream){
        point<double> p(it.second.first, it.second.second);

        test.insert(p, it.first);
    }

    vector<int> ret1 = test.query(region1);
    vector<int> ret2 = test.query(region2);
    vector<int> ret3 = test.query(region3);

    vector<int> brute_ret1 = brute_force_search(stream, region1);
    vector<int> brute_ret2 = brute_force_search(stream, region2);
    vector<int> brute_ret3 = brute_force_search(stream, region3);


    sort(ret1.begin(), ret1.end());
    sort(ret2.begin(), ret2.end());
    sort(ret3.begin(), ret3.end());

    sort(brute_ret1.begin(), brute_ret1.end());
    sort(brute_ret2.begin(), brute_ret2.end());
    sort(brute_ret3.begin(), brute_ret3.end());

    EXPECT_EQ(ret1.size(), brute_ret1.size());
    EXPECT_EQ(ret2.size(), brute_ret2.size());
    EXPECT_EQ(ret3.size(), brute_ret3.size());

    
    for(int i = 0; i < ret1.size(); i++){
        EXPECT_EQ(ret1[i], brute_ret1[i]);
    }

    for(int i = 0; i < ret2.size(); i++){
        EXPECT_EQ(ret2[i], brute_ret2[i]);
    }

    for(int i = 0; i < ret3.size(); i++){
        EXPECT_EQ(ret3[i], brute_ret3[i]);
    }
    

}