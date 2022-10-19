
#include <gtest/gtest.h>

#include "file_formatter.h"

using namespace one_header_gen;

class file_formatter_test : public testing::Test
{
public:
    void SetUp() override
    {
        file_formatter = std::make_shared<file_formatter_t>();
    }

    void TearDown() override
    {
        file.clear();
        reference_file.clear();
    }

    i_file_formatter_ptr_t file_formatter;
    LinesContainer file;
    LinesContainer reference_file;

    const LinesContainer test_file_content =
    {
          "String 1"
        , "String two"
        , "String three"
    };

    const LinesContainer test_file_description =
    {
           "/**"
         , R"(\file Filename)"
         , R"(\desc Description)"
         , "*/"
    };

};

TEST_F(file_formatter_test, delete_description)
{
    auto file_formatter = std::make_shared<file_formatter_t>();

    std::copy(
        test_file_description.cbegin(),
        test_file_description.cend(),
        std::back_inserter(file));
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file));
    file_formatter->delete_file_description(file);
    ASSERT_EQ(file, test_file_content);
    file.clear();

    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file));
    std::copy(
            test_file_description.cbegin(),
            test_file_description.cend(),
            std::back_inserter(file));
    file_formatter->delete_file_description(file);
    ASSERT_EQ(file, test_file_content);
    file.clear();

    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file));
    std::copy(
            test_file_description.cbegin(),
            test_file_description.cend(),
            std::back_inserter(file));
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file));

    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file));
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file));

    file_formatter->delete_file_description(file);
    ASSERT_EQ(file, reference_file);
}

TEST_F(file_formatter_test, move_includes)
{
    std::string include_str1 = "#include <test1>";
    std::string include_str2 = "#include <test2>";

    std::vector<std::string> reference_file;
    reference_file.push_back(include_str1);
    reference_file.push_back(include_str2);
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file));
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file));

    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file));
    file.push_back(include_str1);
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file));
    file.push_back(include_str1);
    file.push_back(include_str2);

    file_formatter->move_includes(file);

    ASSERT_EQ(file, reference_file);
}

TEST_F(file_formatter_test, delete_includes)
{
    std::string include_path_1 = "path/test1.h";
    std::string include_str1 = "#include \"" + include_path_1 + "\"";
    std::string include_str2 = "#include \"test2\"";

    file.push_back(include_str2);
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file));
    file.push_back(include_str1);
    file.push_back(include_str2);
    file.push_back(include_str1);
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file));
    file.push_back(include_str1);
    file.push_back(include_str2);
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file));

    std::vector<std::string> reference_file;
    reference_file.push_back(include_str2);
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file));
    reference_file.push_back(include_str2);
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file));
    reference_file.push_back(include_str2);
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file));

    file_formatter->delete_include(file, include_path_1);

    ASSERT_EQ(file, reference_file);
}

TEST_F(file_formatter_test, replace_all_occurancies_in_single_line)
{
    std::string str1 = "NotRemoved ForReplace NotRemoved";
    std::string str2 = "NotRemoved Replaced NotRemoved";

    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file));
    file.push_back(str1);
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file));
    file.push_back(str2);
    file.push_back(str1);

    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file));
    reference_file.push_back(str2);
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file));
    reference_file.push_back(str2);
    reference_file.push_back(str2);

    file_formatter->replace_all_occurancies_in_single_line(file, "ForReplace", "Replaced");

    ASSERT_EQ(file, reference_file);
}

TEST_F(file_formatter_test, delete_include_guards_positive)
{
    file.push_back("#pragma once");
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file));
    file.push_back("#ifndef _TEST_");
    file.push_back("#define _TEST_");
    file.push_back("#endif // _TEST_");
    file.push_back("#ifndef _TEST_");
    file.push_back("#define _TEST_");
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file));
    file.push_back("#ifndef _TEST_2_");
    file.push_back("#define _TEST_2_");
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file));
    file.push_back("#ifndef _TEST_3_");
    file.push_back("#define _TEST_3_");
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file));
    file.push_back("#endif // _TEST_3_");
    file.push_back("#endif /* _TEST_2_ */");
    file.push_back("#endif // _TEST_");

    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file));
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file));
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file));
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file));

    file_formatter->delete_include_guards(file);

    ASSERT_EQ(file, reference_file);
}

TEST_F(file_formatter_test, preprocess_file)
{
    std::unordered_map<std::string, LinesContainer> include_files;
    std::string included_file1_path = "test/path/file.h";
    std::string included_file2_path = "test/path/file_2_nested.h";
    std::string included_file3_path = "test/path/file_3.h";

    file.push_back("#include \"" + included_file1_path + "\"");
    std::copy(
            test_file_description.cbegin(),
            test_file_description.cend(),
            std::back_inserter(file));
    file.push_back("#include \"" + included_file3_path + "\"");

    LinesContainer include_file_1;
    include_file_1.push_back("#include \"" + included_file2_path + "\"");
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(include_file_1));

    LinesContainer include_file_2;
    std::copy(
            test_file_description.cbegin(),
            test_file_description.cend(),
            std::back_inserter(include_file_2));
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(include_file_2));

    LinesContainer include_file_3;
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(include_file_3));

    include_files =
    {
          { included_file1_path, include_file_1 }
        , { included_file2_path, include_file_2 }
        , { included_file3_path, include_file_3 }
    };

    std::copy(
            test_file_description.cbegin(),
            test_file_description.cend(),
            std::back_inserter(reference_file));
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file));

    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file));
    std::copy(
            test_file_description.cbegin(),
            test_file_description.cend(),
            std::back_inserter(reference_file));
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file));

    file_formatter->preprocess_file(file, include_files);

    ASSERT_EQ(file, reference_file);
}