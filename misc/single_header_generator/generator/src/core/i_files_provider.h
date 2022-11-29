
#ifndef _I_FILES_PROVIDER_H_
#define _I_FILES_PROVIDER_H_

#include <vector>
#include <string>
//#include <filesystem>

#include "i_file.h"

//namespace fs = std::filesystem;

namespace one_header_gen
{

class i_files_provider_t
{
public:
    virtual ~i_files_provider_t() = default;
    virtual i_file_ptr_t get_out_file() const = 0;
    virtual i_file_ptr_t get_template_out_file() const = 0;
    virtual i_file_ptr_t get_input_file(const std::string& path) = 0;
    virtual std::vector<i_file_ptr_t> get_all_input_files() const = 0;
    virtual std::vector<i_file_ptr_t> get_public_input_files() const = 0;
};

using i_files_provider_ptr_t = std::shared_ptr<i_files_provider_t>;

i_files_provider_ptr_t create_files_provider(
      std::string input_dir
    , std::string out_dir
    , std::string template_out_file_path
);

} // namespace one_header_gen

#endif // _I_FILES_PROVIDER_H_
