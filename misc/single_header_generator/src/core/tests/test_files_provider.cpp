
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

#include "files_provider.h"

using namespace one_header_gen;

class files_provider_test_t : public ::testing::Test
{
public:
    void SetUp() override
    {
        fs::create_directories(input_dir / input_file2_path.parent_path());
        std::ofstream {input_dir / input_file3_path};
        std::ofstream {input_dir / input_file1_path};
        std::ofstream {input_dir / input_file2_path};
        std::ofstream {template_out_file_path };

        files_provider = create_files_provider();
    }

    void TearDown() override
    {
        fs::remove_all(input_dir);
        fs::remove_all(output_dir);
    };

    const fs::path input_dir = "test_input_dir";
    const fs::path input_file1_path = "file1.h";
    const fs::path input_file2_path = "nested/file2.h";
    const fs::path input_file3_path = "test_file.cpp";
    const fs::path output_dir = "test_out_dir";
    const fs::path template_out_file_path = "test_template_file.in";


    files_provider_ptr_t files_provider;

    files_provider_ptr_t create_files_provider()
    {
        return std::make_shared<files_provider_t>(
                input_dir
                , output_dir
                , template_out_file_path
        );
    };
};

TEST_F(files_provider_test_t, base)
{
    fs::remove_all(input_dir);
    fs::remove(template_out_file_path);
    ASSERT_ANY_THROW(create_files_provider());

    fs::create_directories(input_dir);
    ASSERT_ANY_THROW(create_files_provider());

    std::ofstream {input_dir / input_file3_path};
    ASSERT_ANY_THROW(create_files_provider());

    std::ofstream {template_out_file_path };
    ASSERT_NO_THROW(create_files_provider());

    std::ofstream {input_dir / input_file2_path};
    ASSERT_NO_THROW(create_files_provider());
}

TEST_F(files_provider_test_t, prepare_out_dir_and_file)
{
    ASSERT_TRUE(fs::exists(files_provider->get_out_file()->get_path().parent_path()));
}

TEST_F(files_provider_test_t, get_input_file)
{
    auto file1 = files_provider->get_input_file(input_dir / input_file1_path);
    auto file2 = files_provider->get_input_file(input_dir / input_file2_path);
    auto file3 = files_provider->get_input_file(input_dir / input_file3_path);


    ASSERT_TRUE(file1);
    ASSERT_TRUE(file2);
    ASSERT_TRUE(file3);

    // TODO
//    ASSERT_EQ(*file1, file_t(input_file1_path));
//    ASSERT_EQ(*file2, file_t(input_file2_path));
//    ASSERT_EQ(*main_file, file_t(main_file_name));
}


TEST_F(files_provider_test_t, get_all_input_files)
{
    auto input_files = files_provider->get_all_input_files();

    ASSERT_EQ(input_files.size(), (decltype(input_files)::size_type)3);

    auto file1 = std::find_if(
        input_files.begin(),
        input_files.end(),
        [&](file_ptr_t file)
        {
            return file->get_path() == input_dir / input_file1_path;
        });

    auto file2 = std::find_if(
        input_files.cbegin(),
        input_files.cend(),
        [&](file_ptr_t file)
        {
            return file->get_path() == input_dir / input_file2_path;
        });

    auto file3 = std::find_if(
        input_files.cbegin(),
        input_files.cend(),
        [&](file_ptr_t file)
        {
            return file->get_path() == input_dir / input_file3_path;
        });

    ASSERT_NE(file1, input_files.cend());
    ASSERT_NE(file2, input_files.cend());
    ASSERT_NE(file3, input_files.cend());

    // TODO
//    ASSERT_EQ(*file1, file_t(input_file1_path));
//    ASSERT_EQ(*file2, file_t(input_file2_path));
//    ASSERT_EQ(*main_file, file_t(main_file_name));
}
