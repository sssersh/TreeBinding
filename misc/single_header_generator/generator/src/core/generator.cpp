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
    auto files_provider = std::make_shared<files_provider_t>(
        config.root_dir / config.input_dir,
        config.root_dir / config.out_dir,
        config.root_dir / config.out_file_path
    );

    auto file_formatter = std::make_shared<file_formatter_t>();

    return std::make_shared<generator_t>(
        std::move(files_provider),
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
    , i_file_formatter_ptr_t file_formatter
    , generator_config_t config
) :
      files_provider(std::move(files_provider))
    , file_formatter(std::move(file_formatter)))
    , config(std::move(config))
{

    LOG("Created single header generator with parameters: ");
    LOG("templateOutFile=");
    LOG("=========================================");
    for(const auto& line : templateOutFile.get_lines())
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
//    LOG("Start generate single header include file ", config.get_out_file_path());

    output_file = files_provider->get_out_file();
    copy_public_includes_to_output_file();
    delete_include_of_public_inputs();
    auto input_files = files_provider->get_all_input_files();
    file_formatter->preprocess_file(output_file->get_lines(), input_files);



    file_formatter->delete_file_description(outFile.get_lines(), );
//    deleteIncludeGuards();
    file_formatter->move_includes(outFile.get_lines());
//    auto resultFile = insertOutFileInTemplate();
    file_t result_file;
    file_formatter->replace_all_occurancies_in_single_line(
            result_file.get_lines(),
            "PATTERN",
            outFile.to_string());
    result_file.write(config.get_out_file_path());

    LOG("Succesfully generate single header include file ", config.get_out_file_path());
}

/*!
 * \brief Read source files to internal representation
 */
void generator_t::copy_public_includes_to_output_file()
{
    auto public_includes = files_provider->get_public_input_files();
    for (auto &file : public_includes)
    {
        LOG("Add input file ", file->get_path(), " to input file");
        *output_file += "\n";
        *output_file += *file;
        *output_file += "\n";
    }
    LOG("Finish read public input files"
        , ", result file contain "
        , output_file->get_lines().size()
        , " lines");
}

void generator_t::delete_include_of_public_inputs()
{
    for (auto input_file : files_provider->get_public_input_files()) {
        file_formatter->delete_include(
                output_file->get_lines(),
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
