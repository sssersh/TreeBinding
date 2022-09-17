
#include "utils.h"

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

void replace_all_occurancies(File &file, const std::string &pattern, const std::string &replacer)
{
    const auto r = std::regex ( pattern  );
    std::smatch match;

    for(auto &line : file.lines)
    {
        if(std::regex_match(line, match, r)) {
            std::ostringstream ostream;
            std::regex_replace(
                    std::ostreambuf_iterator<char>(ostream),
                    line.begin(),
                    line.end(),
                    r,
                    replacer);
            line = ostream.str();
        }
    }
}

} // namespace one_header_gen::utils
