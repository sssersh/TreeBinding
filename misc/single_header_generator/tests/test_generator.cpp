
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

#include "generator.h"

using namespace one_header_gen;

class generator_test : public ::testing::Test
{
public:
    void SetUp() override
    {
        generator = std::make_shared<generator_t>(info);
    }

    void TearDown() override
    {

    }

protected:
    struct generator_info_t info =
    {
        "test_root_dir",
        "test_project_name",
        "test_src_dir_name",
        "file.cpp",
        "test_out_dir",
        "test_template_file"
    };
    fs::path main_file_path = info.out_dir_name / fs::path(info.src_main_file_name);

    std::shared_ptr<generator_t> generator;
};

TEST_F(generator_test, prepare_out_dir_and_file)
{
    fs::create_directories(info.out_dir_name);
    std::fstream file;
    file.open(main_file_path);
    file.close();
    ASSERT_TRUE(fs::exists(main_file_path));

    ASSERT_NO_THROW(generator->prepare_out_dir_and_file());

    fs::remove_all(info.out_dir_name);
}