/*!
 * \file  generator.cpp
 * \brief Generate one header from all files of TreeBinding library
 */

#include <regex>
#include <string>
#include <fstream>
#include <iostream>
#include <experimental/filesystem>
#include <stack>

namespace fs = std::experimental::filesystem;

/*!
 * \brief Structure for represent source file as array of lines
 */
struct File
{
    std::vector<std::string> lines; /*!< Lines */

    File() = default;
    File(const fs::path &path);
    std::string toString() const;
    void write(const fs::path &path) const;
    void deleteFileDescription();
    const File& operator+=(const File &rhs);
    const File& operator+=(const std::string &rhs);
};

/*!
 * \brief Generator implementation
 */
struct Generator
{
    fs::path                 rootDir      ; /*!< Path to TreeBinding library root directory         */
    std::string              srcDirName   ; /*!< Name of directory with TreeBinding library sources */
    std::vector<std::string> srcFilesNames; /*!< Names of TreeBinding library sources (first file used as 
                                                 main file, others - just include main file and 
                                                 and redefine macro from main file) */
    fs::path                 outDirPath   ; /*!< Output directory name */
    fs::path                 outFilePath  ; /*!< Path to out file */
    File                     outFile      ; /*!< Out file content */

    Generator(const fs::path    &rootDir      ,
              const std::string &srcDirName   ,
              const std::string &srcFilesNames,
              const std::string &outDirName);
    void generate();
    void prepareOutDirAndFile() const;
    void readSrcFiles();
    void deleteIncludeMainFile();
    void preprocessFile(File &file);
    void deleteFileDescriptions();
    void deleteIncludeGuards();

    static const size_t MAIN_FILE_INDEX = 0; /*!< Index of main header file in srcFilesNames array */
};

/*!
 * \brief Auxillary utilites
 */
struct Utils
{
    static bool isBeginOfDoxygenComment(const std::string &str);
    static bool isDoxygenFileDescription(const std::string &str);
    static bool isEndOfComment(const std::string &str);
};

/*!
 * \brief File constructor
 * \param[in] path Path to file
 */
File::File(const fs::path &path)
{
    std::ifstream fileStream(path);
    std::string line;

    while (std::getline(fileStream, line)) {
        lines.push_back(std::move(line));
    }
}

/*!
 * \brief Convert file to one string
 */
std::string File::toString() const
{
    std::string result;
    for(const auto &line : lines)
    {
        result += line + "\n";
    }
    return result;
}

/*!
 * \brief Write file to path
 * \param[in] path Path to file
 */
void File::write(const fs::path &path) const
{
    std::ofstream fileStream(path, std::ofstream::trunc);
    fileStream << toString();
}

/*!
 * \brief   Delete file description in Doxygen format
 * \details 1. Find line with "/*!"
 *          2. Find line with "\file"
 *          3. Find line with "*\/"
 *          4. Delete
 */
void File::deleteFileDescription()
{
    size_t begin, size;
    bool isFileDescription = false;

    size = lines.size();
    for(size_t i = 0; i < size; ++i)
    {
        if(Utils::isBeginOfDoxygenComment(lines[i]))
        {
            begin = i;
        }
        isFileDescription = isFileDescription || Utils::isDoxygenFileDescription(lines[i]);
        if(Utils::isEndOfComment(lines[i]))
        {
            if(isFileDescription)
            {
                lines.erase(lines.cbegin() + begin, lines.cbegin() + i + 1);
                size -= i - begin + 1;
                isFileDescription = false;
            }
        }
    }
}

/*!
 * \brief Concatenate files
 * \param[in] rhs Right hand side file
 * \return    Result file
 */
const File& File::operator+=(const File &rhs)
{
    lines.insert(lines.end(), rhs.lines.begin(), rhs.lines.end());
    return *this;
}

/*!
 * \brief Add string to end of file
 * \param[in] rhs Added string
 * \return    Result file
 */
const File& File::operator+=(const std::string &rhs)
{
    lines.push_back(rhs);
    return *this;
}

/*!
 * \brief                     Generator constructor
 * \details                   Read filenames from srcDirName directory
 * \param[in] rootDir         Path to TreeBinding library root directory
 * \param[in] srcDirName      Name of directory with TreeBinding library sources
 * \param[in] srcMainFileName Names of TreeBinding library main header
 * \param[in] outDirName      Output directory name
 */
Generator::Generator(const fs::path    &rootDir        ,
                     const std::string &srcDirName     ,
                     const std::string &srcMainFileName,
                     const std::string &outDirName) :
        rootDir(rootDir),
        srcDirName(srcDirName),
    outDirPath(rootDir / outDirName),
    outFilePath(rootDir / outDirName / srcMainFileName)
{
    srcFilesNames.push_back(srcMainFileName);
    auto srcPath = rootDir/srcDirName;
    for (const auto & srcFile : fs::directory_iterator(srcPath))
    {
        if(srcFile.status().type() == fs::file_type::regular &&
           srcFile.path().filename() != srcMainFileName)
        {
            srcFilesNames.push_back(srcFile.path().filename());
        }
    }
}

