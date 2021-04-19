/*! 
 *  \file TreeBinding.h
 *  \brief Interface file for boost tree and classes/structures binding
 */

#ifndef _TREE_BINDING_H_
#define _TREE_BINDING_H_

#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include "TreeBindingDetails.h"

namespace TreeBinding
{

/*! 
 * \brief Declaration of function, which translate string value to target type
 */
struct Translator
{
  static bool isNumber(const std::string& s);

  /*! 
   *  \brief  Translate string value to target type
   *  \note   Not used direct get<T2> from boost tree, because it's necessary for forward instance of boost translator_between
   *  \tparam Target type
   *  \param  str   String representation of value
   *  \param  value Pointer to target value
   */
  template<typename T>
  static void fromString(std::string const &str, T* value) throw(std::runtime_error);
};

/*!
 *  \brief   Type for store integer fields of Object
 */
typedef long Integer;

// Declared in private, because is necessary for forward declaration
struct NodesNum;

/*! 
 * \brief   Declaration of field which binds with tree node
 * \details When passed 2 arguments, expanded to call of TREE_BINDING_DETAILS_NODE_3 macro.
 *          When passed 3 arguments, expanded to call of TREE_BINDING_DETAILS_NODE_4 macro.
 * \warning Each macro call should be placed in different lines
 * \param   ... 1. Node name.
 *              2. Node's data type
 *              3. Node are optional/mandatory
 */
#define TREE_NODE(...) TREE_BINDING_DETAILS_NODE_COMMON(__VA_ARGS__)

typedef struct WrongChildsNumException : public std::runtime_error
{
  WrongChildsNumException(std::string const &objectName, NodesNum const requiredNum, int const actuallyNum);
} WrongChildsNumException;



typedef class BasicTree
{

protected:

  BasicTree() = delete;
  BasicTree(const char* const _name);

  size_t            nodesNum; /*!< Number of fields in current tree */
  const char* const name;     /*!< Name of tree                     */

  typedef struct NodeIterator
  {
    NodeIterator() = default;
    NodeIterator(const NodeIterator&) = default;
    NodeIterator& operator=(const NodeIterator&) = default;

    bool operator!=(const NodeIterator&) const;
    NodeIterator& operator++();
    Details::BasicNodeData* operator*() const;

    Details::BasicNodeData* ptr;
  } NodeIterator;

public:
 
  NodeIterator begin() const;
  NodeIterator end()   const;

  void parsePtree(boost::property_tree::ptree &tree, bool isRoot = true);
  void parseTable(std::vector<std::vector<std::wstring>> table, std::function<size_t(std::string &const)> nameToIndex);

  bool operator== (BasicTree const &rhs) const;
  bool isLeafsValid() const;
  void reset();

  bool isValid() const
  {
    return true;
  }

//  template<typename, typename> friend class Tree;
} BasicTree;


template<typename NameContainer, typename T>
struct Tree : public BasicTree
{
  Tree();
  typedef NameContainer NameContainer_;
};

/*!
 * \brief  Define structure of tree
 * \tparam name Name of tree (in file)
 * \tparam type Name of this type (in code)
 */
#define TREE_TREE(name, type)                  \
  TREE_BINDING_DETAILS_STRING_CONTAINER(name); \
  struct type : public TreeBinding::Tree < TREE_BINDING_DETAILS_STRING_CONTAINER_NAME, type >

} /* namespace TreeBinding */

// Function definitions file
#include "TreeBindingDefs.h"

#endif /* _TREE_BINDING_H_ */
