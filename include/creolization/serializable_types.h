/*! 
 *  \file  serializable_types.h
 *  \brief Ser Interface file for boost tree and classes/structures data binding
 */

#ifndef _CREOLIZATION_SERIALIZABLE_TYPES_H_
#define _CREOLIZATION_SERIALIZABLE_TYPES_H_

#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/serialization/access.hpp>
#include "creolization/Details/ArchiveSerializerDecl.h"
#include "creolization/Details/Core/Node.h"
#include "creolization/Details/Core/NodeIterator.h"
#include "creolization/Details/Core/BasicTree.h"
#include "creolization/Details/Core/Tree.h"

namespace creolization
{

/*! 
 *  \brief     Declaration of stub for string translator
 *  \param[in] type Type of translated value
 */
#define CREOLIZATION_TRANSLATOR_TO_STRING_STUB(type)   \
  template<>                                           \
  std::string Translator::toString(const type * const) \
  {                                                    \
    throw std::runtime_error                           \
      ("Conversion to string not implementeted for" #type); \
  }

/*! 
 *  \brief     Declaration of string <-> target type translators
 *  \param[in] type Type of translated value
 *  \param[in] table boost bimap, which map string value <-> target type
 */
#define CREOLIZATION_TABLE_TRANSLATORS_DECLARATION(type, table) \
template<>                                                      \
void Translator::fromString(std::string const &str,             \
                            type* const value)                  \
{                                                               \
  *value = table.left.find(str)->second;                        \
}                                                               \
template<>                                                      \
std::string Translator::toString(const type* const value)       \
{                                                               \
  return table.right.find(*value)->second;                      \
}

/*!
 * \copydoc NodesNum
 * \note    Declared in private, because is necessary for forward declaration
 */
struct NodesNum;

/*! 
 * \brief   Declaration of field which binds with tree node
 * \details When passed 2 arguments, expanded to call of CREOLIZATION_DETAILS_NODE_2 macro.
 *          When passed 3 arguments, expanded to call of CREOLIZATION_DETAILS_NODE_3 macro.
 * \note    Name used as first parameter, because this macro can used in wrapper macro with constant name ("", for example),
 *          and it's necessary to pass another 2 parameters from wrapper to this macro. If use name as second parameter (between
 *          type and num, passing another 2 parameters became impossible.
 * \warning Each macro call should be placed in different lines
 * \param   ... 1. Node name.
 *              2. Node's data type
 *              3. Node are optional/mandatory (optional parameter)
 */
#define CREOLIZATION_FIELD(...) CREOLIZATION_DETAILS_NODE(creolization::Details::ContainerRequired::NO, __VA_ARGS__)

#define CREOLIZATION_FIELD_ARRAY(...) CREOLIZATION_DETAILS_NODE(creolization::Details::ContainerRequired::YES, __VA_ARGS__)

/*!
 * \brief  Define structure of tree
 * \tparam type Name of this type (in code)
 * \tparam name Name of tree (in file). "type" by default (optional parameter).
 */
#define CREOLIZATION_TYPE(...) CREOLIZATION_DETAILS_TYPE(__VA_ARGS__)

} /* namespace data_binding */

#include "creolization/Details/TableParser.h"
#include "creolization/Details/PtreeWriter.h"
#include "creolization/Details/ArchiveSerializer.h"



#endif /* _CREOLIZATION_SERIALIZABLE_TYPES_H_ */
