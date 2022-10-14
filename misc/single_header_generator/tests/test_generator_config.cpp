
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

#include "generator_config.h"

using namespace one_header_gen;

TEST(generator_config_test, base)
{
    const fs::path root_dir = "test_root_dir";
    const std::string_view project_name = "test_project_name";
    const std::string_view input_dir_name = "test_input_dir_name";
    const std::string_view main_file_name = "test_file.cpp";
    const std::string_view output_dir_name = "test_out_dir_name";
    const fs::path template_out_file_path = "test_root_dir/test_template_file.in";

    auto create_info = [&]() -> generator_config_t
    {
        return
            {
                root_dir
                , project_name
                , input_dir_name
                , main_file_name
                , output_dir_name
                , template_out_file_path
            };
    };

    fs::remove_all(root_dir);
    ASSERT_ANY_THROW(create_info());

    fs::create_directories(root_dir);
    ASSERT_ANY_THROW(create_info());

    fs::create_directories(root_dir / input_dir_name / project_name);
    ASSERT_ANY_THROW(create_info());

    std::ofstream {root_dir / input_dir_name / project_name / main_file_name};
    ASSERT_ANY_THROW(create_info());

    std::ofstream {template_out_file_path };
    ASSERT_NO_THROW(create_info());

    fs::remove_all(root_dir);
}
