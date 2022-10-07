
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

#include "generator.h"

using namespace one_header_gen;

//class generator_test : public ::testing::Test
//{
//public:
//    void SetUp() override
//    {
////        fs::create_directories(test_generator_info().input_dir_path());
////        // create main file
////        std::ofstream { main_file_path };
////
////        generator = std::make_shared<generator_t>(info);
//    }
//
//    void TearDown() override
//    {
////        fs::remove_all(info.root_dir);
//    }
//
//protected:
////    generator_info_t info = test_generator_info();
//
//    std::shared_ptr<generator_t> generator;
//};

//TEST(generator_create_test, empty_data)
//{
//    struct generator_info_t info = test_generator_info();
//
//    info.root_dir.clear();
//    ASSERT_ANY_THROW(generator_t{info});
//
//    info = test_generator_info();
//    info.project_name.clear();
//    ASSERT_ANY_THROW(generator_t{info});
//
//    info = test_generator_info();
//    info.input_dir_name.clear();
//    ASSERT_ANY_THROW(generator_t{info});
//
//    info = test_generator_info();
//    info.input_main_file_name.clear();
//    ASSERT_ANY_THROW(generator_t{info});
//
//    info = test_generator_info();
//    info.out_dir_name.clear();
//    ASSERT_ANY_THROW(generator_t{info});
//
//    info = test_generator_info();
//    info.template_out_file_path.clear();
//    ASSERT_ANY_THROW(generator_t{info});
//}

//TEST_F(generator_test, prepare_out_dir_and_file)
//{
//    fs::create_directories(info.out_dir_name);
//    ASSERT_TRUE(fs::exists(main_file_path));
//
//    ASSERT_NO_THROW(generator->prepare_out_dir_and_file());
//
//    fs::remove_all(info.out_dir_name);
//}