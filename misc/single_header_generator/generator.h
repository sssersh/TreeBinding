
#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include <set>

#include "file.h"

namespace one_header_gen
{

/*
* \param[in] root_dir          Path to creolization library root directory
* \param[in] project_name      Name of project
* \param[in] src_dir_name       Name of directory with creolization library sources
* \param[in] srcMainFileName  Names of creolization library main header
* \param[in] out_dir_name       Output directory name
* \param[in] templateOutFile  Template of out file
* \param[in] contentLineIndex Index of line, where will be insert generated file
 */
struct generator_info_t
{
    fs::path root_dir;
    std::string project_name;
    std::string src_dir_name;
    std::string src_main_file_name;
    std::string out_dir_name;
    fs::path template_out_file_path;
};

/*!
 * \brief Generator implementation
 */
struct generator_t
{
    generator_t(const generator_info_t &params);
    void generate();

    void prepare_out_dir_and_file() const;

    void readSrcFiles();
    void deleteIncludeMainFile();
    void preprocessFile(file_t &file, std::set<std::string> &&alreadyIncludedFiles = {});
    void deleteIncludeGuards();
    file_t insertOutFileInTemplate();

    fs::path                 root_dir         ; /*!< Path to creolization library root directory         */
    std::string              project_name     ; /*!< Name of project                                     */
    std::string              src_dir_name      ; /*!< Name of directory with creolization library sources */
    std::vector<std::string> src_files_names   ; /*!< Names of creolization library sources (first file used as
                                                    main file, others - just single_include main file and
                                                    and redefine macro from main file) */
    fs::path                 outDirPath      ; /*!< Output directory name */
    fs::path                 outFilePath     ; /*!< Path to out file */
    file_t                   outFile         ; /*!< Out file content */
    file_t                   templateOutFile ; /*!< Template of out file */

    static const std::size_t MAIN_FILE_INDEX = 0; /*!< Index of main header file in src_files_names array */
};

} // one_header_gen

#endif // _GENERATOR_H_
