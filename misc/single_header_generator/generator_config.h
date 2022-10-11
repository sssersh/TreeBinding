//
// Created by sersh on 07.10.2022.
//

#ifndef _GENERATOR_INFO_H_
#define _GENERATOR_INFO_H_

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
class generator_config_t {

    fs::path root_dir;  /*!< Path to creolization library root directory         */
    std::string project_name; /*!< Name of project                                     */
    fs::path input_dir; /*!< Directory with creolization library sources */
    std::string input_main_file_name;
    fs::path out_dir; /*!< Output directory path */
    fs::path template_out_file_path;

public:
    generator_config_t(
          fs::path root_dir
        , std::string project_name
        , std::string input_dir_name
        , std::string input_main_file_name
        , std::string out_dir_name
        , fs::path template_out_file_path
    ) :
          root_dir(root_dir)
        , project_name(project_name)
        , input_dir(root_dir / input_dir_name / project_name)
        , input_main_file_name(input_main_file_name)
        , out_dir(root_dir / out_dir_name / project_name)
        , template_out_file_path(root_dir / template_out_file_path)
    {
        if (!fs::exists(root_dir)) throw std::runtime_error("Root directory not exist");
        if (project_name.empty()) throw std::runtime_error("Project name is empty");
        if (!fs::exists(input_dir)) throw std::runtime_error("Input directory not exist");
        if (!fs::exists(out_dir)) throw std::runtime_error("Output directory not exist");
        if (!fs::exists(template_out_file_path)) throw std::runtime_error("Template out file not exist");

        LOG("Generator parameters:");
        LOG("root_dir=", root_dir);
        LOG("project_name=", project_name);
        LOG("input_main_file=", input_main_file_name);
        LOG("input_dir=", input_dir);
        LOG("out_dir=", out_dir);
        LOG("template_out_file_path=", template_out_file_path);
    }

    const fs::path &get_input_dir_path() const { return input_dir; }

    const fs::path &get_output_dir_path() const { return out_dir; }

    const fs::path &get_template_out_file_path() const { return template_out_file_path; };

    const std::string &get_project_name() const { return project_name; }

    const std::string &get_input_main_file_name() const { return input_main_file_name; }
};

} // namespace one_header_gen

#endif // _GENERATOR_INFO_H_
