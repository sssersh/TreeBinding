
#include <algorithm>

#include "files_provider.h"

namespace one_header_gen
{

files_provider_t::files_provider_t(
      fs::path input_dir
    , fs::path out_dir
    , fs::path template_out_file_path
    )
{
    if (!fs::exists(input_dir)) throw std::runtime_error("Input directory not exist");
    if (!fs::exists(template_out_file_path)) throw std::runtime_error("Template out file not exist");

    read_input_files(input_dir);
    prepare_out_dir(out_dir);

    // TODO: remove .in
    out_file = std::make_shared<file_t>(out_dir / template_out_file_path.filename());

    LOG("Create files provider:");
    LOG("input dir = ", input_dir);
    LOG("out dir = ", out_dir);
    LOG("template_out_file_path = ", template_out_file_path);
    LOG("out file = ", out_file->get_path());
    LOG("input files=");
    for (size_t i = 0; i < input_files.size(); ++i)
    {
        LOG(i, ": ", input_files[i]->get_path());
    }
}

file_ptr_t files_provider_t::get_out_file() const
{
    return out_file;
}

std::vector<file_ptr_t> files_provider_t::get_all_input_files() const
{
    return input_files;
}

file_ptr_t files_provider_t::get_input_file(fs::path path)
{
    auto result = std::find_if(
        input_files.cbegin(),
        input_files.cend(),
        [&](const file_ptr_t& file)
        {
            return file->get_path() == path;
        });
    return result == input_files.cend() ? file_ptr_t{} : *result;
}

// Add all header files from input directory

//    src_files_names.emplace_back(std::string(params.get_input_main_file_name()));

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

//            input_file.path().filename()  != get_input_main_file_name()        &&
//           input_file.path().extension() == ".h"

void files_provider_t::read_input_files(const fs::path& input_dir)
{
    for (const auto & input_file : fs::recursive_directory_iterator(input_dir))
    {
        if(input_file.status().type() == fs::file_type::regular)
        {
            input_files.emplace_back(std::make_shared<file_t>(input_file.path()));
        }
    }
    if (input_files.empty()) throw std::runtime_error("No files in input directory");

}

/*!
 * \brief Prepare output directory
 * \details Create/clear directory
 */
void files_provider_t::prepare_out_dir(const fs::path& output_dir) const
{
    fs::remove_all(output_dir);
    LOG("Remove directory ", output_dir);

    fs::create_directories(output_dir);
    LOG("Create directory ", output_dir);
}

} // namespace one_header_gen
