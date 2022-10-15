
#ifndef _GENERATOR_FIE_STUTURE_H_
#define _GENERATOR_FIE_STUTURE_H_

#include <vector>
#include <filesystem>

#include "logger.h"

namespace one_header_gen {

namespace fs = std::filesystem;

/*
* \param[in] root_dir          Path to creolization library root directory
* \param[in] project_name      Name of project
* \param[in] input_dir_name       Name of directory with creolization library sources
* \param[in] srcMainFileName  Names of creolization library main header
* \param[in] out_dir_name       Output directory name
* \param[in] templateOutFile  Template of out file
* \param[in] contentLineIndex Index of line, where will be insert generated file
 */
class files_structure_t {

    fs::path root_dir;  /*!< Path to creolization library root directory         */
    std::string_view project_name; /*!< Name of project                                     */
    fs::path input_dir; /*!< Directory with creolization library sources */
    std::string_view input_main_file_name;
    fs::path out_dir; /*!< Output directory path */
    fs::path template_out_file_path;
    fs::path out_file_path; /*!< Path to out file */
    std::vector<fs::path> input_files_paths   ; /*!< Names of creolization library sources (first file used as
                                                    main file, others - just single_include main file and
                                                    and redefine macro from main file) */

public:
    files_structure_t(
          fs::path root_dir
        , std::string_view project_name
        , std::string_view input_dir_name
        , std::string_view input_main_file_name
        , std::string_view out_dir_name
        , fs::path template_out_file_path
    );

    const fs::path &get_input_dir_path() const;
    const fs::path &get_output_dir_path() const;
    const fs::path &get_template_out_file_path() const;
    const fs::path &get_out_file_path() const;
    std::string_view get_project_name() const;
    std::string_view get_input_main_file_name() const;
    const std::vector<fs::path>& get_input_files() const;

    void prepare_out_dir_and_file() const;
};

} // namespace one_header_gen

#endif // _GENERATOR_FIE_STUTURE_H_
