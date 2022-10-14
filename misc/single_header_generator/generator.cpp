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

/*!
 * \brief                      Generator constructor
 * \details                    Read filenames from input_dir_name directory
 * \params TODO
 */

generator_t::generator_t(
      i_file_formatter_ptr_t file_formatter
    , const generator_config_t &params) :
          file_formatter(std::move(file_formatter))
        , config(params)
    , templateOutFile(params.get_template_out_file_path())
{
    src_files_names.emplace_back(std::string(params.get_input_main_file_name()));

    // add .cpp files from all nested folders
    // Now library is one-header, ignore all cpp
    /*
    for(const auto &cppFile : fs::recursive_directory_iterator(srcPath))
    {
        if(cppFile.path().extension() == ".cpp")
        {
            auto cppFilePath = cppFile.path().string();
            std::string cppFileName = cppFilePath.erase(0, srcPath.string().size() + std::string("/").size());
            src_files_names.push_back(cppFileName);
        }
    }
    */

    // Add all header files from root sources directory
    for (const auto & input_file : fs::directory_iterator(params.get_input_dir_path()))
    {
        if(input_file.status().type()    == fs::file_type::regular &&
            input_file.path().filename()  != params.get_input_main_file_name()        &&
            input_file.path().extension() == ".h" )
        {
            src_files_names.push_back(input_file.path().filename());
        }
    }

    LOG("Created single header generator with parameters: ");
    LOG("templateOutFile=");
    LOG("=========================================");
    for(const auto& line : templateOutFile.get_lines())
    {
        LOG(line);
    }
    LOG("=========================================");
    LOG("src_files_names=");
    for (size_t i = 0; i < src_files_names.size(); ++i)
    {
        LOG(i, ": ", src_files_names[i], (i ? "" : " (main file)"));
    }
}

/*!
 * \brief Generate output file
 */
void generator_t::generate()
{
    LOG("Start generate single header include file ", config.get_out_file_path());

    prepare_out_dir_and_file();
    read_src_files();
    file_formatter->delete_include(
        outFile.get_lines(),
        std::string(config.get_project_name())
        + "/"
        + src_files_names[MAIN_FILE_INDEX]);
    preprocessFile(outFile);
    file_formatter->delete_file_description(outFile.get_lines());
//    deleteIncludeGuards();
    file_formatter->move_includes(outFile.get_lines());
    auto resultFile = insertOutFileInTemplate();
    resultFile.write(config.get_out_file_path());

    LOG("Succesfully generate single header include file ", config.get_out_file_path());
}

/*!
 * \brief Prepare output directory
 * \details Create/clear directory
 */
void generator_t::prepare_out_dir_and_file() const
{
    fs::remove_all(config.get_output_dir_path());
    LOG("Remove directory ", config.get_output_dir_path());

    fs::create_directories(config.get_output_dir_path());
    LOG("Create directory ", config.get_output_dir_path());

    std::ofstream { config.get_out_file_path() }; // std::ofstream::trunc
    LOG("Create file ", config.get_out_file_path());
    LOG("Current content of directory ", config.get_output_dir_path(), ":");
    for (const auto & outDirEntry : fs::directory_iterator(config.get_output_dir_path()))
    {
        LOG(outDirEntry);
    }
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
        outFile += file_t(srcPath);
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
file_t generator_t::insertOutFileInTemplate()
{
    auto resultFile = file_t();



    resultFile += templateOutFile;
    // TODO
//    resultFile.insert(contentLineIndex, outFile);
    return resultFile;
}

} // namespace one_header_gen
