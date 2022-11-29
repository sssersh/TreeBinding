
#include <algorithm>

#include "files_provider.h"

namespace one_header_gen
{

i_files_provider_ptr_t create_files_provider(
      fs::path input_dir
    , fs::path out_dir
    , fs::path template_out_file_path
)
{
    auto fs_interactor = create_fs_interactor();
    auto file_factory  = create_file_factory();

    return std::make_shared<files_provider_t>(
          input_dir
        , out_dir
        , template_out_file_path
        , fs_interactor
        , file_factory
    );
}

files_provider_t::files_provider_t(
      fs::path input_dir
    , fs::path out_dir
    , fs::path template_out_file_path
    , i_fs_interactor_ptr_t fs_interactor
    , i_file_factory_ptr_t file_factory
    ) :
      input_dir(std::move(input_dir))
    , fs_interactor(std::move(fs_interactor))
    , file_factory(std::move(file_factory))
{
    for (const auto & input_file_path : fs_interactor->get_files_list_recursively(input_dir))
    {
        auto file = file_factory->create(input_file_path);
        file->get_lines() = fs_interactor->read_file(input_file_path);
        input_files.emplace_back(file);
    }
    if (input_files.empty()) throw std::runtime_error("No files in input directory");

    // TODO: remove .in
    out_file = file_factory->create(out_dir / template_out_file_path.filename());

    template_out_file = file_factory->create(template_out_file_path);
    template_out_file->get_lines() = fs_interactor->read_file(template_out_file_path);

//    template_out_file = file_factory->create(template_out_file_path);

//    LOG("Create files provider:");
//    LOG("input dir = ", input_dir);
//    LOG("out dir = ", out_dir);
//    LOG("template_out_file_path = ", template_out_file_path);
//    LOG("out file = ", out_file->get_path());
//    LOG("input files=");
//    for (size_t i = 0; i < input_files.size(); ++i)
//    {
//        LOG(i, ": ", input_files[i]->get_path());
//    }
}

files_provider_t::~files_provider_t()
{
    fs_interactor->write_file(out_file->get_lines(), out_file->get_path());
}

i_file_ptr_t files_provider_t::get_out_file() const
{
    return out_file;
}

i_file_ptr_t files_provider_t::get_template_out_file() const
{
    return template_out_file;
}

std::vector<i_file_ptr_t> files_provider_t::get_all_input_files() const
{
    return input_files;
}

i_file_ptr_t files_provider_t::get_input_file(const std::string& path)
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

std::vector<i_file_ptr_t> files_provider_t::get_public_input_files() const
{
    const std::string detail_include_path = "details";
    std::vector<i_file_ptr_t> result;

    std::copy_if(
        input_files.cbegin(),
        input_files.cend(),
        std::back_inserter(result),
        [&](const file_ptr_t& file)
        {
            return fs::relative(file->get_path(), input_dir).parent_path() != detail_include_path;
        });
    return result;
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

//void files_provider_t::read_input_files()
//{
//    for (const auto & input_file : fs::recursive_directory_iterator(input_dir))
//    {
//        if(input_file.status().type() == fs::file_type::regular)
//        {
//            input_files.emplace_back(std::make_shared<file_t>(input_file));
//        }
//    }
//    if (input_files.empty()) throw std::runtime_error("No files in input directory");
//}

/*!
 * \brief Prepare output directory
 * \details Create/clear directory
 */
//void files_provider_t::prepare_out_dir(const fs::path& output_dir) const
//{
//    fs::remove_all(output_dir);
//    LOG("Remove directory ", output_dir);
//
//    fs::create_directories(output_dir);
//    LOG("Create directory ", output_dir);
//}

} // namespace one_header_gen
