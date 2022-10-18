
#include <gtest/gtest.h>

#include "utils.h"

using namespace one_header_gen::utils;

TEST(utils, is_begin_of_doxygen_comment)
{
    ASSERT_TRUE(is_begin_of_doxygen_comment("/*!"));
    ASSERT_TRUE(is_begin_of_doxygen_comment("    /*!   "));
    ASSERT_TRUE(is_begin_of_doxygen_comment("/**"));
    ASSERT_TRUE(is_begin_of_doxygen_comment("    /**   "));
    ASSERT_TRUE(is_begin_of_doxygen_comment("/*! Text"));
    ASSERT_TRUE(is_begin_of_doxygen_comment("/** **Text"));

    ASSERT_FALSE(is_begin_of_doxygen_comment(""));
    ASSERT_FALSE(is_begin_of_doxygen_comment("text"));
    ASSERT_FALSE(is_begin_of_doxygen_comment("/* **Text"));
}

TEST(utils, is_doxygen_file_description)
{
    ASSERT_TRUE(is_doxygen_file_description("@file"));
    ASSERT_TRUE(is_doxygen_file_description(" * @file"));
    ASSERT_TRUE(is_doxygen_file_description(" * @file file"));
    ASSERT_TRUE(is_doxygen_file_description("\\file"));
    ASSERT_TRUE(is_doxygen_file_description(" * \\file"));
    ASSERT_TRUE(is_doxygen_file_description(" * \\file file"));

    ASSERT_FALSE(is_doxygen_file_description(""));
    ASSERT_FALSE(is_doxygen_file_description("text"));
    ASSERT_FALSE(is_doxygen_file_description("file"));
    ASSERT_FALSE(is_doxygen_file_description("\\ file"));
    ASSERT_FALSE(is_doxygen_file_description("\\ file"));
    ASSERT_FALSE(is_doxygen_file_description("@ file"));
}

TEST(utils, is_end_of_comment)
{
    ASSERT_TRUE(is_end_of_comment("*/"));
    ASSERT_TRUE(is_end_of_comment("  */"));
    ASSERT_TRUE(is_end_of_comment("text*/    "));

    ASSERT_FALSE(is_end_of_comment(""));
    ASSERT_FALSE(is_end_of_comment("text"));
    ASSERT_FALSE(is_end_of_comment("*"));
    ASSERT_FALSE(is_end_of_comment("/"));
    ASSERT_FALSE(is_end_of_comment("* /"));
}
