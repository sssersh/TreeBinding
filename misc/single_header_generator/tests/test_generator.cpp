
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
        fs::remove_all(root_dir);

        fs::create_directories(root_dir / input_dir_name / project_name);

        std::ofstream input_main_file {root_dir / input_dir_name / project_name / main_file_name};
        std::ofstream input_file_1 { root_dir / input_dir_name / project_name / main_file_name};
        std::ofstream input_file_2 { root_dir / input_dir_name / project_name / main_file_name};
        std::ofstream template_out_file {template_out_file_path };

        main_file << main_file_content;
        input_file_1 << file_1_content;
        input_file_2 << file_2_content;

        config = std::make_shared<generator_config_t>(
              root_dir
            , project_name
            , input_dir_name
            , main_file_name
            , output_dir_name
            , template_out_file_path
        );

        generator = std::make_shared<generator_t>(*config);
    }

    void TearDown() override
    {
        fs::remove_all(root_dir);
    }

protected:
    const fs::path root_dir = "test_root_dir";
    const std::string_view project_name = "test_project_name";
    const std::string_view input_dir_name = "test_input_dir_name";
    const std::string_view main_file_name = "test_main_file.h";
    const std::string_view output_dir_name = "test_out_dir_name";
    const fs::path template_out_file_path = "test_root_dir/test_template_file.in";

    const std::vector<fs::path> input_files_paths = 
    {
           root_dir / "test_file_1.h"
        ,  root_dir / "test_nested_dir" / "test_file_2.h"
    };

    const std::string main_file_content = 
R"(
#include "test_file_1.h"
#include <vector>
#include "vector"
#include "test_nested_dir/test_file_2.h"
)";

    const std::string file_1_content = 
R"(
file 1 content
)";

    std::shared_ptr<generator_config_t> config;
    std::shared_ptr<generator_t> generator;
};

TEST_F(generator_test, read_src_files)
{
//    generator->prepare_out_dir_and_file();
//    ASSERT_NO_THROW

}
