
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
        , i_file_formatter_ptr_t file_formatter
        , generator_config_t params);
    void generate() override;

private:
    i_files_provider_ptr_t files_provider;
    i_file_formatter_ptr_t file_formatter;
    generator_config_t config;
//    file_t                   outFile         ; /*!< Out file content */
//    file_t                   templateOutFile ; /*!< Template of out file */

//    static const std::size_t MAIN_FILE_INDEX = 0; /*!< Index of main header file in src_files_names array */
};

} // one_header_gen

#endif // _GENERATOR_H_