/*!
 * \brief Generate output file
 */
void Generator::generate()
{
    prepareOutDirAndFile();
    readSrcFiles();
    deleteIncludeMainFile();
    preprocessFile(outFile);
    outFile.write(outFilePath);
    deleteFileDescriptions();
    deleteIncludeGuards();
}

/*!
 * \brief Prepare output directory
 * \details Create/clear directory
 */
void Generator::prepareOutDirAndFile() const
{
    fs::remove_all(outDirPath);
    fs::create_directory(outDirPath);
    std::ofstream fileStream(outFilePath, std::ofstream::trunc);
}

/*!
 * \brief Read source files to internal representation
 */
void Generator::readSrcFiles()
{
    for(const auto& fileName : srcFilesNames)
    {
        auto srcPath = rootDir / srcDirName / fileName;

        outFile += "\n";
        outFile += File(srcPath);
        outFile += "\n";
    }
}

/*!
 * \brief Delete include of main source file
 * \details Find line "#include "srcDirName/mainFileName"" and delete it
 */
void Generator::deleteIncludeMainFile()
{
    for(auto &line : outFile.lines)
    {
        auto r = std::regex ( R"(#include[ \t]+["])" + srcDirName + "/" + srcFilesNames[MAIN_FILE_INDEX] + R"(["][ \t]*)" );
        if(std::regex_match(line, r))
        {
            line.clear();
        }
    }
}

/*!
 * \brief     Preprocess file
 * \details   Replace line contained "#include "srcDirName/*" with content of file
 * \param[in] file Internal representation of file
 */
void Generator::preprocessFile(File &file)
{
    for(auto &line : file.lines)
    {
        auto r = std::regex ( R"(#include[ \t]+["]()" + srcDirName + R"([^"]+)["][ \t]*)" );
        std::smatch includeMatch;
        if(std::regex_match(line, includeMatch, r))
        {
            auto includeFilePath = rootDir / includeMatch[1].str();
            auto includeFile = File(includeFilePath);
            preprocessFile(includeFile);
            line = "\n" + includeFile.toString() + "\n";
        }
    }
}

/*!
 * \brief Delete file description in Doxygen format from output file
 */
void Generator::deleteFileDescriptions()
{
    auto file = File(outFilePath);
    file.deleteFileDescription();
    file.write(outFilePath);
}

/*!
 * \brief   Delete include guards from output file
 * \details a. 1. Find line with "#ifndef *"
 *             2. If next line is "#define *" with same identificator,
 *                2.1 Delete these lines.
 *                2.2 Save guard identifier
 *          b. If contains saved guard identifiers
 *             1. If line contain "#endif /* last_guard_identifier *\/, delete line.
 */
void Generator::deleteIncludeGuards()
{
    auto file = File(outFilePath);
    std::stack<std::string> guardIdentifiers;
    for(size_t i = 0; i < file.lines.size(); ++i)
    {
        auto ifdefRegex = std::regex ( R"(#ifndef[ \t]+([A-Z0-9_]+[ \t]*))");
        std::smatch match;
        if(std::regex_match(file.lines[i], match, ifdefRegex))
        {
            auto defineRegex = std::regex ( R"(#define[ \t]+)" + match[1].str() );
            if(std::regex_match(file.lines[i + 1], defineRegex))
            {
                guardIdentifiers.push(match[1].str());
                file.lines[i].clear();
                file.lines[i + 1].clear();
            }
        }

        if(!guardIdentifiers.empty())
        {
            auto endifRegex = std::regex ( R"(#endif[ \t]+/\*[ \t+])" + guardIdentifiers.top() + R"([ \t]\*/)");
            if(std::regex_match(file.lines[i], match, endifRegex)) {
                file.lines[i].clear();
                guardIdentifiers.pop();
            }
        }
    }
    file.write(outFilePath);
}

/*!
 * \brief Determine, line contain start of Doxygen comment or not ("/*!")
 * \param[in] str Line from source file
 * \retval true  Line contain start of Doxygen comment
 * \retval false Otherwise
 */
bool Utils::isBeginOfDoxygenComment(const std::string &str)
{
    auto r = std::regex ( R"(.*/\*!.*)" );
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
    auto r = std::regex ( R"(.*\\file.*)" );
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
    auto r = std::regex ( R"(.*\*/.*)" );
    return std::regex_match(str, r);
}

/*!
 * \brief Main function
 */
int main(int argc, char* argv[])
{
    try
    {
        Generator generator = {
            "./../.."      ,
            "TreeBinding"  ,
            "TreeBinding.h",
            "gen"
        };
        generator.generate();
    }
    catch(const std::exception& e)
    {
        std::cout << e.what();
    }
}
