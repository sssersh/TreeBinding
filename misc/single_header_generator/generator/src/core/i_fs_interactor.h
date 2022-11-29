
#ifndef _I_FS_INTERACTOR_H_
#define _I_FS_INTERACTOR_H_

#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

namespace one_header_gen
{

// TODO: adapter
class i_fs_interactor_t
{
public:
    virtual ~i_fs_interactor_t() = default;
    virtual std::vector<fs::path>    get_files_list(const fs::path& directory) = 0;
    virtual std::vector<fs::path>    get_files_list_recursively(const fs::path& directory) = 0;
    virtual std::vector<std::string> read_file(const fs::path& file_path) = 0;

    /*!
     * \brief Write file to path
     * \param[in] path Path to file
     */
    virtual void write_file(const std::vector<std::string>& content, const fs::path& file_path) = 0;
};

using i_fs_interactor_ptr_t = std::shared_ptr<i_fs_interactor_t>;

i_fs_interactor_ptr_t create_fs_interactor();

} // namespace one_header_gen

#endif // _I_FS_INTERACTOR_H_
