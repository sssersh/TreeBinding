
#ifndef _GENERATOR_CONFIG_H_
#define _GENERATOR_CONFIG_H_

#include <string_view>
#include <filesystem>

namespace one_header_gen
{

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
struct generator_config_t
{
    fs::path root_dir;  /*!< Path to creolization library root directory         */
    std::string_view project_name; /*!< Name of project                                     */
    std::string_view input_main_file_name;
    fs::path input_dir; /*!< Directory with creolization library sources */
    fs::path out_dir; /*!< Output directory path */
    fs::path template_out_file_path;
    fs::path out_file_path; /*!< Path to out file */
    //    if (project_name.empty()) throw std::runtime_error("Project name is empty");
};

    LOG("Generator parameters:");
//    LOG("root_dir = ", root_dir);
//    LOG("project_name = ", project_name);
//     LOG("input_main_file = ", input_main_file_name);

} // namespace one_header_gen

#endif // _GENERATOR_CONFIG_H_
