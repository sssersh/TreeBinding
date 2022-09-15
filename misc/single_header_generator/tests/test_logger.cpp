
#include "logger.h"

#include <gtest/gtest.h>

#include <experimental/filesystem>

using namespace one_header_gen;
namespace filesystem = std::experimental::filesystem;

class LoggerTest : public ::testing::Test
{
public:
    void TearDown() override
    {
        filesystem::remove(filename());
    }

    static std::string filename() { return "out.log"; }
};

TEST_F(LoggerTest, CreateFile)
{
    logger_t::set_out_file(LoggerTest::filename());

    ASSERT_TRUE(std::experimental::filesystem::exists(LoggerTest::filename()));
}
