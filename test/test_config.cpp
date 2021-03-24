#include <gtest/gtest.h>
#include <config/config.h>
using namespace std;
using namespace qsbd;

TEST(ConfiguratorTest, TestingLoad){
    configurator::load("config.cfg");

    
    EXPECT_EQ(configurator::get_test(), "test_quantile_quadtree");
    EXPECT_EQ(configurator::get_data_file(), "data.txt");
    EXPECT_EQ(configurator::get_path(), "./");
    EXPECT_EQ(configurator::get_output_file(), "out.txt");
    EXPECT_EQ(configurator::get_file_name(), "config.cfg");
    vector<string> args = configurator::get_args();

    ASSERT_TRUE(args.size() == 8);

    EXPECT_EQ(args[0], "0.0");
    EXPECT_EQ(args[1], "0.0");
    EXPECT_EQ(args[2], "1280.0");
    EXPECT_EQ(args[3], "720.0");
    EXPECT_EQ(args[4], "10");
    EXPECT_EQ(args[5], "0.3");
    EXPECT_EQ(args[6], "10000");
    EXPECT_EQ(args[7], "10");
}
