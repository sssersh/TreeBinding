
#include "file.h"

#include <gtest/gtest.h>

#include <string>
#include <fstream>
#include <algorithm>

using namespace one_header_gen;

class generator_file_test : public testing::Test
{
public:
    const std::string_view test_file_path()
    {
        static const std::string path = "test_out_file.txt";
        return path;
    }

    std::string test_file =
R"(String 1
String two
String three
)";

    const std::string test_file_description =
R"(/**
 \file Filename
 \desc Description
*/
)";

};

TEST_F(generator_file_test, create_file)
{
    ASSERT_NO_THROW(file_t{});
    ASSERT_NO_THROW(file_t{generator_file_test::test_file_path()});
    ASSERT_NO_THROW(file_t{std::string{"String"}});

    file_t empty_file;
    ASSERT_TRUE(empty_file.to_string().empty());

//    std::ofstream raw_file {test_file_path()};
//    raw_file << test_file;
//    raw_file.flush();

    file_t file { test_file_path() };
    ASSERT_EQ(file.to_string(), test_file);
}

TEST_F(generator_file_test, add_operator)
{
    file_t file;
    file += test_file;
    file += test_file_description;

    auto result = test_file + test_file_description;
    ASSERT_EQ(result, file.to_string());

//    file.clear();
//    ASSERT_TRUE(file.to_string().empty());

    file_t file_2;
    file_2 += file;
    ASSERT_EQ(file.to_string(), file.to_string());
}

//TEST_F(generator_file_test, write)
//{
//    file_t file;
//    file += test_file;
//    file.write(test_file_path());
//
//    std::string raw_file_string, line;
//    std::ifstream raw_file {test_file_path()};
//    while(std::getline(raw_file, line))
//    {
//        raw_file_string += line + "\n";
//    }
//
//    ASSERT_EQ(file.to_string(), raw_file_string);
//}

TEST_F(generator_file_test, insert)
{
//    file_t file, inserted_file;
//    file += test_file;
//    inserted_file += test_file_description;
//
//    file.insert(0, inserted_file);
//    ASSERT_EQ(file.to_string(), test_file_description + test_file);
//    file.clear();
//
//    file += test_file;
//    auto lines_num =
//        std::count_if(
//            test_file.cbegin(),
//            test_file.cend(),
//            [](char symbol) { return symbol == '\n'; });
//    file.insert(lines_num, inserted_file);
//    ASSERT_EQ(file.to_string(), test_file + test_file_description);
//    file.clear();
//
//    file +=
//R"(String 1
//String 2
//String 3
//)";
//    inserted_file.clear();
//    inserted_file += "String 4";
//    file.insert(1, inserted_file);
//    ASSERT_EQ(file, file_t(
//std::string(R"(String 1
//String 4
//String 2
//String 3
//)")));
    ASSERT_TRUE(false);
}
