
#ifndef _I_FILE_FORMATTER_H_
#define _I_FILE_FORMATTER_H_

#include <vector>
#include <string>

namespace one_header_gen
{

using LinesContainer = std::vector<std::string>;

class i_file_formatter_t
{
public:
    virtual ~i_file_formatter_t() = default;
    virtual void delete_file_description(LinesContainer& lines) = 0;
    virtual void move_includes(LinesContainer& lines)  = 0;
    virtual void replace_all_occurancies_in_single_line(
            LinesContainer& lines
        , const std::string &pattern
        , const std::string &replacer) = 0;
    virtual void delete_include(
            LinesContainer& lines
        , const std::string& path
    ) = 0;
    virtual void delete_include_guards(LinesContainer& lines) = 0;
    virtual void preprocess_file(LinesContainer &preprocessed_file,
                                 std::unordered_map<std::string, LinesContainer> included_files) = 0;
};

using i_file_formatter_ptr_t = std::shared_ptr<i_file_formatter_t>;

} // namespace one_header_gen

#endif // _I_FILE_FORMATTER_H_
