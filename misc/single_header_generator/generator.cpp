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

generator_t::generator_t(const generator_info_t &params) :
      params(params)
    , outFile(outFilePath)
    , templateOutFile(params.get_template_out_file_path())
{
    src_files_names.push_back(params.get_input_main_file_name());

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
    for(const auto& line : templateOutFile.lines)
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
    LOG("Start generate single header include file ", outFilePath);

    prepare_out_dir_and_file();
    readSrcFiles();
    deleteIncludeMainFile();
    preprocessFile(outFile);
    outFile.delete_file_description();
    deleteIncludeGuards();
    outFile.move_includes();
    auto resultFile = insertOutFileInTemplate();
    resultFile.write(outFilePath);

    LOG("Succesfully generate single header include file ", outFilePath);
}

/*!
 * \brief Prepare output directory
 * \details Create/clear directory
 */
void generator_t::prepare_out_dir_and_file() const
{
    fs::remove_all(params.get_output_dir_path());
    LOG("Remove directory ", params.get_output_dir_path());

    fs::create_directory(params.get_output_dir_path());
    LOG("Create directory ", params.get_output_dir_path());

    std::ofstream fileStream(outFilePath, std::ofstream::trunc);
    LOG("Create file ", outFilePath);
    LOG("Current content of directory ", params.get_output_dir_path(), ":");
    for (const auto & outDirEntry : fs::directory_iterator(params.get_output_dir_path()))
    {
        LOG(outDirEntry);
    }
}

/*!
 * \brief Read source files to internal representation
 */
void generator_t::readSrcFiles()
{
    LOG("Start read input files in path ", params.get_input_dir_path());
    for(const auto& fileName : src_files_names)
    {
        auto srcPath = params.get_input_dir_path() / fileName;

        LOG("Read source file ", srcPath);

        outFile += "\n";
        outFile += file_t(srcPath);
        outFile += "\n";
    }
    LOG("Finish read source files in path ", params.get_input_dir_path(), ", result file contain ", outFile.lines.size(), " lines");
}

/*!
 * \brief Delete include of main source file
 * \details Find line "#include "input_dir_name/mainFileName"" and delete it
 */
void generator_t::deleteIncludeMainFile()
{
    const std::string pattern =
            R"(#include[ \t]+["])" + params.get_project_name() + "/" + src_files_names[MAIN_FILE_INDEX] + R"(["][ \t]*)";
    const auto r = std::regex(pattern);
    LOG("Delete include of main file, search by pattern: ", pattern);

    bool main_file_founded = false;
    for(auto &line : outFile.lines)
    {
        if(std::regex_match(line, r))
        {
            main_file_founded = true;
            LOG("Delete line with content \"", line, "\"");
            line.clear();
        }
    }

    if (!main_file_founded)
    {
        LOG("Include of main file not founded");
    }
}

/*!
 * \brief     Preprocess file
 * \details   Recursively replace line contained "#include "input_dir_name{SLASH}*" with content of file.
 *            Multiply included of one file ignored.
 * \param[in] file Internal representation of file
 * \param[in] alreadyIncludedFiles Already included files (used rvalue reference because it's necessary to
 *            bind refence with default value, also it's necessary to pass by reference)
 */
void generator_t::preprocessFile(file_t &file, std::set<std::string> &&alreadyIncludedFiles)
{
    auto size = file.lines.size();
    const std::string pattern = R"(#include[ \t]+["]()" + params.get_project_name() + R"([^"]+)["][ \t]*)";
    const auto r = std::regex ( pattern );

    LOG("Start recursively preprocess file, now file contains ", size , " lines",
        (alreadyIncludedFiles.size() ? ", already included files: " : "") );
    for(const auto& i : alreadyIncludedFiles)
    {
        LOG("- ", i);
    }

    LOG("Start search include of files by pattern: ", pattern);
    for(std::size_t i = 0; i < size; ++i)
    {
        std::smatch includeMatch;
        if(std::regex_match(file.lines[i], includeMatch, r))
        {
            LOG("Found include of file: ", file.lines[i]);
            bool notYetIncluded;
            std::tie(std::ignore, notYetIncluded) = alreadyIncludedFiles.insert(includeMatch[1].str());
            if(notYetIncluded)
            {
                auto includeFilePath = params.get_input_dir_path() / includeMatch[1].str();
                auto includeFile = file_t(includeFilePath);
                preprocessFile(includeFile, std::move(alreadyIncludedFiles));
                LOG("Delete line ", file.lines[i], " from file ", includeFilePath.filename());
                file.lines[i].erase();
                file.insert(i, includeFile);
                size += includeFile.lines.size() - 1; // -1 - erased line
            }
            else
            {
                LOG("File ", includeMatch[1].str(), " already included, delete line: ", file.lines[i] );
                file.lines[i].erase();
                size--;
            }
            LOG("Current size of file ", file.filename, ": ", file.lines.size(), " lines");
        }
    }
    LOG("Finish search of included files");
    LOG("End of preprocessing file");
}

/*!
 * \brief   Delete include guards from output file
 * \details a. 1. Find line with "#ifndef *"
 *             2. If next line is "#define *" with same identificator,
 *                2.1 Delete these lines.
 *                2.2 Save guard identifier
 *          b. If contains saved guard identifiers
 *             1. If line contain "#endif {SLASH}* last_guard_identifier *\/, delete line.
 */
void generator_t::deleteIncludeGuards()
{
    std::stack<std::string> guardIdentifiers;
    for(std::size_t i = 0; i < outFile.lines.size(); ++i)
    {
        static const auto ifdefRegex = std::regex ( R"(#ifndef[ \t]+([A-Z0-9_]+[ \t]*))");
        std::smatch match;
        if(std::regex_match(outFile.lines[i], match, ifdefRegex))
        {
            auto defineRegex = std::regex ( R"(#define[ \t]+)" + match[1].str() );
            if(std::regex_match(outFile.lines[i + 1], defineRegex))
            {
                guardIdentifiers.push(match[1].str());
                outFile.lines[i].clear();
                outFile.lines[i + 1].clear();
            }
        }

        if(!guardIdentifiers.empty())
        {
            auto endifRegex =
                    std::regex ( R"(#endif[ \t]+/\*[ \t+])" + guardIdentifiers.top() + R"([ \t]\*/)");
            if(std::regex_match(outFile.lines[i], match, endifRegex)) {
                outFile.lines[i].clear();
                guardIdentifiers.pop();
            }
        }
    }
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