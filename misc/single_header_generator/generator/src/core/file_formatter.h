
#ifndef _FILE_FORMATTER_H_
#define _FILE_FORMATTER_H_

#include "i_file_formatter.h"

namespace one_header_gen
{

class file_formatter_t : public i_file_formatter_t
{
public:
    file_formatter_t(i_editable_file_ptr_t file);
    void delete_file_description() override;
    void move_includes() override;
    void replace_all_occurancies_in_single_line(
          const std::string &pattern
        , const std::string &replacer) override;
    void delete_include(const std::string& path) override;
    void delete_include_guards() override;
    void preprocess_file(std::unordered_map<std::string, i_editable_file_ptr_t> included_files) override;

private:
    i_editable_file_ptr_t file;
};

} // namespace one_header_gen

#endif // _FILE_FORMATTER_H_
