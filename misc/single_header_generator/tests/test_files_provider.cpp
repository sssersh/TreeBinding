
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

#include "files_provider.h"

using namespace one_header_gen;

class files_provider_test : public ::testing::Test
{
public:
    void SetUp() override
    {
        fs::create_directories(root_dir / input_dir_name / input_file2_path.parent_path()); // / project_name
        std::ofstream {root_dir / input_dir_name / main_file_name}; // / project_name
        std::ofstream {root_dir / input_dir_name / input_file1_path}; // / project_name
        std::ofstream {root_dir / input_dir_name / input_file2_path}; //  / project_name
        std::ofstream {template_out_file_path };
    }

    void TearDown() override
    {
        fs::remove_all(root_dir);
    };

    const fs::path root_dir = "test_root_dir";
//    const std::string_view project_name = "test_project_name";
    const std::string_view input_dir_name = "test_input_dir_name";
    const std::string_view main_file_name = "test_file.cpp";
    const std::string_view output_dir_name = "test_out_dir_name";
    const fs::path template_out_file_path = "test_root_dir/test_template_file.in";
    const fs::path input_file1_path = "file1.h";
    const fs::path input_file2_path = "nested/file2.h";

    files_provider_t create_file_structure()
    {
        return
            {
                  root_dir
//                , project_name
                , input_dir_name
                , main_file_name
                , output_dir_name
                , template_out_file_path
            };
    };
};

TEST_F(files_provider_test, base)
{
    fs::remove_all(root_dir);
    ASSERT_ANY_THROW(create_file_structure());

    fs::create_directories(root_dir);
    ASSERT_ANY_THROW(create_file_structure());

    fs::create_directories(root_dir / input_dir_name); // / project_name
    ASSERT_ANY_THROW(create_file_structure());

    std::ofstream {root_dir / input_dir_name / main_file_name}; // / project_name
    ASSERT_ANY_THROW(create_file_structure());

    std::ofstream {template_out_file_path };
    ASSERT_NO_THROW(create_file_structure());
}

TEST_F(files_provider_test, prepare_out_dir_and_file)
{
    ASSERT_TRUE(!fs::exists(create_file_structure().get_output_dir_path()));

    ASSERT_NO_THROW(create_file_structure().prepare_out_dir_and_file());

    ASSERT_TRUE(fs::exists(create_file_structure().get_output_dir_path()));
    ASSERT_TRUE(fs::exists(create_file_structure().get_out_file_path()));
}

TEST_F(files_provider_test, read_src_files)
{
    auto input_files = create_file_structure().get_input_files();

    std::set<fs::path> input_files_set =
    {
        input_files.cbegin(),
        input_files.cend()
    };

    ASSERT_NE(input_files_set.find(input_file1_path), input_files_set.cend());
    ASSERT_NE(input_files_set.find(input_file1_path), input_files_set.cend());
}
