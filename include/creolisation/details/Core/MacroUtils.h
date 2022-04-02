/*!
 *  \file  MacroUtils.h
 *  \brief Utils for macroses
 */

#ifndef _MACROUTILS_H_
#define _MACROUTILS_H_

/*!
 * \brief Concatenate two tokens
 */
#define CREOLISATION_DETAILS_TOKEN_PASTE(x, y) x##y

/*!
 * \brief Concatenate two tokens
 */
#define CREOLISATION_DETAILS_CONCAT(x, y) CREOLISATION_DETAILS_TOKEN_PASTE(x, y)

/*!
 * \brief     Macro for expand multiply parameters, because stupid MSVC passed __VA_ARGS__ as single parameter
 * \param[in] __VA_ARGS__ macro
 * \return    __VA_ARGS__ expanded to multiply parameters
 */
#define CREOLISATION_DETAILS_EXPAND( x ) x

/*!
 * \brief   Choose overloaded macro
 * \details First args - arguments, passes to target macro. After it passed overloaded macroses in descent order.
 */
#define CREOLISATION_DETAILS_GET_MACRO(_1, _2, _3, _4, TARGET_MACRO, ...) TARGET_MACRO

/*!
 * \brief    Overload macro
 * \details  Pass empty string to CREOLISATION_DETAILS_GET_MACRO() to avoid error
 *           "ISO C++11 requires at least one argument for the "..." in a variadic macro"
 * \warning  Overload macroses only with 1, 2, 3 or 4 arguments
 * \param[in] overload1 Overload with 1 argument
 * \param[in] overload2 Overload with 2 argument
 * \param[in] overload3 Overload with 3 argument
 * \param[in] overload4 Overload with 3 argument
 * \param[in] ... arguments of overloaded macro
 */
#define CREOLISATION_DETAILS_OVERLOAD_MACRO(overload1, overload2, overload3, overload4, ...) \
    CREOLISATION_DETAILS_EXPAND(                                                             \
        CREOLISATION_DETAILS_GET_MACRO(__VA_ARGS__,                                          \
                                       overload4, overload3, overload2, overload1, ""        \
                                      )(__VA_ARGS__)                                         \
                               )

#endif /* _MACROUTILS_H_ */
