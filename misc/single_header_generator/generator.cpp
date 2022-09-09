/*!
 * \file  generator.cpp
 * \brief Generate one header from all files of creolization library
 */

#include <regex>
#include <string>
#include <set>
#include <ostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <tuple>
#include <iomanip>

#include "logger.h"
#include "file.h"



/*!
 * \brief Generator implementation
 */
struct Generator
{
    Generator(const fs::path    &rootDir            ,
              const std::string &projectName        ,
              const std::string &srcDirName         ,
              const std::string &srcFilesNames      ,
              const std::string &outDirName         ,
              const std::string &templateOutFilePath);
    void generate();

private:
    void prepareOutDirAndFile() const;
    void readSrcFiles();
    void deleteIncludeMainFile();
    void preprocessFile(file_t &file, std::set<std::string> &&alreadyIncludedFiles = {});
    void deleteIncludeGuards();
    file_t insertOutFileInTemplate();

    fs::path                 rootDir         ; /*!< Path to creolization library root directory         */
    std::string              projectName     ; /*!< Name of project                                     */
    std::string              srcDirName      ; /*!< Name of directory with creolization library sources */
    std::vector<std::string> srcFilesNames   ; /*!< Names of creolization library sources (first file used as
                                                    main file, others - just single_include main file and
                                                    and redefine macro from main file) */
    fs::path                 outDirPath      ; /*!< Output directory name */
    fs::path                 outFilePath     ; /*!< Path to out file */
    file_t                     outFile         ; /*!< Out file content */
    file_t                     templateOutFile ; /*!< Template of out file */

    static const std::size_t MAIN_FILE_INDEX = 0; /*!< Index of main header file in srcFilesNames array */
};

/*!
 * \brief Auxillary utilites
 */
struct Utils
{
    static bool isBeginOfDoxygenComment(const std::string &str);
    static bool isDoxygenFileDescription(const std::string &str);
    static bool isEndOfComment(const std::string &str);
    static void replaceAllOccurancies(File &file, const std::string &pattern, const std::string &replacer);
};

/*!
 * \brief                      Generator constructor
 * \details                    Read filenames from srcDirName directory
 * \param[in] rootDir          Path to creolization library root directory
 * \param[in] projectName      Name of project
 * \param[in] srcDirName       Name of directory with creolization library sources
 * \param[in] srcMainFileName  Names of creolization library main header
 * \param[in] outDirName       Output directory name
 * \param[in] templateOutFile  Template of out file
 * \param[in] contentLineIndex Index of line, where will be insert generated file
 */

Generator::Generator(const fs::path    &rootDir            ,
                     const std::string &projectName        ,
                     const std::string &srcDirName         ,
                     const std::string &srcMainFileName    ,
                     const std::string &outDirName         ,
                     const std::string &templateOutFilePath) :
    rootDir(rootDir),
    projectName(projectName),
    srcDirName(srcDirName),
    outDirPath(rootDir / outDirName / projectName),
    outFilePath(rootDir / outDirName / projectName / srcMainFileName),
    outFile(outFilePath),
    templateOutFile(templateOutFilePath)
{
    srcFilesNames.push_back(srcMainFileName);

    auto srcPath = rootDir / srcDirName / projectName;
    // add .cpp files from all nested folders
    // Now library is one-header, ignore all cpp
    /*
    for(const auto &cppFile : fs::recursive_directory_iterator(srcPath))
    {
        if(cppFile.path().extension() == ".cpp")
        {
            auto cppFilePath = cppFile.path().string();
            std::string cppFileName = cppFilePath.erase(0, srcPath.string().size() + std::string("/").size());
            srcFilesNames.push_back(cppFileName);
        }
    }
    */

    // Add all header files from root sources directory
    for (const auto & srcFile : fs::directory_iterator(srcPath))
    {
        if(srcFile.status().type()    == fs::file_type::regular &&
           srcFile.path().filename()  != srcMainFileName        &&
           srcFile.path().extension() == ".h" )
        {
            srcFilesNames.push_back(srcFile.path().filename());
        }
    }

    LOG("Created single header generator with parameters: ");
    LOG("rootDir=", rootDir);
    LOG("projectName=", projectName);
    LOG("srcDirName=", srcDirName);
    LOG("srcPath=", srcPath);
    LOG("outDirPath=", outDirPath);
    LOG("outFilePath=", outFilePath);
    LOG("templateOutFile=");
    LOG("=========================================");
    for(const auto& line : templateOutFile.lines)
    {
        LOG(line);
    }
    LOG("=========================================");
    LOG("srcFilesNames=");
    for (size_t i = 0; i < srcFilesNames.size(); ++i)
    {
        LOG(i, ": ", srcFilesNames[i], (i ? "" : " (main file)"));
    }
}

/*!
 * \brief Generate output file
 */
