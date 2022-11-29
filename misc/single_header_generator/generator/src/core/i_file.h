
#ifndef _I_FILE_H_
#define _I_FILE_H_

#include <memory>
#include <vector>

namespace one_header_gen
{

class i_file_t
{
public:
    virtual ~i_file_t() = default;
    virtual std::vector<std::string>& get_lines() = 0;
    virtual const std::string_view get_path() const = 0;
};

using i_file_ptr_t = std::shared_ptr<i_file_t>;

} // namespace one_header_gen

#endif // _I_FILE_H_
