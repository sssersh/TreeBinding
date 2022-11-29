
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <filesystem>

namespace fs = std::filesystem;

#include "files_provider.h"

using namespace one_header_gen;

class fs_interactor_mock_t : public i_fs_interactor_t
{
public:
    MOCK_METHOD(std::vector<fs::path>, get_files_list, (const fs::path& directory), (override));
    MOCK_METHOD(std::vector<fs::path>, get_files_list_recursively, (const fs::path& directory), (override));
    MOCK_METHOD(std::vector<std::string>, read_file, (const fs::path& file_path), (override));
    MOCK_METHOD(void, write_file, (const std::vector<std::string>& content, const fs::path& file_path), (override));
};

using fs_interactor_mock_ptr_t = std::shared_ptr<fs_interactor_mock_t>;

class file_factory_mock_t : public i_file_factory_t
{
public:
    MOCK_METHOD(i_file_ptr_t, create, (const std::string& file_path), (override));
};

using file_factory_mock_ptr_t = std::shared_ptr<file_factory_mock_t>;

class files_provider_test_t : public ::testing::Test
{
public:
    void SetUp() override
    {
//        fs::create_directories(input_dir / input_file2_path.parent_path());
//        std::ofstream {input_dir / input_file3_path};
//        std::ofstream {input_dir / input_file1_path};
//        std::ofstream {input_dir / input_file2_path};
//        std::ofstream {template_out_file_path };

//        files_provider = create_files_provider();

        fs_interactor_mock = std::make_shared<fs_interactor_mock_t>();
        file_factory_mock = std::make_shared<file_factory_mock_t>();
    }

    void TearDown() override
    {
//        fs::remove_all(input_dir);
//        fs::remove_all(output_dir);
    };

    const fs::path input_dir = "test_input_dir";
    const fs::path input_file1_path = "file1.h";
    const fs::path input_file2_path = "details/file2.h";
    const fs::path input_file3_path = "test_file.cpp";
    const fs::path output_dir = "test_out_dir";
    const fs::path template_out_file_path = "test_template_file.in";

    i_files_provider_ptr_t files_provider;
    fs_interactor_mock_ptr_t fs_interactor_mock;
    file_factory_mock_ptr_t file_factory_mock;

    i_files_provider_ptr_t create_files_provider()
    {
        return std::make_shared<files_provider_t>(
                  input_dir
                , output_dir
                , template_out_file_path
                , fs_interactor_mock
                , file_factory_mock
        );
    };
};

TEST_F(files_provider_test_t, no_input_files)
{
    EXPECT_CALL(*fs_interactor_mock, get_files_list())
        .WillRepeatedly(decltype(fs_interactor_mock->get_files_list()){});

    ASSERT_ANY_THROW(create_files_provider());
}

//TEST_F(files_provider_test_t, no_input_files)
//{
//    fs::remove_all(input_dir);
//    fs::remove(template_out_file_path);
//    ASSERT_ANY_THROW(create_files_provider());
//
//    fs::create_directories(input_dir);
//    ASSERT_ANY_THROW(create_files_provider());
//
//    std::ofstream {input_dir / input_file3_path};
//    ASSERT_ANY_THROW(create_files_provider());
//
//    std::ofstream {template_out_file_path };
//    ASSERT_NO_THROW(create_files_provider());
//
//    std::ofstream {input_dir / input_file2_path};
//    ASSERT_NO_THROW(create_files_provider());
//}

//TEST_F(files_provider_test_t, prepare_out_dir_and_file)
//{
//    ASSERT_TRUE(fs::exists(files_provider->get_out_file()->get_path().parent_path()));
//}

//TEST_F(files_provider_test_t, get_input_file)
//{
//    auto file1 = files_provider->get_input_file(input_dir / input_file1_path);
//    auto file2 = files_provider->get_input_file(input_dir / input_file2_path);
//    auto file3 = files_provider->get_input_file(input_dir / input_file3_path);
//
//    ASSERT_TRUE(file1);
//    ASSERT_TRUE(file2);
//    ASSERT_TRUE(file3);
//
//    // TODO
////    ASSERT_EQ(*file1, file_t(input_file1_path));
////    ASSERT_EQ(*file2, file_t(input_file2_path));
////    ASSERT_EQ(*main_file, file_t(main_file_name));
//}


//TEST_F(files_provider_test_t, get_all_input_files)
//{
//    auto input_files = files_provider->get_all_input_files();
//
//    ASSERT_EQ(input_files.size(), (decltype(input_files)::size_type)3);
//
//    auto file1 = std::find_if(
//        input_files.begin(),
//        input_files.end(),
//        [&](file_ptr_t file)
//        {
//            return file->get_path() == input_dir / input_file1_path;
//        });
//
//    auto file2 = std::find_if(
//        input_files.cbegin(),
//        input_files.cend(),
//        [&](file_ptr_t file)
//        {
//            return file->get_path() == input_dir / input_file2_path;
//        });
//
//    auto file3 = std::find_if(
//        input_files.cbegin(),
//        input_files.cend(),
//        [&](file_ptr_t file)
//        {
//            return file->get_path() == input_dir / input_file3_path;
//        });
//
//    ASSERT_NE(file1, input_files.cend());
//    ASSERT_NE(file2, input_files.cend());
//    ASSERT_NE(file3, input_files.cend());
//
//    // TODO
////    ASSERT_EQ(*file1, file_t(input_file1_path));
////    ASSERT_EQ(*file2, file_t(input_file2_path));
////    ASSERT_EQ(*main_file, file_t(main_file_name));
//}


//TEST_F(files_provider_test_t, get_public_input_files)
//{
//    auto public_input_files = files_provider->get_public_input_files();
//
//    ASSERT_EQ(public_input_files.size(), (decltype(public_input_files)::size_type)2);
//
//    // TODO
////    ASSERT_EQ(public_input_files[0], file_t(input_file1_path));
//}
