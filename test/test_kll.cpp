#include <gtest/gtest.h>
#include <kll/kll.hpp>
using namespace std;

TEST(KllTest, TestUpdate){    
	vector<int> input = {2, 3, 4, 5, 6, 12, 32, 13, 45};
	kll<int> test(0.3);

	for(auto&it : input){
		test.update(it);
		test.print();
	}
}

TEST(KllTest, TestGoogleAssert){
	ASSERT_TRUE(false);
}
