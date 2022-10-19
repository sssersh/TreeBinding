
#include "utils.h"

#include <regex>

namespace one_header_gen::utils
{

bool is_begin_of_doxygen_comment(const std::string &str)
{
    static const auto r1 = std::regex ( R"(.*/\*!.*)" ), r2 = std::regex ( R"(.*/\*\*.*)" );
    return std::regex_match(str, r1) || std::regex_match(str, r2);
}

bool is_doxygen_file_description(const std::string &str)
{
    static const auto r1 = std::regex ( R"(.*\\file.*)" ), r2 = std::regex ( R"(.*@file.*)" );
    return std::regex_match(str, r1) || std::regex_match(str, r2);
}

bool is_end_of_comment(const std::string &str)
{
    static const auto r = std::regex ( R"(.*\*/.*)" );
    return std::regex_match(str, r);
}

} // namespace one_header_gen::utils
