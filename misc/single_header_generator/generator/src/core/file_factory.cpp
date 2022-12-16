
#include "file.h"
#include "file_factory.h"

namespace one_header_gen
{

i_file_factory_u_ptr_t create_file_factory()
{
    return std::make_unique<file_factory_t>();
}

i_file_u_ptr_t file_factory_t::create(const std::string &file_path)
{
    return std::make_unique<file_t>(file_path);
}


} // namespace one_header_gen
