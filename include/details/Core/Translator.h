/*!
 *  \file  Translator.h
 *  \brief Translate string value to target type and visa versa
 */

#ifndef _TREE_BINDING_TRANSLATOR_H_
#define _TREE_BINDING_TRANSLATOR_H_

#include <type_traits>
#include <string>
#include "TreeBindingDecl.h"

namespace TreeBinding
{

template<typename T>
struct isString
{
    static const bool value = std::is_same<T, std::string>::value || std::is_same<T, std::wstring>::value;
};

// Overloaded translators declared for arithmetic types and strings
// Translator functions are templates, to keep it only in header file
template<typename T>
struct hasOverloadedTranslator
{
    static const bool value = std::is_arithmetic<T>::value || isString<T>::value;
};

/*!
 *  \brief  Translate string value to target type and visa versa
 */
struct Translator
{
    /*!
     *  \brief  Translate string value to target type
     *  \note   Not used direct get<T2> from boost tree, because it's necessary for forward instance of boost translator_between
     *  \tparam Target type
     *  \param[in]  str   String representation of value
     *  \return     value Target value
     */
    template<typename T>
    static
    typename std::enable_if<!hasOverloadedTranslator<T>::value, T>::type
    fromString(std::string const &str);

    /*!
     *  \brief     Translate target type to string value
     *  \tparam    Target type
     *  \param[in] value Target value
     *  \return    String representation of value
     */
    template<typename T>
    static
    typename std::enable_if<!hasOverloadedTranslator<T>::value, std::string>::type
    toString(const T& value);

    /*!
     *  \brief     Translate arithmetic to string value
     *  \param[in] value Arithmetic value
     *  \return    String representation of value
     */
    template<typename T>
    static
    typename std::enable_if<std::is_arithmetic<T>::value, std::string>::type
    toString(const T& value)
    {
        return std::to_string(value);
    }

    /*!
     *  \brief     Translate string to string value
     *  \param[in] value Source value
     *  \return    Copy of same string
     */
    template<typename T>
    static
    typename std::enable_if<std::is_same<T, std::string>::value, T>::type
    toString(const T& value)
    {
        return value;
    }

    /*!
     *  \brief      Translate string value to integer
     *  \param[in]  str   String representation of value
     *  \return     Integer value
     */
    template<typename T>
    static
    typename std::enable_if<std::is_integral<T>::value, T>::type
    fromString(const std::string& str)
    {
        return std::stoll(str);
    }

    /*!
     *  \brief      Translate string value to floating
     *  \param[in]  str   String representation of value
     *  \return     Float value
     */
    template<typename T>
    static
    typename std::enable_if<std::is_floating_point<T>::value, T>::type
    fromString(const std::string& str)
    {
        return std::stod(str.c_str());
    }

    /*!
     *  \brief     Translate string value to string value
     *  \param[in] str   Source string
     *  \return    Same string
     */
    template<typename T>
    static
    typename std::enable_if<isString<T>::value, T>::type
    fromString(const std::string &str)
    {
        return str;
    }

};

} // namespace data_binding

#endif /* _TREE_BINDING_TRANSLATOR_H_ */
