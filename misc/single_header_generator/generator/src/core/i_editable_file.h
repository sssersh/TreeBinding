
#ifndef _I_EDITABLE_FILE_H_
#define _I_EDITABLE_FILE_H_

#include <vector>
#include <string>
#include <memory>

class i_editable_file_t
{
public:
    virtual ~i_editable_file_t() = default;
    virtual std::vector<std::string>& get_lines() = 0;
};

using i_editable_file_ptr_t = std::shared_ptr<i_editable_file_t>;

#endif // _I_EDITABLE_FILE_H_
