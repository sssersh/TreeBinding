
#ifndef _I_FILE_FORMATTER_H_
#define _I_FILE_FORMATTER_H_

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
//#include <filesystem>

#include "i_file.h"

//namespace fs = std::filesystem;

namespace one_header_gen
{

class i_file_formatter_t
{
public:
    virtual ~i_file_formatter_t() = default;
    virtual void delete_file_description() = 0;
    virtual void move_includes_to_top()  = 0;
    virtual void replace_all_occurancies_in_single_line(
          const std::string &pattern
        , const std::string &replacer) = 0;
    virtual void delete_include(const std::string& path) = 0;
    virtual void delete_include_guards() = 0;
    virtual void preprocess_file(
        std::unordered_map<std::string, i_file_ptr_t> included_files) = 0;
};

using i_file_formatter_ptr_t = std::shared_ptr<i_file_formatter_t>;

} // namespace one_header_gen

#endif // _I_FILE_FORMATTER_H_
