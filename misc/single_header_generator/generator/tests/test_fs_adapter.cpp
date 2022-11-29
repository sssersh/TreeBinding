
#include "i_fs_adapter.h"

#include <gtest/gtest.h>

#include <fstream>

using namespace one_header_gen;

class fs_adapter_test_t : public ::testing::Test
{
public:
    void SetUp() override
    {
        fs::create_directories(file2_path.parent_path());
        std::ofstream file1{file1_path};
        std::ofstream file2{file2_path};
        std::ofstream file3{file3_path};

        for (const auto& line : test_file_content_1)
        {
            file1 << line << "\n";
        }
        for (const auto& line : test_file_content_2)
        {
            file2 << line << "\n";
        }
        for (const auto& line : test_file_content_3)
        {
            file3 << line << "\n";
        }

        fs_adapter = create_fs_adapter();
    }

    void TearDown() override
    {
        fs::remove_all(dir);
    }

    i_fs_adapter_ptr_t fs_adapter;

    const fs::path dir        = "test_input_dir";
    const fs::path file1_path = "test_input_dir/file1.h";
    const fs::path file2_path = "test_input_dir/details/file2.h";
    const fs::path file3_path = "test_input_dir/test_file.cpp";

    const std::vector<std::string> test_file_content_1 =
    {
          "String 1"
        , "String two"
        , "String three"
    };

    const std::vector<std::string> test_file_content_2 =
    {
          "String 5"
        , "String 2"
        , "String 9"
    };

    const std::vector<std::string> test_file_content_3 =
    {
          "Test string"
        , "FS interactor test"
        , "TeSt"
    };
};

TEST_F(fs_adapter_test_t, get_files_list_positive)
{
    auto files_list = fs_adapter->get_files_list(dir);

    ASSERT_EQ((int)files_list.size(), 2);

    auto contain_file1_path =
        files_list[0] == file1_path ||
        files_list[1] == file1_path;

    auto contain_file3_path =
            files_list[0] == file3_path ||
            files_list[1] == file3_path;

    ASSERT_TRUE(contain_file1_path);
    ASSERT_TRUE(contain_file3_path);
}

TEST_F(fs_adapter_test_t, get_files_list_negative)
{
    ASSERT_ANY_THROW(fs_adapter->get_files_list(""));
    ASSERT_ANY_THROW(fs_adapter->get_files_list(dir / dir));
}

TEST_F(fs_adapter_test_t, get_files_list_recursively)
{
    auto files_list = fs_adapter->get_files_list_recursively(dir);

    ASSERT_EQ((int)files_list.size(), 3);

    auto contain_file1_path =
            files_list[0] == file1_path ||
            files_list[1] == file1_path ||
            files_list[2] == file1_path;

    auto contain_file2_path =
            files_list[0] == file2_path ||
            files_list[1] == file2_path ||
            files_list[2] == file2_path;

    auto contain_file3_path =
            files_list[0] == file3_path ||
            files_list[1] == file3_path ||
            files_list[2] == file3_path;

    ASSERT_TRUE(contain_file1_path);
    ASSERT_TRUE(contain_file2_path);
    ASSERT_TRUE(contain_file3_path);
}

TEST_F(fs_adapter_test_t, get_files_list_recursively_negative)
{
    ASSERT_ANY_THROW(fs_adapter->get_files_list_recursively(""));
    ASSERT_ANY_THROW(fs_adapter->get_files_list_recursively(dir / dir));
}

TEST_F(fs_adapter_test_t, read_file_positive)
{
    auto lines_file_1 = fs_adapter->read_file(file1_path);
    auto lines_file_2 = fs_adapter->read_file(file2_path);
    auto lines_file_3 = fs_adapter->read_file(file3_path);

    ASSERT_EQ(lines_file_1, test_file_content_1);
    ASSERT_EQ(lines_file_2, test_file_content_2);
    ASSERT_EQ(lines_file_3, test_file_content_3);
}

TEST_F(fs_adapter_test_t, read_file_negative)
{
    ASSERT_ANY_THROW(fs_adapter->read_file(""));
    ASSERT_ANY_THROW(fs_adapter->read_file(dir));
    ASSERT_ANY_THROW(fs_adapter->read_file(dir / file1_path));
}

TEST_F(fs_adapter_test_t, write_file_positive)
{
    fs_adapter->write_file(test_file_content_3, file1_path);
    fs_adapter->write_file(test_file_content_1, file2_path);
    ASSERT_NO_THROW(fs_adapter->write_file(std::vector<std::string>{}, file3_path));

    auto lines_file_1 = fs_adapter->read_file(file1_path);
    auto lines_file_2 = fs_adapter->read_file(file2_path);
    auto lines_file_3 = fs_adapter->read_file(file3_path);

    ASSERT_EQ(lines_file_1, test_file_content_3);
    ASSERT_EQ(lines_file_2, test_file_content_1);
    ASSERT_TRUE(lines_file_3.empty());
}

TEST_F(fs_adapter_test_t, write_file_negative)
{
    std::vector<std::string> lines;
    ASSERT_ANY_THROW(fs_adapter->write_file(lines, ""));
    ASSERT_ANY_THROW(fs_adapter->write_file(lines, dir));
}
