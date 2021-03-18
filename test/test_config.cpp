#include <gtest/gtest.h>
#include <config/config.h>
using namespace std;

TEST(ConfiguratorTest, TestingLoad){
    configurator::load("config.txt");

    
    EXPECT_EQ(configurator::get_test(), "test_config");
    EXPECT_EQ(configurator::get_data_file(), "data.txt");
    EXPECT_EQ(configurator::get_path(), "./");
    EXPECT_EQ(configurator::get_output_file(), "out.txt");
    EXPECT_EQ(configurator::get_file_name(), "config.txt");
    vector<string> args = configurator::get_args();

    EXPECT_EQ(args[0], "10");
    EXPECT_EQ(args[1], "30");
    EXPECT_EQ(args[2], "40");
    EXPECT_EQ(args[3], "50");
}
