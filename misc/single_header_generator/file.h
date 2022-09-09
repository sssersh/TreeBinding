#ifndef _FILE_H_
#define _FILE_H_

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

namespace one_header_gen {

/*!
 * \brief Structure for represent source file as array of lines
 */
struct file_t {
    std::string filename;
    std::vector<std::string> lines; /*!< Lines */

    file_t() = default;

    file_t(const fs::path &path);

    std::string toString() const;

    void write(const fs::path &path) const;

    void deleteFileDescription();

    void reprlaceInludes();

    const file_t &operator+=(const file_t &rhs);

    const file_t &operator+=(const std::string &rhs);

    void insert(const std::size_t position, const file_t &file);

    void clear();
};

} // namespace one_header_gen

#endif // _FILE_H_
