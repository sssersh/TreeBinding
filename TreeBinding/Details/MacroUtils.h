/*!
 *  \file  MacroUtils.h
 *  \brief Utils for macroses
 */

#ifndef _MACROUTILS_H_
#define _MACROUTILS_H_

/*!
 * \brief Concatenate two tokens
 */
#define TREE_BINDING_DETAILS_TOKEN_PASTE(x, y) x##y

/*!
 * \brief Concatenate two tokens
 */
#define TREE_BINDING_DETAILS_CONCAT(x, y) TREE_BINDING_DETAILS_TOKEN_PASTE(x, y)

/*!
 * \brief     Macro for expand multiply parameters, because stupid MSVC passed __VA_ARGS__ as single parameter
 * \param[in] __VA_ARGS__ macro
 * \return    __VA_ARGS__ expanded to multiply parameters
 */
#define TREE_BINDING_DETAILS_EXPAND( x ) x

/*!
 * \brief   Choose overloaded macro
 * \details First args - arguments, passes to target macro. After it passed overloaded macroses in descent order.
 */
#define TREE_BINDING_DETAILS_GET_MACRO(_1, _2, _3, TARGET_MACRO, ...) TARGET_MACRO

/*!
 * \brief    Overload macro
 * \details  Pass empty string to TREE_BINDING_DETAILS_GET_MACRO() to avoid error
 *           "ISO C++11 requires at least one argument for the "..." in a variadic macro"
 * \warning  Overload macroses only with 1, 2 or 3 arguments
 * \param[in] overload1 Overload with 1 argument
 * \param[in] overload2 Overload with 2 argument
 * \param[in] overload3 Overload with 3 argument
 * \param[in] ... arguments of overloaded macro
 */
#define TREE_BINDING_DETAILS_OVERLOAD_MACRO(overload1, overload2, overload3, ...) \
    TREE_BINDING_DETAILS_EXPAND(                                                  \
        TREE_BINDING_DETAILS_GET_MACRO(__VA_ARGS__,                               \
                                       overload3, overload2, overload1, ""        \
                                      )(__VA_ARGS__)                              \
                               )

#endif /* _MACROUTILS_H_ */
