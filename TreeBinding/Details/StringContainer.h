/*!
 *  \file  StringContainer.h
 *  \brief StringContainer class and macroses definitions
 */

#ifndef _STRINGCONTAINER_H_
#define _STRINGCONTAINER_H_

/*!
 * \brief Concatenate two tokens
 */
#define TREE_BINDING_DETAILS_TOKEN_PASTE(x, y) x##y

/*!
 * \brief Concatenate two tokens
 */
#define TREE_BINDING_DETAILS_CONCAT(x, y) TREE_BINDING_DETAILS_TOKEN_PASTE(x, y)

/*!
 * \brief   Build unique string container name
 * \details Concatenate token "__StringContainer__" and current line number
 */
#define TREE_BINDING_DETAILS_STRING_CONTAINER_NAME \
    TREE_BINDING_DETAILS_CONCAT(__StringContainer__, __LINE__)

/*!
 * \brief     Unique string container
 * \details   template can't has string literal arguments, it's passed by it wrapper
 * \param[in] String, which will hold in container
 */
#define TREE_BINDING_DETAILS_STRING_CONTAINER(str)   \
  struct TREE_BINDING_DETAILS_STRING_CONTAINER_NAME  \
  {                                                  \
    static const char* const getName()               \
    {                                                \
      return str;                                    \
    }                                                \
  }

#endif /* _STRINGCONTAINER_H_ */
