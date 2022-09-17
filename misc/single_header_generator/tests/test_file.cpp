
#include "file.h"

#include <gtest/gtest.h>

#include <memory>

using namespace one_header_gen;

class generator_file_test : public testing::Test
{
public:
    const std::string& test_file_name()
    {
        static const std::string str = "test_out_file.txt";
        return str;
    }

    std::string test_file =
R"(String 1
String two
String three
)";

    std::string test_file_description =
R"(/*
 \file Filename
 \desc Descrioption
*/
)";

};

TEST_F(generator_file_test, create_file)
{
    ASSERT_NO_THROW(file_t());
    ASSERT_NO_THROW(file_t(generator_file_test::test_file_name()));

    file_t empty_file;
    ASSERT_TRUE(empty_file.to_string().empty());

    std::ofstream raw_file {test_file_name()};
    raw_file << test_file;

    file_t file {test_file_name()};
    ASSERT_EQ(file.to_string(), test_file);
}

TEST_F(generator_file_test, add_operator)
{
    file_t file;
    file += test_file;
    file += test_file_description;

    auto result = test_file + test_file_description;
    ASSERT_EQ(result, file.to_string());

    file.clear();
    ASSERT_TRUE(file.to_string().empty());

    file_t file_2;
    file_2 += file;
    ASSERT_EQ(file.to_string(), file.to_string());
}

TEST_F(generator_file_test, write)
{
    file_t file;
    file += test_file;
    file.write(test_file_name());

    std::string raw_file_string;
    std::ifstream raw_file {test_file_name()};
    raw_file >> raw_file_string;

    ASSERT_EQ(file.to_string(), raw_file_string);
}

TEST_F(generator_file_test, insert)
{
    file_t file, inserted_file;
    file += test_file;
    inserted_file += test_file_description;

    file.insert(0, inserted_file);
    ASSERT_EQ(file.to_string(), test_file_description + test_file);
    file.clear();

    file += test_file;
    auto lines_num = test_file.count() ???
    file.insert(lines_num, inserted_file);
    ASSERT_EQ(file.to_string(), test_file + test_file_description);
    file.clear();

    file +=
R"(
String 1
String 2
String 3
)";
    inserted_file.clear();
    inserted_file += "String 4";
    file.insert(1, inserted_file);
    ASSERT_EQ(file,
R"(
String 1
String 4
String 2
String 3
)");
}

TEST_F(generator_file_test, delete_description)
{
    file_t file;

    file += test_file_description;
    file += test_file;
    file.delete_file_description();
    ASSERT_EQ(file.to_string(), test_file);
    file.clear();

    file += test_file;
    file += test_file_description;
    file.delete_file_description();
    ASSERT_EQ(file.to_string(), test_file);
    file.clear();

    file += test_file;
    file += test_file_description;
    file += test_file;
    file.delete_file_description();
    ASSERT_EQ(file.to_string(), test_file + test_file);
}

TEST_F(generator_file_test, move_includes)
{
    file_t file;
    std::string include_str = "#include <test>";

    file += test_file
    file += include_str;
    file += test_file
    file += include_str;
    file.move_includes();

    auto file_str = file.to_string();
    file_str.resize(2 * include_str.size() + sizeof('\n'));
    ASSERT_EQ(file_str, include_str + '\n' + include_str);
}

TEST_F(generator_file_test, replace_all_occurancies)
{
    TODO
}