
#include "files_structure.h"

namespace one_header_gen
{

files_structure_t::files_structure_t(
      fs::path root_dir
    , std::string_view project_name
    , std::string_view input_dir_name
    , std::string_view input_main_file_name
    , std::string_view out_dir_name
    , fs::path template_out_file_path
    ) :
    root_dir(root_dir)
    , project_name(project_name)
    , input_dir(root_dir / input_dir_name / project_name)
    , input_main_file_name(input_main_file_name)
    , out_dir(root_dir / out_dir_name / project_name)
    , template_out_file_path(root_dir / template_out_file_path)
    , out_file_path(out_dir / input_main_file_name)
{
    if (!fs::exists(root_dir)) throw std::runtime_error("Root directory not exist");
    if (project_name.empty()) throw std::runtime_error("Project name is empty");
    if (!fs::exists(input_dir)) throw std::runtime_error("Input directory not exist");
//        if (!fs::exists(out_dir)) throw std::runtime_error("Output directory not exist");
    if (!fs::exists(template_out_file_path)) throw std::runtime_error("Template out file not exist");

    LOG("Generator parameters:");
    LOG("root_dir = ", root_dir);
    LOG("project_name = ", project_name);
    LOG("input_main_file = ", input_main_file_name);
    LOG("input_dir = ", input_dir);
    LOG("out_dir = ", out_dir);
    LOG("out_file_path = ", out_file_path);
    LOG("template_out_file_path = ", template_out_file_path);
}

const fs::path& files_structure_t::get_input_dir_path() const
{
    return input_dir;
}

const fs::path& files_structure_t::get_output_dir_path() const
{
    return out_dir;
}

const fs::path& files_structure_t::get_template_out_file_path() const
{
    return template_out_file_path;
}

const fs::path& files_structure_t::get_out_file_path() const
{
    return out_file_path;
}

std::string_view files_structure_t::get_project_name() const
{
    return project_name;
}

std::string_view files_structure_t::get_input_main_file_name() const
{
    return input_main_file_name;
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

std::vector<fs::path> files_structure_t::get_input_files() const
{
    std::vector<fs::path> result;
    for (const auto & input_file : fs::directory_iterator(get_input_dir_path()))
    {
        if(input_file.status().type() == fs::file_type::regular)
        {
            result.push_back(input_file.path().filename());
        }
    }
    return result;
}

/*!
 * \brief Prepare output directory
 * \details Create/clear directory
 */
void files_structure_t::prepare_out_dir_and_file() const
{
    fs::remove_all(get_output_dir_path());
    LOG("Remove directory ", get_output_dir_path());

    fs::create_directories(get_output_dir_path());
    LOG("Create directory ", get_output_dir_path());

    std::ofstream { get_out_file_path() }; // std::ofstream::trunc
    LOG("Create file ", get_out_file_path());
    LOG("Current content of directory ", get_output_dir_path(), ":");
    for (const auto & outDirEntry : fs::directory_iterator(get_output_dir_path()))
    {
        LOG(outDirEntry);
    }
}

} // namespace one_header_gen
