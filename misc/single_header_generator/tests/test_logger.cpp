
#include "logger.h"

#include <gtest/gtest.h>

#include <experimental/filesystem>

using namespace one_header_gen;
namespace filesystem = std::experimental::filesystem;

class logger_test : public ::testing::Test
{
public:
    void SetUp() override
    {
        logger_t::set_out_file(logger_test::filename());
    }

    void TearDown() override
    {
        filesystem::remove(filename());
    }

    static std::string filename() { return "out.log"; }
};

TEST_F(logger_test, create_file)
{
    ASSERT_TRUE(std::experimental::filesystem::exists(logger_test::filename()));
}

TEST_F(logger_test, write_to_file)
{    
    logger_t::write("First string\n");
    logger_t::write("Second string ", "Argument 2 ", 555);

    logger_t::set_out_file("");

    std::ifstream file {logger_test::filename()};
    std::vector<std::string> lines(2);

    std::getline(file, lines[0]);
    std::getline(file, lines[1]);

    ASSERT_EQ(lines[0], "First string");
    ASSERT_EQ(lines[1], "Second string Argument 2 555");    
}

static void write_test()
{
    LOG("String 1");
    LOG("String 2 ", "Second argument ", 111);
}

TEST_F(logger_test, write_to_file_from_foo)
{    
    write_test();

    logger_t::set_out_file("");

    std::ifstream file {logger_test::filename()};
    std::vector<std::string> lines(2);

    std::getline(file, lines[0]);
    std::getline(file, lines[1]);

    ASSERT_EQ(lines[0], "[       write_test       ] String 1");
    ASSERT_EQ(lines[1], "[       write_test       ] String 2 Second argument 111");    
}
