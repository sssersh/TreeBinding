
#ifndef _CREOLIZATION_UTILS_H_
#define _CREOLIZATION_UTILS_H_

#include <string>

namespace one_header_gen::utils
{

/*!
 * \brief Determine, line contain start of Doxygen comment or not ("{SLASH}*!")
 * \param[in] str Line from source file
 * \retval true  Line contain start of Doxygen comment
 * \retval false Otherwise
 */
bool is_begin_of_doxygen_comment(const std::string &str);
/*!
 * \brief Determine, line contain Doxygen tag "file" or not
 * \param[in] str Line from source file
 * \retval true  Line contain Doxygen tag "file"
 * \retval false Otherwise
 */
bool is_doxygen_file_description(const std::string &str);

/*!
 * \brief Determine, line contain end of multiply line comment or not ("*\/")
 * \param[in] str Line from source file
 * \retval true  Line contain end of multiply line comment
 * \retval false Otherwise
 */
bool is_end_of_comment(const std::string &str);

} // namespace one_header_gen::utils

#endif //_CREOLIZATION_UTILS_H_
