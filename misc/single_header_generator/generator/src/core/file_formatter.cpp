
#include <regex>
#include <set>

#include "file_formatter.h"
#include "utils.h"
#include "logger.h"

namespace one_header_gen
{

file_formatter_t::file_formatter_t(i_editable_file_ptr_t file)
    : file(std::move(file))
{}

/*!
 * \brief   Delete file description in Doxygen format
 * \details 1. Find line with "{SLASH}*!"
 *          2. Find line with "\file"
 *          3. Find line with "*\/"
 *          4. Delete
 */
void file_formatter_t::delete_file_description()
{
    std::size_t begin = 0, size;
    bool isFileDescription = false;
    auto& lines = file->get_lines();

    size = lines.size();
    for (std::size_t i = 0; i < size; ++i) {
        if (utils::is_begin_of_doxygen_comment(lines[i]))
        {
            begin = i;
        }
        isFileDescription = isFileDescription || utils::is_doxygen_file_description(lines[i]);
        if (utils::is_end_of_comment(lines[i]))
        {
            if (isFileDescription)
            {
                lines.erase(
                        lines.cbegin() + begin,
                        lines.cbegin() + i + 1);
                size -= i - begin + 1;
                isFileDescription = false;
            }
        }
    }
}

/*!
 * \brief Move lines with "#include" to begin of file
 */
void file_formatter_t::move_includes()
{
    auto& lines = file->get_lines();
    const auto r = std::regex(R"((#include[ \t]*[<][a-zA-Z0-9\._/]*[>]).*)");
    size_t size = lines.size();

    std::set<std::string> includes;
    std::smatch match;

    // use temporary vector and swap
    for (size_t i = 0; i < size; ++i) {
        if (std::regex_match(lines[i], match, r)) {
            includes.insert(match[1]);
            lines.erase(lines.cbegin() + i);
//            lines[i].erase();
            --i;
            --size;
        }
    }

        lines.insert(lines.begin(), includes.cbegin(), includes.cend());
}

void file_formatter_t::replace_all_occurancies_in_single_line(
      const std::string &pattern
    , const std::string &replacer)
{
    const auto r = std::regex ( pattern  );
    std::smatch match;
    auto& lines = file->get_lines();

    for(auto &line : lines)
    {
        if(std::regex_search(line, match, r)) {
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
 * \brief Delete include of main source file
 * \details Find line "#include "input_dir_name/mainFileName"" and delete it
 */
void file_formatter_t::delete_include(const std::string& path)
{
    const std::string pattern =
          R"(#include[ \t]+["])"
        + path
        + R"(["][ \t]*)";
    const auto r = std::regex(pattern);
    LOG("Delete include of file, search by pattern: ", pattern);
    auto& lines = file->get_lines();

    auto previous_size = lines.size();
    for(std::size_t i = 0; i < lines.size(); ++i)
    {
        if(std::regex_match(lines[i], r))
        {
            LOG("Delete line with content \"", lines[i], "\"");
            lines.erase(lines.cbegin() + i);
        }
    }

    if (previous_size == lines.size())
    {
        LOG("Include of file ", path ," not founded");
    }
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
void file_formatter_t::delete_include_guards()
{
    auto& lines = file->get_lines();

    auto delete_line = [&lines](std::size_t& i)
    {
        lines.erase(lines.cbegin() + i);
        --i;
    };

    std::stack<std::string> guardIdentifiers;
    for (std::size_t i = 0; i < lines.size(); ++i)
    {
        static const auto ifndef_regex       = std::regex ( R"(#ifndef[ \t]+([A-Z0-9_]+[ \t]*))");
        static const auto common_endif_regex = std::regex ( R"(#endif.*)");
        static const auto pragma_once_regex  = std::regex ( R"(#pragma once.*)");
        std::smatch match;
        if (std::regex_match(lines[i], match, ifndef_regex))
        {
            auto define_regex = std::regex ( R"(#define[ \t]+)" + match[1].str() );
            if(std::regex_match(lines[i + 1], define_regex))
            {
                guardIdentifiers.push(match[1].str());
                lines.erase(std::next(lines.cbegin() + i));
                delete_line(i);
            }
        }

        if (std::regex_match(lines[i], match, common_endif_regex))
        {
            auto commented_endif_regex =
                    std::regex ( R"(#endif[ \t]+/[\*/][ \t]+)" + guardIdentifiers.top() + ".*");
            if (std::regex_match(lines[i], match, commented_endif_regex))
            {
                delete_line(i);
                guardIdentifiers.pop();
            }
            else
            {
                throw std::logic_error(std::string("Not closed guard: " + guardIdentifiers.top()).c_str());
            }
        }

        if (std::regex_match(lines[i], match, pragma_once_regex))
        {
            delete_line(i);
        }
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
void file_formatter_t::preprocess_file(
      std::unordered_map<std::string, i_editable_file_ptr_t> included_files)
{
    const std::string pattern = R"(#include[ \t]+["]([^"]+)["][ \t]*)";
    const auto include_regex = std::regex ( pattern );
    auto& preprocessed_file = file->get_lines();

    std::set<std::string> already_included_files;

    LOG("Start search include of files by pattern: ", pattern);
    for(std::size_t i = 0; i < preprocessed_file.size(); ++i) // i < size
    {
        std::smatch includeMatch;
        if(std::regex_match(preprocessed_file[i], includeMatch, include_regex))
        {
            bool notYetIncluded;
            auto include_path = includeMatch[1].str();
            std::tie(std::ignore, notYetIncluded) = already_included_files.insert(include_path);
            if(notYetIncluded)
            {
                LOG("Delete line ", preprocessed_file[i], " from file ", include_path);
                preprocessed_file.erase(preprocessed_file.cbegin() + i);
                preprocessed_file.insert(
                    preprocessed_file.cbegin() + i,
                    included_files[include_path]->get_lines().cbegin(),
                    included_files[include_path]->get_lines().cend());
                --i;
            }
            else
            {
                LOG("File ", include_path, " already included, delete line: ", preprocessed_file[i] );
                preprocessed_file[i].erase();
                --i;
            }
            LOG("Current size of file ", include_path, ": ", preprocessed_file.size(), " lines");
        }
    }
    LOG("Finish search of included files");
    LOG("End of preprocessing file");
}

} // namespace one_header_gen