void Generator::generate()
{
    LOG("Start generate single header include file ", outFilePath);

    prepareOutDirAndFile();
    readSrcFiles();
    deleteIncludeMainFile();
    preprocessFile(outFile);
    outFile.deleteFileDescription();
    deleteIncludeGuards();
    outFile.reprlaceInludes();
    auto resultFile = insertOutFileInTemplate();
    resultFile.write(outFilePath);

    LOG("Succesfully generate single header include file ", outFilePath);
}

/*!
 * \brief Prepare output directory
 * \details Create/clear directory
 */
void Generator::prepareOutDirAndFile() const
{
    fs::remove_all(outDirPath);
    LOG("Remove directory ", outDirPath);

    fs::create_directory(outDirPath);
    LOG("Create directory ", outDirPath);

    std::ofstream fileStream(outFilePath, std::ofstream::trunc);
    LOG("Create file ", outFilePath);
    LOG("Current content of directory ", outDirPath, ":");
    for (const auto & outDirEntry : fs::directory_iterator(outDirPath))
    {
        LOG(outDirEntry);
    }
}

/*!
 * \brief Read source files to internal representation
 */
void Generator::readSrcFiles()
{
    auto srcDirPath = rootDir / srcDirName / projectName;
    LOG("Start read source files in path ", outDirPath);
    for(const auto& fileName : srcFilesNames)
    {
        auto srcPath = srcDirPath / fileName;

        LOG("Read source file ", srcPath);

        outFile += "\n";
        outFile += File(srcPath);
        outFile += "\n";
    }
    LOG("Finish read source files in path ", outDirPath, ", result file contain ", outFile.lines.size(), " lines");
}

/*!
 * \brief Delete include of main source file
 * \details Find line "#include "srcDirName/mainFileName"" and delete it
 */
void Generator::deleteIncludeMainFile()
{
    const std::string pattern =
            R"(#include[ \t]+["])" + projectName + "/" + srcFilesNames[MAIN_FILE_INDEX] + R"(["][ \t]*)";
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
 * \details   Recursively replace line contained "#include "srcDirName{SLASH}*" with content of file.
 *            Multiply included of one file ignored.
 * \param[in] file Internal representation of file
 * \param[in] alreadyIncludedFiles Already included files (used rvalue reference because it's necessary to
 *            bind refence with default value, also it's necessary to pass by reference)
 */
void Generator::preprocessFile(File &file, std::set<std::string> &&alreadyIncludedFiles)
{
    auto size = file.lines.size();
    const std::string pattern = R"(#include[ \t]+["]()" + projectName + R"([^"]+)["][ \t]*)";
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
                auto includeFilePath = rootDir / srcDirName / includeMatch[1].str();
                auto includeFile = File(includeFilePath);
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
void Generator::deleteIncludeGuards()
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
File Generator::insertOutFileInTemplate()
{
    auto resultFile = File();



    resultFile += templateOutFile;
    resultFile.insert(contentLineIndex, outFile);
    return resultFile;
}

/*!
 * \brief Determine, line contain start of Doxygen comment or not ("{SLASH}*!")
 * \param[in] str Line from source file
 * \retval true  Line contain start of Doxygen comment
 * \retval false Otherwise
 */
bool Utils::isBeginOfDoxygenComment(const std::string &str)
{
    static const auto r = std::regex ( R"(.*/\*!.*)" );
    return std::regex_match(str, r);
}

/*!
 * \brief Determine, line contain Doxygen tag "file" or not
 * \param[in] str Line from source file
 * \retval true  Line contain Doxygen tag "file"
 * \retval false Otherwise
 */
bool Utils::isDoxygenFileDescription(const std::string &str)
{
    static const auto r = std::regex ( R"(.*\\file.*)" );
    return std::regex_match(str, r);
}

/*!
 * \brief Determine, line contain end of multiply line comment or not ("*\/")
 * \param[in] str Line from source file
 * \retval true  Line contain end of multiply line comment
 * \retval false Otherwise
 */
bool Utils::isEndOfComment(const std::string &str)
{
    static const auto r = std::regex ( R"(.*\*/.*)" );
    return std::regex_match(str, r);
}

void Utils::replaceAllOccurancies(File &file, const std::string &pattern, const std::string &replacer)
{
    const auto r = std::regex ( pattern  );
    std::smatch match;

    for(auto &line : file.lines)
    {
        if(std::regex_match(line, match, r)) {
            std::ostringstream ostream;
            std::regex_replace(
                std::ostreambuf_iterator<char>(ostream),
                line.begin(),
                line.end(),
                r,
                replacer);
            line = ostream.str();
        }
    }
}

/*!
 * \brief Main function
 */
int main(int argc, char* argv[])
{
//    logger_t::set_out_file("single_header_genearator.log");

    if(argc != 2)
    {
        LOG("Invalid number of single header generator arguments. Expected: 2, actually: ", argc);
        return -1;
    }

    try
    {
        Generator generator = {
            argv[1]      ,
            "creolization",
            "include",
            "serializable_types.h",
            "single_include",
            "output.template"
        };
        generator.generate();
        return 0;
    }
    catch(const std::exception& e)
    {
        std::cout << "Single header generator error: \n" << e.what();
        return -1;
    }
}
