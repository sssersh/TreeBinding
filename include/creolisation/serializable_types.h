/*! 
 *  \file  serializable_types.h
 *  \brief Ser Interface file for boost tree and classes/structures data binding
 */

#ifndef _CREOLISATION_SERIALIZABLE_TYPES_H_
#define _CREOLISATION_SERIALIZABLE_TYPES_H_

#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/serialization/access.hpp>
#include "creolisation/Details/ArchiveSerializerDecl.h"
#include "creolisation/Details/Core/Node.h"
#include "creolisation/Details/Core/NodeIterator.h"
#include "creolisation/Details/Core/BasicTree.h"
#include "creolisation/Details/Core/Tree.h"

namespace creolisation
{

/*! 
 *  \brief     Declaration of stub for string translator
 *  \param[in] type Type of translated value
 */
#define CREOLISATION_TRANSLATOR_TO_STRING_STUB(type)   \
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
#define CREOLISATION_TABLE_TRANSLATORS_DECLARATION(type, table) \
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
 * \details When passed 2 arguments, expanded to call of CREOLISATION_DETAILS_NODE_2 macro.
 *          When passed 3 arguments, expanded to call of CREOLISATION_DETAILS_NODE_3 macro.
 * \note    Name used as first parameter, because this macro can used in wrapper macro with constant name ("", for example),
 *          and it's necessary to pass another 2 parameters from wrapper to this macro. If use name as second parameter (between
 *          type and num, passing another 2 parameters became impossible.
 * \warning Each macro call should be placed in different lines
 * \param   ... 1. Node name.
 *              2. Node's data type
 *              3. Node are optional/mandatory (optional parameter)
 */
#define CREOLISATION_FIELD(...) CREOLISATION_DETAILS_NODE(creolisation::Details::ContainerRequired::NO, __VA_ARGS__)

#define CREOLISATION_FIELD_ARRAY(...) CREOLISATION_DETAILS_NODE(creolisation::Details::ContainerRequired::YES, __VA_ARGS__)

/*!
 * \brief  Define structure of tree
 * \tparam type Name of this type (in code)
 * \tparam name Name of tree (in file). "type" by default (optional parameter).
 */
#define CREOLISATION_TYPE(...) CREOLISATION_DETAILS_TYPE(__VA_ARGS__)

} /* namespace data_binding */

#include "creolisation/Details/TableParser.h"
#include "creolisation/Details/PtreeWriter.h"
#include "creolisation/Details/ArchiveSerializer.h"



#endif /* _CREOLISATION_SERIALIZABLE_TYPES_H_ */
