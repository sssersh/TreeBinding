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
#include "TreeBinding/Details/TreeBindingDetails.h"
#include "TreeBinding/Details/Translator.h"

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
      (TREE_BINDING_CONCAT("Conversion to string not implementeted for", #type)); \
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
#define TREE_NODE(...) TREE_BINDING_DETAILS_NODE_COMMON(__VA_ARGS__)

/*!
 * \brief Basic tree
 */
class BasicTree
{

protected:

  BasicTree() = delete;
  BasicTree(const char* const _name);
  Details::BasicNodeData& operator[](size_t const index) const;
  Details::BasicNodeData& getSameNode(const Details::BasicNodeData &rhs) const;

  size_t            nodesNum; /*!< Number of fields in current tree */
  const char* const name;     /*!< Name of tree                     */

  struct NodeIterator;

  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
public:
 
  NodeIterator begin() const;
  NodeIterator end()   const;

  void parsePtree(boost::property_tree::ptree &tree, bool isRoot = true);
  void parseTable(std::vector<std::vector<std::wstring>> &table,
    std::function<boost::optional<size_t>(const std::string&)> const &nameToIndex,
    std::pair<size_t, size_t> const &rows);

  // true for XML, false for JSON
  void writePtree(boost::property_tree::ptree &tree, const bool isRoot = true) const;

  bool operator== (BasicTree const &rhs) const;
  virtual BasicTree& operator= (BasicTree const &rhs);
  bool isLeafsValid() const;
  bool isMandatoryLeafsValid() const;
  void reset();
  const char* getKeyNodeName() const;
  void copyLeafs(BasicTree const &rhs);

  bool isValid() const;
  bool containValidNodes() const;

//  template<typename, typename> friend class Tree;
};

/*!
 * \brief Iterator for iterate over fields in tree
 */
struct BasicTree::NodeIterator : public std::iterator<std::input_iterator_tag, Details::BasicNodeData>
{
  NodeIterator() = default;
  NodeIterator(const NodeIterator&) = default;
  NodeIterator& operator=(const NodeIterator&) = default;

  bool operator!=(const NodeIterator&) const;
  bool operator==(const NodeIterator&) const;
  NodeIterator& operator+(int const index);
  NodeIterator& operator++();
  Details::BasicNodeData& operator*() const;
  Details::BasicNodeData* operator->() const;

  Details::BasicNodeData* ptr;
};

/*!
 * \brief serialize BasicTree by boost serializer
 * \param[out] ar      Serialized archive
 * \param[in]  version Data structure version
 */
// TODO: move to defs file
template<class Archive>
void BasicTree::serialize(Archive & ar, const unsigned int version)
{
  for (auto nodeIt = this->begin(); nodeIt != this->end(); ++nodeIt)
  {
    ar & (*nodeIt);
  }
}

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
#define TREE_TREE(...) TREE_BINDING_DETAILS_TREE_COMMON(__VA_ARGS__) 

} /* namespace TreeBinding */

#include "TreeBinding/Details/Parsers/TableParser.h"
#include "TreeBinding/Details/Parsers/PtreeWriter.h"
#include "TreeBinding/Details/Parsers/ArchiveSerializer.h"

// Function definitions file
#include "TreeBinding/Details/TreeBindingDefs.h"

#endif /* _TREE_BINDING_H_ */
