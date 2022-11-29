
#include <algorithm>
#include <fstream>
#include <memory>

#include "fs_adapter.h"

namespace
{

template <typename T>
auto get_files_list(const fs::path& directory)
{
    std::vector<fs::path> result;
    auto directory_iterator = T(directory);

    std::copy_if(
        fs::begin(directory_iterator),
        fs::end(directory_iterator),
        std::back_inserter(result),
        [](const typename decltype(directory_iterator)::value_type &dir_entry)
        {
            return dir_entry.status().type() == fs::file_type::regular;
        });
    return result;
}

}

namespace one_header_gen
{

i_fs_adapter_ptr_t create_fs_adapter()
{
    return std::make_shared<fs_adapter_t>();
}

std::vector<fs::path> fs_adapter_t::get_files_list(const fs::path& directory)
{
    return ::get_files_list<fs::directory_iterator>(directory);
}

std::vector<fs::path> fs_adapter_t::get_files_list_recursively(const fs::path& directory)
{
    return ::get_files_list<fs::recursive_directory_iterator>(directory);
}

std::vector<std::string> fs_adapter_t::read_file(const fs::path& file_path)
{
    if (!fs::exists(file_path) || !std::filesystem::is_regular_file(file_path))
        throw std::invalid_argument("Incorrect file path: " + file_path.string());

    std::vector<std::string> result;
    std::ifstream file_stream(file_path);
    std::string line;

    while (std::getline(file_stream, line))
    {
        result.emplace_back(std::move(line));
    }

    return result;
}

void fs_adapter_t::write_file(const std::vector<std::string>& content, const fs::path& file_path)
{
    if (!std::filesystem::is_regular_file(file_path))
        throw std::invalid_argument("Incorrect file path: " + file_path.string());
    std::ofstream file_stream(file_path, std::ofstream::trunc);

    for (const auto& line : content)
    {
        file_stream << line << "\n";
    }
}

} // namespace one_header_gen
