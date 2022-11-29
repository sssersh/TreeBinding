/*!
 * \file  generator.cpp
 * \brief Generate one header from all files of creolization library
 */

#include <regex>
#include <string>
#include <ostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <tuple>
#include <iomanip>

#include "logger.h"
#include "utils.h"
#include "files_provider.h"
#include "file_formatter.h"
#include "generator.h"

namespace one_header_gen
{

i_generator_ptr_t create_generator(const generator_config_t& config)
{
    auto files_provider = create_files_provider(
        config.root_dir / config.input_dir,
        config.root_dir / config.out_dir,
        config.root_dir / config.out_file_path
    );

    auto result_file = std::make_shared<file_t>();
    auto file_formatter = std::make_shared<file_formatter_t>(result_file);

    return std::make_shared<generator_t>(
        std::move(files_provider),
        std::move(result_file)   ,
        std::move(file_formatter),
        config
    );
}


/*!
 * \brief                      Generator constructor
 * \details                    Read filenames from input_dir_name directory
 * \params TODO
 */

generator_t::generator_t(
      i_files_provider_ptr_t files_provider
    , file_ptr_t result_file
    , i_file_formatter_ptr_t file_formatter
    , generator_config_t config
) :
      files_provider(std::move(files_provider))
    , result_file(std::move(result_file))
    , result_file_formatter(std::move(file_formatter)))
    , config(std::move(config))
{
    LOG("Created single header generator with parameters: ");
    LOG("templateOutFile=");
    LOG("=========================================");
    for(const auto& line : files_provider->get_template_out_file->get_lines())
    {
        LOG(line);
    }
    LOG("=========================================");
}

/*!
 * \brief Generate output file
 */
void generator_t::generate()
{
    LOG("Start generate single header include file ", config.out_file_path);

    copy_public_includes_to_output_file();
    delete_include_of_public_inputs();

    auto input_files = files_provider->get_all_input_files();
    std::unordered_map<std::string, i_file_ptr_t> input_files_map;
    std::transform(
        input_files.begin(),
        input_files.end(),
        std::inserter(input_files_map, input_files_map.end()),
        [](decltype(input_files)::value_type &file)
        {
            return std::pair{ file->get_path(), file };
        });
    result_file_formatter->preprocess_file(input_files_map);

    result_file_formatter->delete_file_description();
    result_file_formatter->delete_include_guards();
    result_file_formatter->move_includes_to_top();

    file_t result_file;
    output_file = files_provider->get_out_file();
    // Get template and copy it to output file
    auto result_file_formatter = std::make_shared<file_formatter_t>(result_file);
    result_file_formatter->replace_all_occurancies_in_single_line(
        result_file.get_lines(),
        "PATTERN",
        output_file->to_string());

//    result_file.write(config.get_out_file_path());

    LOG("Succesfully generate single header include file ", config.get_out_file_path());
}

/*!
 * \brief Read public (not in directory "details") input files to internal representation
 */
void generator_t::copy_public_includes_to_output_file()
{
    auto public_includes = files_provider->get_public_input_files();
    for (auto &file : public_includes)
    {
        LOG("Add input file ", file->get_path(), " to output file");
        *result_file += "\n";
        *result_file += *file;
        *result_file += "\n";
    }
    LOG("Finish read public input files"
        , ", result file contain "
        , result_file->get_lines().size()
        , " lines");
}

void generator_t::delete_include_of_public_inputs()
{
    for (auto input_file : files_provider->get_public_input_files())
    {
        result_file_formatter->delete_include(
            fs::relative(config.input_dir, input_file->get_path()));
    }
}

/*!
 * \brief  Insert generated output file in template
 * \return Generated file, inserted in template
 */
//file_t generator_t::insertOutFileInTemplate()
//{
//    auto resultFile = file_t();
//
//
//
//    resultFile += templateOutFile;
//    // TODO
////    resultFile.insert(contentLineIndex, outFile);
//    return resultFile;
//}

} // namespace one_header_gen
