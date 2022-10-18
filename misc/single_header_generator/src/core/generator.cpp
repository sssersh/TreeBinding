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
#include "generator.h"

namespace one_header_gen
{

i_generator_ptr_t create_generator()
{
    auto files_provider = std::make_shared<files_provider_t>(TODO);
    auto file_formatter = std::make_shared<file_formatter_t>(TODO)
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
    LOG("Start generate single header include file ", config.get_out_file_path());

    read_src_files();
    file_formatter->delete_include(
        outFile.get_lines(),
//        std::string(config.get_project_name()) +
        "/"
        + src_files_names[MAIN_FILE_INDEX]);
    preprocessFile(outFile);
    file_formatter->delete_file_description(outFile.get_lines());
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
void generator_t::read_src_files()
{
    LOG("Start read input files in path ", config.get_input_dir_path());
    for(const auto& fileName : src_files_names)
    {
        auto srcPath = config.get_input_dir_path() / fileName;

        LOG("Read source file ", srcPath);

        outFile += "\n";
        outFile += files_provider.get_input_file(path);// file_t(srcPath);
        outFile += "\n";
    }
    LOG("Finish read source files in path "
        , config.get_input_dir_path()
        , ", result file contain "
        , outFile.get_lines().size()
        , " lines");
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
