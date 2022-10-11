
#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include <set>
#include <cassert>

#include "file.h"
#include "logger.h"
#include "generator_config.h"

namespace one_header_gen
{

/*!
 * \brief Generator implementation
 */
struct generator_t
{
    generator_t(const generator_config_t &params);
    void generate();

    void prepare_out_dir_and_file() const;

    void readSrcFiles();
    void deleteIncludeMainFile();
    void preprocessFile(file_t &file, std::set<std::string> &&alreadyIncludedFiles = {});
    void deleteIncludeGuards();
    file_t insertOutFileInTemplate();

    generator_config_t config;
    std::vector<std::string> src_files_names   ; /*!< Names of creolization library sources (first file used as
                                                    main file, others - just single_include main file and
                                                    and redefine macro from main file) */
    file_t                   outFile         ; /*!< Out file content */
    file_t                   templateOutFile ; /*!< Template of out file */

    static const std::size_t MAIN_FILE_INDEX = 0; /*!< Index of main header file in src_files_names array */
};

} // one_header_gen

#endif // _GENERATOR_H_
