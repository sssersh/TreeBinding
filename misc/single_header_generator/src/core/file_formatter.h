
#ifndef _FILE_FORMATTER_H_
#define _FILE_FORMATTER_H_

#include "i_file_formatter.h"

namespace one_header_gen
{

class file_formatter_t : public i_file_formatter_t
{
public:
    void delete_file_description(LinesContainer& lines) override;
    void move_includes(LinesContainer& lines) override;
    void replace_all_occurancies_in_single_line(
            LinesContainer& lines
        , const std::string &pattern
        , const std::string &replacer) override;
    void delete_include(
            LinesContainer& lines
        , const std::string& path
    ) override;
    void delete_include_guards(LinesContainer& lines) override;
    void preprocess_file(LinesContainer &preprocessed_file,
                                 std::unordered_map<std::string, LinesContainer> included_files) override;
};

} // namespace one_header_gen

#endif // _FILE_FORMATTER_H_
