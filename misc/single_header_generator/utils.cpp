
#include "utils.h"

#include <regex>

namespace one_header_gen::utils
{

bool is_begin_of_doxygen_comment(const std::string &str)
{
    static const auto r = std::regex ( R"(.*/\*!.*)" );
    return std::regex_match(str, r);
}

bool is_doxygen_description(const std::string &str)
{
    static const auto r = std::regex ( R"(.*\\file.*)" );
    return std::regex_match(str, r);
}

bool is_end_of_comment(const std::string &str)
{
    static const auto r = std::regex ( R"(.*\*/.*)" );
    return std::regex_match(str, r);
}

} // namespace one_header_gen::utils
