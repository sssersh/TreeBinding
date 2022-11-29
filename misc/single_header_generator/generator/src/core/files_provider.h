
#ifndef _GENERATOR_FIE_STUTURE_H_
#define _GENERATOR_FIE_STUTURE_H_

#include <vector>
#include <filesystem>

#include "logger.h"
#include "file.h"
#include "i_files_provider.h"
#include "i_fs_adapter.h"
#include "i_file_factory.h"

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
class files_provider_t : public i_files_provider_t
{

public:
    files_provider_t(
          fs::path input_dir
        , fs::path out_dir
        , fs::path template_out_file_path
        , i_fs_adapter_ptr_t fs_adapter
        , i_file_factory_ptr_t file_factory
    );

    std::vector<i_file_ptr_t> get_all_input_files() const override;
    std::vector<i_file_ptr_t> get_public_input_files() const override;
    i_file_ptr_t get_input_file(const std::string& path) override;
    i_file_ptr_t get_template_out_file() const override;
    i_file_ptr_t get_out_file() const override;

    ~files_provider_t();

private:
    void read_input_files();
    void prepare_out_dir(const fs::path& out_dir) const;

    fs::path input_dir;
    i_fs_adapter_ptr_t fs_adapter;
    i_file_factory_ptr_t file_factory;
    i_file_ptr_t out_file; /*!< Path to out file */
    std::vector<i_file_ptr_t> input_files   ; /*!< Names of creolization library sources (first file used as
                                                    main file, others - just single_include main file and
                                                    and redefine macro from main file) */
    i_file_ptr_t template_out_file;
};

//using files_provider_ptr_t = std::shared_ptr<files_provider_t>;

} // namespace one_header_gen

#endif // _GENERATOR_FIE_STUTURE_H_
