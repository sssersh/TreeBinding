
#ifndef _FS_INTERACTOR_H_
#define _FS_INTERACTOR_H_

#include "i_fs_interactor.h"

namespace one_header_gen
{

class fs_interactor_t : public i_fs_interactor_t
{
    std::vector<fs::path>    get_files_list(const fs::path& directory) override;
    std::vector<fs::path>    get_files_list_recursively(const fs::path& directory) override;
    std::vector<std::string> read_file(const fs::path& file_path) override;
    void write_file(const std::vector<std::string>& content, const fs::path& file_path) override;
};

} // namespace one_header_gen

#endif // _FS_INTERACTOR_H_
