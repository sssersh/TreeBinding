
#ifndef _I_FILES_PROVIDER_H_
#define _I_FILES_PROVIDER_H_

namespace one_header_gen
{

class i_files_provider_t
{
public:
    virtual ~i_files_provider_t() = default;
    virtual file_ptr_t get_out_file() const = 0;
    virtual file_ptr_t get_input_file(fs::path path) = 0;
    virtual std::vector<file_ptr_t> get_all_input_files() const = 0;
};

using i_files_provider_ptr_t = std::shared_ptr<i_files_provider_t>;

} // namespace one_header_gen

#endif // _I_FILES_PROVIDER_H_
