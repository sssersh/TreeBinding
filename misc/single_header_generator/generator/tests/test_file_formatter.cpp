
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "file_formatter.h"

using namespace one_header_gen;

class file_mock_t : public i_file_t
{
public:
    MOCK_METHOD(std::vector<std::string>&, get_lines, (), (override));
    MOCK_METHOD(const std::string_view, get_path, (), (override, const));
};

using file_mock_ptr_t = std::shared_ptr<file_mock_t>;

class file_formatter_test : public testing::Test
{
public:
    void SetUp() override
    {
        file = std::make_shared<file_mock_t>();
        file_formatter = std::make_shared<file_formatter_t>(file);

        EXPECT_CALL(*file, get_lines())
                .WillRepeatedly(::testing::ReturnRef(file_lines));
    }

    void TearDown() override
    {
        file_lines.clear();
        reference_file_lines.clear();
    }

    i_file_formatter_ptr_t file_formatter;
    file_mock_ptr_t file;

    std::vector<std::string> file_lines;
    std::vector<std::string> reference_file_lines;

    const std::vector<std::string> test_file_content =
    {
          "String 1"
        , "String two"
        , "String three"
    };

    const std::vector<std::string> test_file_description =
    {
           "/**"
         , R"(\file Filename)"
         , R"(\desc Description)"
         , "*/"
    };
};

TEST_F(file_formatter_test, delete_description)
{
    std::copy(
        test_file_description.cbegin(),
        test_file_description.cend(),
        std::back_inserter(file_lines));
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file_lines));
    file_formatter->delete_file_description();
    ASSERT_EQ(file_lines, test_file_content);
    file_lines.clear();

    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file_lines));
    std::copy(
            test_file_description.cbegin(),
            test_file_description.cend(),
            std::back_inserter(file_lines));
    file_formatter->delete_file_description();
    ASSERT_EQ(file_lines, test_file_content);
    file_lines.clear();

    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file_lines));
    std::copy(
            test_file_description.cbegin(),
            test_file_description.cend(),
            std::back_inserter(file_lines));
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file_lines));

    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file_lines));
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file_lines));

    file_formatter->delete_file_description();
    ASSERT_EQ(file_lines, reference_file_lines);
}

TEST_F(file_formatter_test, move_includes_to_top)
{
    std::string include_str1 = "#include <test1>";
    std::string include_str2 = "#include <test2>";

    reference_file_lines.push_back(include_str1);
    reference_file_lines.push_back(include_str2);
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file_lines));
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file_lines));

    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file_lines));
    file_lines.push_back(include_str1);
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file_lines));
    file_lines.push_back(include_str1);
    file_lines.push_back(include_str2);

    file_formatter->move_includes_to_top();

    ASSERT_EQ(file_lines, reference_file_lines);
}

TEST_F(file_formatter_test, delete_includes)
{
    std::string include_path_1 = "path/test1.h";
    std::string include_str1 = "#include \"" + include_path_1 + "\"";
    std::string include_str2 = "#include \"test2\"";

    file_lines.push_back(include_str2);
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file_lines));
    file_lines.push_back(include_str1);
    file_lines.push_back(include_str2);
    file_lines.push_back(include_str1);
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file_lines));
    file_lines.push_back(include_str1);
    file_lines.push_back(include_str2);
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file_lines));

    reference_file_lines.push_back(include_str2);
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file_lines));
    reference_file_lines.push_back(include_str2);
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file_lines));
    reference_file_lines.push_back(include_str2);
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file_lines));

    file_formatter->delete_include(include_path_1);

    ASSERT_EQ(file_lines, reference_file_lines);
}

TEST_F(file_formatter_test, replace_all_occurancies_in_single_line)
{
    std::string str1 = "NotRemoved ForReplace NotRemoved";
    std::string str2 = "NotRemoved Replaced NotRemoved";

    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file_lines));
    file_lines.push_back(str1);
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file_lines));
    file_lines.push_back(str2);
    file_lines.push_back(str1);

    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file_lines));
    reference_file_lines.push_back(str2);
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file_lines));
    reference_file_lines.push_back(str2);
    reference_file_lines.push_back(str2);

    file_formatter->replace_all_occurancies_in_single_line("ForReplace", "Replaced");

    ASSERT_EQ(file_lines, reference_file_lines);
}

TEST_F(file_formatter_test, delete_include_guards_positive)
{
    file_lines.push_back("#pragma once");
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file_lines));
    file_lines.push_back("#ifndef _TEST_");
    file_lines.push_back("#define _TEST_");
    file_lines.push_back("#endif // _TEST_");
    file_lines.push_back("#ifndef _TEST_");
    file_lines.push_back("#define _TEST_");
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file_lines));
    file_lines.push_back("#ifndef _TEST_2_");
    file_lines.push_back("#define _TEST_2_");
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file_lines));
    file_lines.push_back("#ifndef _TEST_3_");
    file_lines.push_back("#define _TEST_3_");
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(file_lines));
    file_lines.push_back("#endif // _TEST_3_");
    file_lines.push_back("#endif /* _TEST_2_ */");
    file_lines.push_back("#endif // _TEST_");

    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file_lines));
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file_lines));
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file_lines));
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file_lines));

    file_formatter->delete_include_guards();

    ASSERT_EQ(file_lines, reference_file_lines);
}

TEST_F(file_formatter_test, preprocess_file)
{
    std::unordered_map<std::string, i_file_ptr_t> include_files;
    std::string included_file1_path = "test/path/file.h";
    std::string included_file2_path = "test/path/file_2_nested.h";
    std::string included_file3_path = "test/path/file_3.h";

    file_lines.push_back("#include \"" + included_file1_path + "\"");
    std::copy(
            test_file_description.cbegin(),
            test_file_description.cend(),
            std::back_inserter(file_lines));
    file_lines.push_back("#include \"" + included_file3_path + "\"");

    std::vector<std::string> include_file_1_lines;
    include_file_1_lines.push_back("#include \"" + included_file2_path + "\"");
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(include_file_1_lines));

    std::vector<std::string> include_file_2_lines;
    std::copy(
            test_file_description.cbegin(),
            test_file_description.cend(),
            std::back_inserter(include_file_2_lines));
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(include_file_2_lines));

    std::vector<std::string> include_file_3_lines;
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(include_file_3_lines));

    auto include_file_1 = std::make_shared<file_mock_t>();
    auto include_file_2 = std::make_shared<file_mock_t>();
    auto include_file_3 = std::make_shared<file_mock_t>();

    EXPECT_CALL(*include_file_1, get_lines())
            .WillRepeatedly(::testing::ReturnRef(include_file_1_lines));
    EXPECT_CALL(*include_file_2, get_lines())
            .WillRepeatedly(::testing::ReturnRef(include_file_2_lines));
    EXPECT_CALL(*include_file_3, get_lines())
            .WillRepeatedly(::testing::ReturnRef(include_file_3_lines));

    include_files =
    {
          { included_file1_path, include_file_1 }
        , { included_file2_path, include_file_2 }
        , { included_file3_path, include_file_3 }
    };

    std::copy(
            test_file_description.cbegin(),
            test_file_description.cend(),
            std::back_inserter(reference_file_lines));
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file_lines));

    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file_lines));
    std::copy(
            test_file_description.cbegin(),
            test_file_description.cend(),
            std::back_inserter(reference_file_lines));
    std::copy(
            test_file_content.cbegin(),
            test_file_content.cend(),
            std::back_inserter(reference_file_lines));

    file_formatter->preprocess_file(include_files);

    ASSERT_EQ(file_lines, reference_file_lines);
}
