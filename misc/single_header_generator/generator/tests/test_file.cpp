
#include "file.h"

#include <gtest/gtest.h>

#include <string>
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

//    std::string test_file =
//R"(String 1
//String two
//String three
//)";
//
//    const std::string test_file_description =
//R"(/**
// \file Filename
// \desc Description
//*/
//)";

};

TEST_F(generator_file_test, create_file)
{
    ASSERT_NO_THROW(file_t{});
    ASSERT_NO_THROW(file_t{generator_file_test::test_file_path()});
}

TEST_F(generator_file_test, get_path)
{
    auto file = file_t{generator_file_test::test_file_path()};
    ASSERT_EQ(file.get_path(), generator_file_test::test_file_path());
}

TEST_F(generator_file_test, get_lines)
{
    file_t file;
    auto& lines = file.get_lines();

    std::string test_line = "test line";
    lines.emplace_back(test_line);

    ASSERT_EQ(file.get_lines().at(0), test_line);
}

//TEST_F(generator_file_test, add_operator)
//{
//    file_t file;
//    file += test_file;
//    file += test_file_description;
//
//    auto result = test_file + test_file_description;
//    ASSERT_EQ(result, file.to_string());
//
//    file_t file_2;
//    file_2 += file;
//    ASSERT_EQ(file.to_string(), file.to_string());
//}
//
//TEST_F(generator_file_test, insert)
//{
//    file_t file, inserted_file;
//    file += test_file;
//    inserted_file += test_file_description;
//
//    file.insert(0, inserted_file);
//    ASSERT_EQ(file.to_string(), test_file_description + test_file);
//
//    file_t file2;
//    file2 += test_file;
//    auto lines_num =
//        std::count_if(
//            test_file.cbegin(),
//            test_file.cend(),
//            [](char symbol) { return symbol == '\n'; });
//    file2.insert(lines_num, inserted_file);
//    ASSERT_EQ(file2.to_string(), test_file + test_file_description);
//
//    file_t file3;
//    file3 +=
//R"(String 1
//String 2
//String 3
//)";
//    file_t inserted_file3;
//    inserted_file3 += "String 4";
//    file3.insert(1, inserted_file3);
//    ASSERT_EQ(file3.to_string(), std::string(
//R"(String 1
//String 4
//String 2
//String 3
//)"));
//}
