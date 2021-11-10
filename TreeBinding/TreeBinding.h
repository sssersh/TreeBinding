/*! 
 *  \file TreeBinding.h
 *  \brief Interface file for boost tree and classes/structures binding
 */

#ifndef _TREE_BINDING_H_
#define _TREE_BINDING_H_

#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/serialization/access.hpp>
#include "TreeBinding/Details/Parsers/ArchiveSerializerDecl.h"
#include "TreeBinding/Details/Node.h"
#include "TreeBinding/Details/NodeIterator.h"
#include "TreeBinding/Details/BasicTree.h"

namespace TreeBinding
{

/*! 
 *  \brief     Declaration of stub for string translator
 *  \param[in] type Type of translated value
 */
#define TREE_BINDING_TRANSLATOR_TO_STRING_STUB(type)   \
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
#define TREE_BINDING_TABLE_TRANSLATORS_DECLARATION(type, table) \
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
 * \details When passed 2 arguments, expanded to call of TREE_BINDING_DETAILS_NODE_3 macro.
 *          When passed 3 arguments, expanded to call of TREE_BINDING_DETAILS_NODE_4 macro.
 * \note    Name used as first parameter, because this macro can used in wrapper macro with constant name ("", for example),
 *          and it's necessary to pass another 2 parameters from wrapper to this macro. If use name as second parameter (between
 *          type and num, passing another 2 parameters became impossible.
 * \warning Each macro call should be placed in different lines
 * \param   ... 1. Node name.
 *              2. Node's data type
 *              3. Node are optional/mandatory
 */
#define TREE_NODE(...) TREE_BINDING_DETAILS_NODE(__VA_ARGS__)



/*!
 * \brief  Tree
 * \tparam T Structure, which represent tree (derived of current struct)
 * \tparam NameContainer Container with name of tree
 */
template<typename T, typename NameContainer>
struct Tree : public BasicTree
{
  Tree();
  typedef NameContainer NameContainer_;

private:
  template<int diff, int basicNodeSize>
  struct CheckSize;

  // Replace it by 
//  template<typename T1 = CheckSize<sizeof(T) - sizeof(Tree<NameContainer, T>), Details::NodeDataSize>>
  static void checkSize() {}

//  friend class boost::serialization::access;
};

/*!
 * \brief  Define structure of tree
 * \tparam type Name of this type (in code)
 * \tparam name Name of tree (in file). "type" by default.
 */
#define TREE_TREE(...) TREE_BINDING_DETAILS_TREE(__VA_ARGS__)

} /* namespace TreeBinding */

#include "TreeBinding/Details/Parsers/TableParser.h"
#include "TreeBinding/Details/Parsers/PtreeWriter.h"
#include "TreeBinding/Details/Parsers/ArchiveSerializer.h"

// Function definitions file
#include "TreeBinding/Details/TreeBindingDefs.h"

#endif /* _TREE_BINDING_H_ */
