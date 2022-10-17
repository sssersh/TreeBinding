
#ifndef _I_GENERATOR_H_
#define _I_GENERATOR_H_

#include <memory>

namespace one_header_gen
{

class i_generator_t
{
    ~i_generator_t() = default;
    virtual void generate() = 0;
};

using i_generator_ptr_t = std::shared_ptr<i_generator_t>;

i_generator_ptr_t create_generator();

} // namespace one_header_gen

#endif // _I_GENERATOR_H_
