
#ifndef _FILE_FACTORY_H_
#define _FILE_FACTORY_H_

#include "i_file_factory.h"

namespace one_header_gen
{

class file_factory_t : public i_file_factory_t
{
public:
    i_file_u_ptr_t create(const std::string& file_path) override;
};

i_file_factory_u_ptr_t create_file_factory();

} // namespace one_header_gen

#endif // _FILE_FACTORY_H_
