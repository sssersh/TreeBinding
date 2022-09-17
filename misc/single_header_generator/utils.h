
#ifndef _CREOLIZATION_UTILS_H_
#define _CREOLIZATION_UTILS_H_

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
bool is_doxygen_description(const std::string &str)
{
    static const auto r = std::regex ( R"(.*\\file.*)" );
    return std::regex_match(str, r);
}

/*!
 * \brief Determine, line contain end of multiply line comment or not ("*\/")
 * \param[in] str Line from source file
 * \retval true  Line contain end of multiply line comment
 * \retval false Otherwise
 */
bool is_end_of_comment(const std::string &str)
{
    static const auto r = std::regex ( R"(.*\*/.*)" );
    return std::regex_match(str, r);
}

} // namespace one_header_gen::utils

#endif //_CREOLIZATION_UTILS_H_
