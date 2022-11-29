
#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include <set>
#include <cassert>

#include "i_file_formatter.h"
#include "file.h"
#include "logger.h"
#include "i_files_provider.h"
#include "i_generator.h"

namespace one_header_gen
{

/*!
 * \brief Generator implementation
 */
class generator_t : public i_generator_t
{
public:
    generator_t(
          i_files_provider_ptr_t files_provider
        , file_ptr_t result_file
        , i_file_formatter_ptr_t result_file_formatter
        , generator_config_t params);
    void generate() override;

private:
    void copy_public_includes_to_output_file();
    void delete_include_of_public_inputs();

    generator_config_t config;
    i_files_provider_ptr_t files_provider;
    file_ptr_t result_file; /*!< Result file */
    i_file_formatter_ptr_t result_file_formatter;
};

} // one_header_gen

#endif // _GENERATOR_H_
