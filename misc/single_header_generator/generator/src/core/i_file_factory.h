
#ifndef _I_FILE_FACTORY_H_
#define _I_FILE_FACTORY_H_

#include "i_file.h"

namespace one_header_gen
{

class i_file_factory_t
{
public:
    virtual ~i_file_factory_t() = default;
    virtual i_file_u_ptr_t create(const std::string& file_path) = 0;
};

using i_file_factory_ptr_t = std::shared_ptr<i_file_factory_t>;
using i_file_factory_u_ptr_t = std::unique_ptr<i_file_factory_t>;

} // namespace one_header_gen

#endif // _I_FILE_FACTORY_H_
