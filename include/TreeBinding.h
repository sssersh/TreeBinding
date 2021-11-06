/*!
 *  \file TreeBinding.h
 *  \brief TODO
 */

#ifndef _TREE_BINDING_H_
#define _TREE_BINDING_H_

#include <algorithm>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <cctype>
#include <cinttypes>
#include <codecvt>
#include <iostream>
#include <locale>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>
























namespace TreeBinding
{

class BasicTree;

template<typename, typename>
struct Tree;

/*!
 * \brief  Containter for subtree elements
 * \tparam Type of subtree elements
 */
template<typename T>
using SubtreesSet = std::vector<T>;

template<typename T>
using Row = std::vector<T>;

template<typename T>
using Table = std::vector<Row<T>>;

// second index include in range
using RowsRange = std::pair<size_t, size_t>;

namespace Details
{

class BasicNodeData;

template<typename>
class NodeData;

struct TableParser;
class PtreeWriter;

template<typename>
struct is_subtrees_set : std::false_type {};

template<typename T>
struct is_subtrees_set<SubtreesSet<T>> : std::true_type{};

} /* namespace Details */

} /* namespace TreeBinding */




namespace TreeBinding
{

namespace Details
{

class Archivable
{
public:

  virtual void serializeData(boost::archive::text_iarchive & ar, const unsigned int version) = 0;
  virtual void serializeData(boost::archive::text_oarchive & ar, const unsigned int version) = 0;

};

} /* namespace Details */

} /* namespace TreeBinding */













namespace TreeBinding
{

/*!
 *  \brief Type for represent number of fields in Tree object
 */
struct NodesNum
{
  typedef int32_t ValueType;

  static const ValueType NOT_SPECIFIED = -1; /*!< Number of nodes not specified         */
  static const ValueType MORE_THAN_0   = -2; /*!< Number of nodes should be more than 0 */

  NodesNum() = delete;
  NodesNum(ValueType const value) { this->value = value; }
  operator ValueType() const { return value; }
//  ValueType operator<(ValueType const rhs) { return value < rhs; }

  bool isCertain() const;
  std::string toString() const;

private:
  ValueType value;
};

namespace Details
{

/*!
 * \brief Default path delimeter in ptree
 */
static const char DEFAULT_DELIMETER = '/';

// Base class used for iteration in Tree
class BasicNodeData : public Archivable
{
public:

  BasicNodeData() = delete;
  BasicNodeData(const char* const name, NodesNum::ValueType const num, bool const isLeaf);
  BasicNodeData(BasicNodeData const &rhs) = delete;
  virtual ~BasicNodeData() = default;
  void operator= (BasicNodeData const &rhs);

  virtual void* getValue ()                            const = 0;
  virtual void  reset    ()                                  = 0;
  virtual bool  compare  (BasicNodeData const &rhs)    const = 0;
  virtual void  copy     (BasicNodeData const &rhs)          = 0;
  virtual void  parsePtree(boost::property_tree::ptree &tree, const char pathDelimeter = Details::DEFAULT_DELIMETER) = 0;
  virtual void parseTable(Table<std::wstring> &table,
                          std::function<boost::optional<size_t>(const std::string&)> const &nameToIndex,
                          RowsRange const &rows) = 0;

  virtual void writePtree(boost::property_tree::ptree &tree) const = 0;
  bool operator== (BasicNodeData const &rhs) const;

  const char* const name;        /*!< Node name                        */
  const NodesNum    requiredNum; /*!< Required number of nodes in tree */
  bool              validity;    /*!< Value of node is valid           */
  bool              isLeaf;      /*!< Value is leaf (not subtree containter and not Tree) */

  template <typename T> operator T&();


protected:
  friend class boost::serialization::access;

  friend class NodeTableParser;
  friend class PtreeWriter;
  friend class BasicTree;
  template <typename, typename> friend struct Tree;
};

// for cast unrefenced iterator to target type
template<typename T>
BasicNodeData::operator T&()
{
  return static_cast<T&>(*static_cast<NodeData<T>*>(this));
}
 
// Store pointer to value, not value, to fix size of structure
template<typename DataType>
class NodeData : public BasicNodeData
{
public:
  DataType* value; /*!< Value of parameter */

  NodeData() = delete;
  NodeData(const char* const _name, NodesNum::ValueType const _requiredNum);
  NodeData(NodeData const &rhs);
  virtual ~NodeData();
  virtual const DataType& operator= (DataType const &value);
  virtual const DataType& operator= (DataType const &&value);
//  virtual operator DataType() const;
  virtual operator const DataType&() const;
  virtual bool operator==(DataType const &rhs);

  virtual void  reset   ()                                  override final;
  virtual void  copy    (BasicNodeData const &rhs)          override final;

  virtual void  parsePtree(boost::property_tree::ptree &tree, 
                           const char pathDelimeter = Details::DEFAULT_DELIMETER) override final;
  virtual void parseTable(Table<std::wstring> &table,
                          std::function<boost::optional<size_t>(const std::string&)> const &nameToIndex,
                          RowsRange const &rows) override final;

  virtual void writePtree(boost::property_tree::ptree &tree) const override final;
  virtual void serializeData(boost::archive::text_iarchive & ar, const unsigned int version) override final;
  virtual void serializeData(boost::archive::text_oarchive & ar, const unsigned int version) override final;

  // [] is accessible only when DataType is container
//  template<typename KeyType, typename T = DataType>
//  typename T::const_iterator operator[](const KeyType &key) const;

  virtual void* getValue()                            const override final;

  using ValueType = DataType;

protected:

  const NodeData& operator= (NodeData const &rhs);

  // define separate functions for implementation, because SFINAE work only for overloading
  template<typename T = DataType>
  typename std::enable_if_t<!is_subtrees_set<T>::value && !std::is_base_of<BasicTree, DataType>::value>
    parsePtreeImpl(boost::property_tree::ptree &tree, const char pathDelimeter = Details::DEFAULT_DELIMETER);

  template<typename T = DataType>
  typename std::enable_if_t<is_subtrees_set<T>::value>
    parsePtreeImpl(boost::property_tree::ptree &tree, const char pathDelimeter = Details::DEFAULT_DELIMETER);

  template<typename T = DataType>
  typename std::enable_if_t<std::is_base_of<BasicTree, T>::value>
    parsePtreeImpl(boost::property_tree::ptree &tree, const char pathDelimeter = Details::DEFAULT_DELIMETER);

  template<typename T = DataType>
  std::enable_if_t<!TreeBinding::Details::is_subtrees_set<T>::value>
    resetImpl();

  template<typename T = DataType>
  std::enable_if_t<TreeBinding::Details::is_subtrees_set<T>::value>
    resetImpl();

  virtual bool compare (BasicNodeData const &rhs) const override;

  friend class NodeTableParser;
  friend class PtreeWriter;

  typedef boost::property_tree::ptree::path_type path;
}; /* class NodeData */

// defined here for use in Tree constructor
// declared not in class for get opportunity of declaration in header
static const size_t NodeDataSize = sizeof(BasicNodeData) + sizeof(void*);

/*! 
 *  \brief  Tree node
 *  \tparam NameContainer String wrapper class, which contain field name
 *  \tparam DataType      NodeData data type
 *  \tparam RequiredNum   Required number of fields
 */
template< typename NameContainer, 
          typename DataType     , 
          NodesNum::ValueType RequiredNum = NodesNum::MORE_THAN_0
        >
struct Node final : public NodeData< std::conditional_t< std::is_base_of<BasicTree, DataType>::value && RequiredNum != 1,
                                                         SubtreesSet< DataType >,
                                                         DataType
                                                       > 
                                   >
{
  using DeducedDataType = typename
    std::conditional_t< std::is_base_of<BasicTree, DataType>::value && RequiredNum != 1,
                        SubtreesSet< DataType >,
                        DataType
                      >;

  Node() : NodeData<DeducedDataType>(NameContainer::getName(), RequiredNum) {};
//  template<typename T = std::remove_cv<DeducedDataType>::type>
  const DeducedDataType& operator=(const DeducedDataType& rhs)
  {
    this->validity = true;
    return *this->value = rhs;
  }
//  template<typename T = std::remove_cv<DeducedDataType>::type>
  const DeducedDataType& operator=(const DeducedDataType&& rhs)
  {
    this->validity = true;
    return *this->value = rhs;
  }
  Node& operator=(const Node&) = default;

  /*
  template<typename KeyType, typename T = DeducedDataType>
  typename T::const_iterator operator[](const KeyType &key) const
  {
    return this->NodeData<DeducedDataType>::operator[](key);
  }
   */

  DeducedDataType& operator*() { return *this->value; };
  const DeducedDataType& operator*() const { return *this->value; };

  DeducedDataType* const operator->() { return this->value; };
  const DeducedDataType* const operator->() const { return this->value; };
};

struct AssertName 
{
  static const char* const getName()
  {
    return "";
  }
};
static_assert(sizeof(Node<AssertName, int, 0>) == NodeDataSize, "Fatal error: incorrect alignment in Node.");

#define TREE_BINDING_DETAILS_TOKEN_PASTE(x, y) x##y

#define TREE_BINDING_CONCAT(x,y) TREE_BINDING_DETAILS_TOKEN_PASTE(x,y)

// template can't has string literal arguments, it's passed by wrapper
#define TREE_BINDING_DETAILS_STRING_CONTAINER_NAME TREE_BINDING_CONCAT(__StringContainer__, __LINE__)

#define TREE_BINDING_DETAILS_STRING_CONTAINER(str)   \
  struct TREE_BINDING_DETAILS_STRING_CONTAINER_NAME  \
  {                                                  \
    TREE_BINDING_DETAILS_STRING_CONTAINER_NAME() {}; \
    static const char* const getName()               \
    {                                                \
      return str;                                    \
    }                                                \
  }

/*! 
 *  \copydoc TREE_BINDING_DETAILS_NODE_2()
 *  \param[in] num Required number of fields
 */
#define TREE_BINDING_DETAILS_NODE_3(paramName, dataType, num) \
  TREE_BINDING_DETAILS_STRING_CONTAINER(paramName);           \
  TreeBinding::Details::Node < TREE_BINDING_DETAILS_STRING_CONTAINER_NAME, dataType, num >

/*! 
 *  \brief     Declaration of reflection field (mandatory)
 *  \details   Declare and pass to field specialization wrapper class with uniq name, which contain name of field
 *  \warning   Each macro call should be placed in different lines
 *  \param[in] paramName Name of field
 *  \param[in] dataType Underlied type of field
 */
#define TREE_BINDING_DETAILS_NODE_2(paramName, dataType) TREE_BINDING_DETAILS_NODE_3(paramName, dataType, TreeBinding::NodesNum::MORE_THAN_0)

/*!
 * \brief     Macro for expand multiply parameters, because stupid MSVC passed __VA_ARGS__ as single parameter
 * \param[in] __VA_ARGS__ macro
 * \return    __VA_ARGS__ expanded to multiply parameters
 */
#define TREE_BINDING_DETAILS_EXPAND( x ) x

/*!
 * \brief Choose necessary overloaded macro (with 2 or 3 parameters) 
 */
#define TREE_BINDING_DETAILS_NODE_GET_MACRO(_1, _2, _3, TARGET_MACRO, ...) TARGET_MACRO

#define TREE_BINDING_DETAILS_NODE_COMMON(...)                       \
  TREE_BINDING_DETAILS_EXPAND(                                      \
    TREE_BINDING_DETAILS_NODE_GET_MACRO(__VA_ARGS__,                \
                                       TREE_BINDING_DETAILS_NODE_3, \
                                       TREE_BINDING_DETAILS_NODE_2  \
                                      )(__VA_ARGS__)                \
                             )



/*! 
 *  \copydoc TREE_BINDING_DETAILS_TREE_1()
 *  \param[in] name Name of tree
 */
#define TREE_BINDING_DETAILS_TREE_2(type, name) \
  TREE_BINDING_DETAILS_STRING_CONTAINER(name);  \
  struct type : public TreeBinding::Tree < type, TREE_BINDING_DETAILS_STRING_CONTAINER_NAME >


#define TREE_BINDING_DETAILS_TREE_1(type) TREE_BINDING_DETAILS_TREE_2(type, #type)

#define TREE_BINDING_DETAILS_TREE_GET_MACRO(_1, _2, TARGET_MACRO, ...) TARGET_MACRO


#define TREE_BINDING_DETAILS_TREE_COMMON(...)                       \
  TREE_BINDING_DETAILS_EXPAND(                                      \
    TREE_BINDING_DETAILS_TREE_GET_MACRO(__VA_ARGS__,                \
                                       TREE_BINDING_DETAILS_TREE_2, \
                                       TREE_BINDING_DETAILS_TREE_1  \
                                      )(__VA_ARGS__)                \
                             )

} /* namespace Details */

} /* namespace TreeBinding */




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
   *  \param[in]  str   String representation of value
   *  \param[out] value Pointer to target value
   */
  template<typename T>
  // TODO: T as return type
  static void fromString(std::string const &str, T* const value);

  /*! 
   *  \brief     Translate target type to string value
   *  \tparam    Target type
   *  \param[in] value Pointer to target value
   *  \return    String representation of value
   */
  template<typename T>
  static std::string toString(const T* const value);

};

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
 *  \brief Type for store integer fields of Tree
 */
typedef int Integer;

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
 * \brief Exception for signal wrong number of childs elements in tree
 */
struct WrongChildsNumException : public std::runtime_error
{
  WrongChildsNumException(std::string const &nodeName, NodesNum const requiredNum, int const actuallyNum);
};

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
 * \tparam name Name of tree (in file)
 */
#define TREE_TREE(...) TREE_BINDING_DETAILS_TREE_COMMON(__VA_ARGS__) 

} /* namespace TreeBinding */















namespace TreeBinding
{

namespace Details
{

/*!
 * \brief NodeData parser from table 
 * \note  Use indexes for range, because sort table
 */
struct TableParser
{

  template<typename DataType>
  static typename std::enable_if_t<!is_subtrees_set<DataType>::value && !std::is_base_of<BasicTree, DataType>::value>
    parse(NodeData<DataType> &node,
          Table<std::wstring> const &table,
          std::function<boost::optional<size_t>(const std::string&)> const &columnNameToIndex,
          RowsRange const &rows);

  template<typename DataType>
  static typename std::enable_if_t<is_subtrees_set<DataType>::value>
    parse(NodeData<DataType> &node,
          Table<std::wstring> &table,
          std::function<boost::optional<size_t>(const std::string&)> const &columnNameToIndex,
          RowsRange const &rows);

  template<typename DataType>
  static typename std::enable_if_t<std::is_base_of<BasicTree, DataType>::value>
    parse(NodeData<DataType> &node,
          Table<std::wstring> &table,
          std::function<boost::optional<size_t>(const std::string&)> const &columnNameToIndex,
          RowsRange const &rows);

};

// parse leaf
template<typename DataType>
typename std::enable_if_t<!is_subtrees_set<DataType>::value && !std::is_base_of<BasicTree, DataType>::value>
TableParser::parse(NodeData<DataType> &node,
                   Table<std::wstring> const &table,
                   std::function<boost::optional<size_t>(const std::string&)> const &columnNameToIndex,
                   RowsRange const &rows)
{
  auto columnIndex = columnNameToIndex(std::string(node.name));
  if (!columnIndex && !node.requiredNum.isCertain())
  {
    node.validity = false;
    return;
  }

  auto rowIndex = rows.first; // all rows for this column must have same value, use first

  using convert_type = std::codecvt_utf8<wchar_t>;
  std::wstring_convert<convert_type, wchar_t> converter;

  const std::string str = converter.to_bytes(table[rowIndex][*columnIndex]);
  if (str.empty() && !node.requiredNum.isCertain())
  {
    node.validity = false;
  }
  else
  {
    try
    {
      Translator::fromString(str, node.value);
    }
    catch (std::exception const &) // can't convert from string to taget type
    {
      throw(std::out_of_range("Tree node " + std::string(node.name) + " contain wrong value: \"" + str +
        "\", could not convert to " + std::string(typeid(DataType).name()) + "\n"));
    }
    node.validity = true;
  }
}

// parse subtree array
template<typename DataType>
typename std::enable_if_t<is_subtrees_set<DataType>::value>
TableParser::parse(NodeData<DataType> &node,
                   Table<std::wstring> &table,
                   std::function<boost::optional<size_t>(const std::string&)> const &columnNameToIndex,
                   RowsRange const &rows)
{
  auto subtreesSet = (DataType*)(node.getValue()); // DataType = SubtreesSet<>

  typedef typename DataType::value_type SubtreeElementType;

  auto keyFieldName   = std::string(SubtreeElementType().getKeyNodeName());
  auto keyColumnIndex = columnNameToIndex(keyFieldName);

  if (!keyColumnIndex && !node.requiredNum.isCertain())
  {
    node.validity = false;
    return;
  }

  using convert_type = std::codecvt_utf8<wchar_t>;
  std::wstring_convert<convert_type, wchar_t> converter;

  auto _begin = table.begin() + rows.first;
  auto _end   = table.begin() + rows.second + 1;
  std::sort(_begin, _end, [&](Row<std::wstring> const &row1, Row<std::wstring> const &row2)
  {
    return row1[*keyColumnIndex] < row2[*keyColumnIndex];
  });

  std::map<std::string, RowsRange> uniqKeys;

  auto rangeBegin = table.begin() + rows.first;;
  auto rangeEnd = rangeBegin + 1;
  while (true)
  {
    if (rangeEnd == _end || rangeBegin->at(*keyColumnIndex) != rangeEnd->at(*keyColumnIndex))
    {
      uniqKeys.insert(std::pair<std::string, RowsRange>(
        converter.to_bytes(rangeBegin->at(*keyColumnIndex)),
        RowsRange(rows.first + std::distance(_begin, rangeBegin),
                  rows.first + std::distance(_begin, rangeEnd) - 1
        )
        ));
      if (rangeEnd == _end) break;
      else rangeBegin = rangeEnd++;
    }
    else ++rangeEnd;
  }

  for (auto &i : uniqKeys)
  {
    subtreesSet->emplace_back();
    auto& subtreeElement = subtreesSet->back();
    subtreeElement.parseTable(table, columnNameToIndex, i.second);
  }

  /*
  // Check num 
  if ((requiredNum.isCertain() && subtreesSet->size() != requiredNum) ||
    (NodesNum::MORE_THAN_0 == requiredNum && 0 == subtreesSet->size()))
  {
    throw(WrongChildsNumException(typeid(DataType).name(), requiredNum, subtreesSet->size()));
  }
  */

  node.validity = true;
}

template<typename DataType>
typename std::enable_if_t<std::is_base_of<BasicTree, DataType>::value>
TableParser::parse(NodeData<DataType> &node,
                   Table<std::wstring> &table,
                   std::function<boost::optional<size_t>(const std::string&)> const &columnNameToIndex,
                   RowsRange const &rows)
{
  std::cout << "Here";
}

} /* namespace Details */

} /* namespace TreeBinding */









namespace TreeBinding
{

namespace Details
{

/*!
 * \brief NodeData writer to ptree
 */
class PtreeWriter
{
public:

  template<typename DataType>
  static typename std::enable_if_t<!is_subtrees_set<DataType>::value && !std::is_base_of<BasicTree, DataType>::value>
  write(NodeData<DataType> const &node,
        boost::property_tree::ptree &tree);

  template<typename DataType>
  static typename std::enable_if_t<is_subtrees_set<DataType>::value>
    write(NodeData<DataType> const &node,
        boost::property_tree::ptree &tree);

  template<typename DataType>
  static typename std::enable_if_t<std::is_base_of<BasicTree, DataType>::value>
  write(NodeData<DataType> const &node,
        boost::property_tree::ptree &tree);

};

template<typename DataType>
typename std::enable_if_t<!is_subtrees_set<DataType>::value && !std::is_base_of<BasicTree, DataType>::value>
PtreeWriter::write(NodeData<DataType> const &node,
                   boost::property_tree::ptree &tree)
{
  if (node.validity)
  {
    const std::string str = Translator::toString(node.value);
    tree.add(boost::property_tree::path(node.name), str);
  }
}

template<typename DataType>
typename std::enable_if_t<is_subtrees_set<DataType>::value>
PtreeWriter::write(NodeData<DataType> const &node,
                   boost::property_tree::ptree &tree)
{
  if (node.validity)
  {
    auto subtreesSet = (DataType*)node.getValue(); // DataType = SubtreesSet<>

    boost::property_tree::ptree subtree;

    for (auto &i : *subtreesSet)
    {
      boost::property_tree::ptree entry;
      i.writePtree(entry);
      subtree.push_back(std::make_pair("", entry));
    }

    tree.add_child(node.name, subtree);
  }
}

template<typename DataType>
typename std::enable_if_t<std::is_base_of<BasicTree, DataType>::value>
PtreeWriter::write(NodeData<DataType> const &node,
                   boost::property_tree::ptree &tree)
{
  std::runtime_error("PtreeWriter::write Not implement!");
}

} /* namespace Details */

} /* namespace TreeBinding */










// Should be declared in global namespace
#define TREE_BINDING_STRONG_TYPEDEF_SERIALIZE_DECLARATION(type)          \
  namespace boost {                                                      \
  namespace serialization {                                              \
  template<class Archive>                                                \
  void serialize(Archive & ar, type & value, const unsigned int version) \
  {                                                                      \
    ar & value.t;                                                        \
  }                                                                      \
  } /* namespace serialization */                                        \
  } /* namespace boost        */

namespace boost {
namespace serialization {

using namespace TreeBinding;
using namespace Details;

/*
template<class Archive>
void serialize(Archive & ar, BasicTree& tree, const unsigned int version)
{
  for (auto nodeIt = tree.begin(); nodeIt != tree.end(); ++nodeIt)
  {
    ar & (*nodeIt);
  }
}

template<class Archive>
void serialize(Archive & ar, const BasicTree& tree, const unsigned int version)
{
  for (auto nodeIt = tree.begin(); nodeIt != tree.end(); ++nodeIt)
  {
    ar & (*nodeIt);
  }
}
*/

/*
template<class Archive, typename NameContainer, typename T>
typename std::enable_if_t<std::is_base_of<BasicTree, Tree<NameContainer, T>>::value>
serialize(Archive & ar, Tree<NameContainer, T>& tree, const unsigned int version)
{
  for (auto nodeIt = tree.begin(); nodeIt != tree.end(); ++nodeIt)
  {
    ar & (*nodeIt);
  }
}
*/

template<class Archive>
void serialize(Archive & ar, BasicNodeData& node, const unsigned int version)
{
  ar & node.validity;
  ar & node.isLeaf;
  node.serializeData(ar, version);
}

/*
// especially for BasicNodeData
template<class Archive, typename Node>
typename std::enable_if_t<std::is_same<BasicNodeData, Node>::value> 
serialize(Archive & ar, Node& tree, const unsigned int version)
{
  ar & validity;
  serializeData(ar, version);
}
*/

// for implementations of NodeData<>
template<class Archive, typename DataType>
typename std::enable_if_t<!std::is_same<BasicNodeData, NodeData<DataType>>::value && std::is_base_of<BasicNodeData, NodeData<DataType>>::value>
serialize(Archive & ar, NodeData<DataType>& node, const unsigned int version)
{
  ar & *(node.value);
}

} // namespace serialization
} // namespace boost

namespace TreeBinding
{

namespace Details
{

template<typename DataType>
void NodeData<DataType>::serializeData(boost::archive::text_oarchive & ar, const unsigned int version)
{
  boost::serialization::serialize(ar, *this, version);
}

template<typename DataType>
void NodeData<DataType>::serializeData(boost::archive::text_iarchive & ar, const unsigned int version)
{
  boost::serialization::serialize(ar, *this, version);
}

} /* namespace Details */

} /* namespace TreeBinding */




// Function definitions file







namespace TreeBinding
{

namespace Details
{

/*! 
 *  \brief  NodeData constructor
 *  \tparam DataType NodeData data type
 */
template<typename DataType>
NodeData<DataType>::NodeData(const char* const _name, NodesNum::ValueType const _requiredNum) :
  BasicNodeData(_name, _requiredNum, !is_subtrees_set<DataType>::value && !std::is_base_of<BasicTree, DataType>::value)
{
  value = new DataType(); // TODO: move to initialization list
};

/*! \brief     NodeData constructor
 *  \details   Used virtual function for copy, specialized for this template arguments
 *  \tparam    DataType  NodeData data type
 *  \param[in] rhs Right hand side
 */
template<typename DataType>
NodeData<DataType>::NodeData(NodeData const &rhs) :
  NodeData(rhs.name, rhs.requiredNum)
{
  this->copy(rhs);
};

/*! 
 *  \brief     NodeData assignment operator
 *  \details   Used virtual function for copy, specialized for this template arguments
 *  \tparam    DataType  NodeData data type
 *  \param[in] rhs Right hand side
 */
template<typename DataType>
const NodeData<DataType>& NodeData<DataType>::operator= (NodeData const &rhs)
{
  this->copy(rhs);
  return *this;
}

/*! 
 *  \brief  NodeData desstructor
 *  \tparam DataType NodeData data type
 */
template<typename DataType>
NodeData<DataType>::~NodeData()
{
  delete value;
}

/*! 
 *  \brief     NodeData assignment operator
 *  \tparam    DataType NodeData data type
 *  \param[in] value Right side value
 *  \return    Assigned value
 */
template<typename DataType>
const DataType& NodeData<DataType>::operator= (DataType const &value)
{
  *this->value = value;
  validity = true;
  return *this->value;
} 

template<typename DataType>
const DataType& NodeData<DataType>::operator= (DataType const &&value)
{
  *this->value = value;
  validity = true;
  return *this->value;
}

/*! 
 *  \brief  NodeData cast to underlying type operator
 *  \tparam DataType NodeData data type
 *  \return Value of field
 */
/*
template<typename DataType>
NodeData<DataType>::operator DataType() const
{
  return *value;
}
*/

template<typename DataType>
NodeData<DataType>::operator const DataType&() const
{
  return *value;
}

template<typename DataType>
bool NodeData<DataType>::operator==(DataType const &rhs)
{
  return *(this->value) == rhs;
}

/*! 
 *  \brief  Get pointer to value of field
 *  \tparam DataType NodeData data type
 *  \return Pointer to value of field
 */
template<typename DataType>
void* NodeData<DataType>::getValue() const
{
  return (void*)value;
}

/*! 
 *  \brief  Reset validity of value
 *  \tparam DataType NodeData data type
 */
/*
template<typename DataType>
void NodeData<DataType>::reset()
{
  validity = false;
}
*/

template<typename DataType>
void NodeData<DataType>::reset()
{
  resetImpl<DataType>();
}

template<typename DataType>
template<typename T>
typename std::enable_if_t<!TreeBinding::Details::is_subtrees_set<T>::value>
NodeData<DataType>::resetImpl()
{
  validity = false;
}

template<typename DataType>
template<typename T>
std::enable_if_t<TreeBinding::Details::is_subtrees_set<T>::value>
NodeData<DataType>::resetImpl()
{
  value->clear();
  validity = false;
}

/*! 
 *  \brief     Copy field with value and validity
 *  \tparam    DataType NodeData data type
 *  \param[in] rhs Right side value
 */
template<typename DataType>
void NodeData<DataType>::copy(BasicNodeData const &rhs)
{
  *(this->value) = *static_cast<DataType*>(rhs.getValue());
  validity = rhs.validity;
}

/*!
 *  \brief     Compare values of fields (validity ignored)
 *  \details   Used in compare operator in NodeData
 *  \tparam    DataType    NodeData data type
 *  \param[in] rhs   Right hand side
 *  \retval    true  Values of fields are same
 *  \retval    false Value of fields are different
 */
template<typename DataType>
bool NodeData<DataType>::compare(BasicNodeData const &rhs) const
{
  return *this->value == *static_cast<DataType*>(rhs.getValue());
}

/*
template<typename DataType>
template<typename KeyType, typename T>
typename T::const_iterator NodeData<DataType>::operator[](const KeyType &key) const
{
  static_assert(std::is_assignable< KeyType&, KeyType >::value, "Key type for operator [] should be assignable");

  return std::find_if(this->value->cbegin(), this->value->cend(), [&](typename DataType::const_reference element)
  {
    auto keyField = std::find_if(element.begin(), element.end(), [&](const BasicNodeData &node)
    {
      return dynamic_cast<const NodeData<KeyType>*>(&node);
    });
    if (keyField == element.end()) throw std::runtime_error(std::string("There are not key field in ") + typeid(typename DataType::value_type).name() + "\n");
    return static_cast<NodeData<KeyType>&>(*keyField) == key;
  });
}
*/

template<typename T>
void NodeData<T>::parsePtree(boost::property_tree::ptree &tree, const char pathDelimeter)
{
  parsePtreeImpl<T>(tree, pathDelimeter);
}

template<typename T>
void NodeData<T>::writePtree(boost::property_tree::ptree &tree) const
{
  PtreeWriter::write(*this, tree);
}

// parse leaf
template<typename DataType>
template<typename T>
typename std::enable_if_t<!is_subtrees_set<T>::value && !std::is_base_of<BasicTree, DataType>::value>
NodeData<DataType>::parsePtreeImpl(boost::property_tree::ptree &tree, const char pathDelimeter)
{
  std::string str;
  try
  {
    // Not used direct get<T>, because it's necessary for forward instance of boost translator_between
    str = tree.get<std::string>(path(this->name, pathDelimeter));
  }
  catch (boost::exception const &) // not found such node in ptree
  {
    if (requiredNum.isCertain() || (NodesNum::MORE_THAN_0 == requiredNum))
    {
      throw(WrongChildsNumException(std::string(this->name) + " (" + typeid(DataType).name() + ")", requiredNum, 0));
    }
  }

  if (str.empty() && !this->requiredNum.isCertain())
  {
    this->validity = false;
  }
  else
  {
    try
    {
      Translator::fromString(str, value);
    }
    catch (std::exception const &) // can't convert from string to taget type
    {
      throw(std::out_of_range("Tree node " + std::string(this->name) + " contain wrong value: \"" + str +
        "\", could not convert to " + std::string(typeid(DataType).name()) + "\n"));
    }
    validity = true;
  }
};

// parse subtree array
// pathDelimeter not used
template<typename DataType>
template<typename T>
typename std::enable_if_t<is_subtrees_set<T>::value>
NodeData<DataType>::parsePtreeImpl(boost::property_tree::ptree &tree, const char pathDelimeter)
{
  auto subtreesSet = (DataType*)this->getValue(); // DataType = SubtreesSet<>

  typedef typename DataType::value_type SubtreeElementType;

  if (!std::strcmp("", this->name)) // XML: array of same elements not stored in separate subtree
  {
    static const int XML_ATTR_SUBTREE_NUM = 1;

    auto elementName = SubtreeElementType::NameContainer_::getName();

    // XML contain 1 pseudo subtree - xmlattr, not reserve for it
    subtreesSet->reserve(tree.size() - XML_ATTR_SUBTREE_NUM);

    // find if and error, if not found 
    for (auto& j : tree)
    {
      if (!std::strcmp(j.first.c_str(), elementName))
      {
        subtreesSet->emplace_back(); 
        auto& subtreeElement = subtreesSet->back();
        subtreeElement.parsePtree(j.second, false);
      }
    }
  }
  else // for JSON name of array stored in Subtree
  {
    try
    {
      auto &subtree = tree.get_child(this->name);
      for (auto &j : subtree)
      {
        subtreesSet->emplace_back();
        auto& subtreeElement = subtreesSet->back();
        subtreeElement.parsePtree(j.second, false);
      }
    }
    catch (const std::exception &)
    {
      if (requiredNum.isCertain()) throw;
    }
  }

  /* Check num */
  if ((requiredNum.isCertain()            && subtreesSet->size() != requiredNum) ||
    (NodesNum::MORE_THAN_0 == requiredNum && 0 == subtreesSet->size()))
  {
    throw(WrongChildsNumException(typeid(DataType).name(), requiredNum, subtreesSet->size()));
  }
  
  validity = true;
};

// parse single subtree
template<typename DataType>
template<typename T>
typename std::enable_if_t<std::is_base_of<BasicTree, T>::value>
NodeData<DataType>::parsePtreeImpl(boost::property_tree::ptree &tree, const char pathDelimeter)
{
  auto subtree = (DataType*)this->getValue();
  size_t num = 0;

  if (!std::strcmp("", this->name)) // XML: array of same elements not stored in separate subtree
  {
    auto elementName = DataType::NameContainer_::getName();

    // find if and error, if not found 
    for (auto& j : tree)
    {
      if (!std::strcmp(j.first.c_str(), elementName))
      {
        subtree->parsePtree(j.second, false);
        num++;
      }
    }
  }
  else // for JSON name of array stored in Subtree
  {
    auto &tree_ = tree.get_child(this->name);
    subtree->parsePtree(tree_, false);
    num = 1;
  }

  /* Check num */
  if ((requiredNum.isCertain() && num != requiredNum) ||
      (NodesNum::MORE_THAN_0 == requiredNum && 0 == num))
  {
    throw(WrongChildsNumException(typeid(DataType).name(), requiredNum, num));
  }

  validity = true;
};

template<typename T>
void NodeData<T>::parseTable(std::vector<std::vector<std::wstring>> &table,
                             std::function<boost::optional<size_t>(const std::string&)> const &nameToIndex,
                             std::pair<size_t, size_t> const &rows)
{
  TableParser::parse(*this, table, nameToIndex, rows);
}

} /* namespace Details */

/*! \brief   Tree constructor
 *  \details Calculate number of values: size of Derived - size of current Tree / size of field specialization
 *  \tparam  Derived Derived class
 */
template<typename Derived, typename NameContainer>
Tree<Derived, NameContainer>::Tree() :
  BasicTree(NameContainer::getName())
{
  checkSize();
  nodesNum = (sizeof(Derived) - sizeof(Tree<NameContainer, Derived>)) / Details::NodeDataSize;
}

// for print size
template<int diff, int basicNodeSize>
struct CheckSize
{
  static_assert(diff % basicNodeSize == 0, "Tree contains incorrect nodes");
  static_assert((diff >= basicNodeSize || diff == 0), "Tree contains incorrect nodes"); // 0 - for empty tree
};

} /* namespace TreeBinding */











namespace TreeBinding
{

/*!
 * \brief     Check that string contain only digits
 * \param[in] s string for check
 * \retval    true string contain only digits
 * \retval    false otherwise
 */
bool Translator::isNumber(const std::string& s)
{
  auto it = s.cbegin();
  while (it != s.end() && std::isdigit(static_cast<unsigned char>(*it))) ++it;
  return !s.empty() && it == s.end();
}

/*! 
 *  \brief      Translate string value to integer
 *  \param[in]  str   String representation of value
 *  \param[out] value Pointer to integer value
 */
template<>
void Translator::fromString(std::string const &str, Integer* const value)
{
  if (isNumber(str)) *value = atol(str.c_str());
  else throw(std::out_of_range("Integer contain incorrect value: " + str + "\n"));
}

/*! 
 *  \brief      Translate string value to string value
 *  \param[in]  str   Source string
 *  \param[out] value Target string
 */
template<>
void Translator::fromString(std::string const &str, std::string* const value)
{
  *value = str;
}

/*!
 *  \brief     Translate integer to string value
 *  \param[in] value Pointer to integer value
 *  \return    String representation of value
 */
template<>
std::string Translator::toString(const Integer* const value)
{
  char buf[20];
  std::sprintf(buf, "%d", *const_cast<Integer*>(value));
  return std::string(buf);
}

/*!
 *  \brief     Translate string to string value
 *  \param[in] value Pointer to source value
 *  \return    Copy of source string
 */
template<>
std::string Translator::toString(const std::string* const value)
{
  return *value;
}

namespace Details
{

/*!
 * \brief     BasicNodeData constructor
 * \param[in] name        Name of node
 * \param[in] requiredNum Required numbers of node
 * \param[in] isLeaf      Node is leaf (not subtree container)
 */
BasicNodeData::BasicNodeData(const char* const name, NodesNum::ValueType const requiredNum, bool const isLeaf) :
  name(name),
  requiredNum(requiredNum),
  validity(false),
  isLeaf(isLeaf)
{}

/*!
 * \brief     BasicNodeData compare operator
 * \note      Compare nodes only if both are valid.
 *            Comparison function are declared in derived class.
 * \param[in] rhs Right hand side value
 * \retval    true Both are valid and are equals
 * \retval    false One/both are invalid or are not equals
 */
bool BasicNodeData::operator== (BasicNodeData const &rhs) const
{
  if (this->validity && rhs.validity)
  {
    return this->compare(rhs);
  }
  else return true;
}

/*!
 * \brief     BasicNodeData assignment operator
 * \note      Assignment function are declared in derived class.
 * \param[in] rhs Right hand side value
 */
void BasicNodeData::operator= (BasicNodeData const &rhs)
{
  this->copy(rhs);
}

} /* namespace Details */

/*!
 * \brief  Show that number of nodes is certain or not
 * \retval true Number of nodes is certain
 * \retval false Number of nodes is float
 */
bool NodesNum::isCertain() const
{
  return value >= 0;
}

/*!
 * \brief  Get number of nodes in string representation
 * \return Number of nodes in string representation
 */
std::string NodesNum::toString() const
{
  switch (value)
  {
  case NOT_SPECIFIED:
    return "not specified";
  case MORE_THAN_0:
    return "more than 0";
  default:
    return std::to_string(value);
  }
}

/*!
 * \brief     WrongChildsNumException constructor
 * \param[in] nodeName Node name
 * \param[in] requiredNum Required number of childs nodes
 * \param[in] actuallyNum Actual number of childs nodes
 */
WrongChildsNumException::WrongChildsNumException
  (std::string         const &nodeName , 
   NodesNum            const  requiredNum, 
   NodesNum::ValueType const  actuallyNum) :
   std::runtime_error("Invalid number of childs in node " + nodeName + ". Required: " + requiredNum.toString() +
                     ", present: " + std::to_string(actuallyNum))
{}

BasicTree::BasicTree(const char* const name) :
  name(name)
{}

/*!
 * \brief     Get node by index
 * \param[in] index Index of node
 * \return    Reference to node
 */
Details::BasicNodeData& BasicTree::operator[](size_t const index) const
{
  return *(this->begin() + index);
}

/*!
 * \brief Copy only leafs nodes
 * \param[in] rhs Right hand side value
 */
void BasicTree::copyLeafs(BasicTree const &rhs)
{
  /*
  for (auto &node : *this)
  {
    if (node.isLeaf) node = rhs.getSameNode(node);
  }
  */
  for (size_t i = 0; i < this->nodesNum; ++i)
  {
    if (rhs[i].isLeaf) (*this)[i] = rhs[i];
  }
}

/*!
 * \brief  Check that tree is valid
 * \note   Tree is valid if all nodes are valid
 * \retval true All nodes are valid
 * \retval false Otherwise
 */
bool BasicTree::isValid() const
{
  return std::all_of(this->begin(), this->end(), [](const Details::BasicNodeData &node) 
  { 
    return node.validity; 
  });
}

/*!
 * \brief  Check that tree contain valid nodes
 * \retval true Tree contain valid nodes
 * \retval false Otherwise
 */
bool BasicTree::containValidNodes() const
{
  return std::any_of(this->begin(), this->end(), [](const Details::BasicNodeData &node)
  {
    return node.validity;
  });
}

// deprecated
Details::BasicNodeData& BasicTree::getSameNode(const Details::BasicNodeData &rhs) const
{
  for (auto &node : *this)
  {
    if(typeid(node) == typeid(rhs)) return node;
  }
  throw(std::runtime_error("Cannot find same field"));
}


BasicTree::NodeIterator& BasicTree::NodeIterator::operator+(int const index)
{
  ptr = (Details::BasicNodeData*)((uint8_t*)ptr + Details::NodeDataSize * index);
  return *this;
}

BasicTree::NodeIterator& BasicTree::NodeIterator::operator++()
{
  return *this + 1;
}

bool BasicTree::NodeIterator::operator== (const BasicTree::NodeIterator& rhs) const
{
  return this->ptr == rhs.ptr;
}

//deprecated
bool BasicTree::NodeIterator::operator!= (const BasicTree::NodeIterator& rhs) const
{
  return !(*this == rhs);
}

Details::BasicNodeData& BasicTree::NodeIterator::operator*() const
{
  return *this->ptr;
}

Details::BasicNodeData* BasicTree::NodeIterator::operator->() const
{
  return this->ptr;
}

/*! 
 *  \brief   Get iterator on first node
 *  \details Calculate pointer as offset from this equals to size of BasicTree
 *  \return  Iterator on first node
 */
BasicTree::NodeIterator BasicTree::begin() const
{
  BasicTree::NodeIterator ret{};
  ret.ptr = (Details::BasicNodeData*)((uint8_t*)this + sizeof(*this));
  return ret;
}

/*! \brief   Get iterator on end node
 *  \warning Iterator contain pointer after last node. Only for equal, not unrefernce.
 *  \return  Iterator on end node
 */
BasicTree::NodeIterator BasicTree::end() const
{
  BasicTree::NodeIterator ret{}, beg;
  beg = begin();
  ret.ptr = (Details::BasicNodeData*)((uint8_t*)beg.ptr + nodesNum * Details::NodeDataSize);
  return ret;
}

/*! 
 *  \brief   Equal operator
 *  \note    Equal operator for Node compare values, only when both are valid. Otherwise, return true.
 *  \retval  true  Valid fields of derived are same
 *  \retval  false Valid fields of derived are different
 */
bool BasicTree::operator== (BasicTree const &rhs) const
{
  bool result = true;
  for (size_t i = 0; i < this->nodesNum; ++i)
  {
    result = result && ((*this)[i] == rhs[i]);
  }
  return result;
}

BasicTree& BasicTree::operator= (BasicTree const &rhs)
{
  for (size_t i = 0; i < this->nodesNum; ++i)
  {
    (*this)[i] = rhs[i];
  }
  return *this;
}

/*! \brief   Get validity
 *  \details Calculate validity as logical multiply of all fields of derived
 *  \warning Validity for childs not calculated
 *  \retval  true  All fields of derived Tree are valid
 *  \retval  false Not all fields of derived Tree are valid
 */
bool BasicTree::isLeafsValid() const
{
  return std::all_of(this->begin(), this->end(), [](const Details::BasicNodeData &node)
  {
    return node.isLeaf ? node.validity : true;
  });
}

bool BasicTree::isMandatoryLeafsValid() const
{
  return std::all_of(this->begin(), this->end(), [](const Details::BasicNodeData &node)
  {
    return (node.isLeaf && node.requiredNum != NodesNum::NOT_SPECIFIED ) ? node.validity : true;
  });
}

/*! \brief   Parse Tree from text representation (XML, JSON, etc.)
 *  \note    It's necessary to define ChildIterator functions and parseNode() function for type T.
 *  \param[in] tree Boost tree which represent current node and it's childs
 *  \param[in] isRoot Tree node is root node
 */
void BasicTree::parsePtree(boost::property_tree::ptree &tree, const bool isRoot)
{
  if (isRoot)
  {
    tree = tree.back().second;
  }

  /* Parse nodes */
  for (auto nodeIt = this->begin(); nodeIt != this->end(); ++nodeIt)
  {
    nodeIt->parsePtree(tree);
  }
}

void BasicTree::parseTable(std::vector<std::vector<std::wstring>> &table,
  std::function<boost::optional<size_t>(const std::string&)> const &nameToIndex,
  std::pair<size_t, size_t> const &rows)
{
  for (auto nodeIt = this->begin(); nodeIt != this->end(); ++nodeIt)
  {
    nodeIt->parseTable(table, nameToIndex, rows);
  }
}

void BasicTree::writePtree(boost::property_tree::ptree &tree, const bool isRoot) const
{
  /*
  if (isRoot)
  {
    tree = tree.back().second;
  }
  */

  for (auto nodeIt = this->begin(); nodeIt != this->end(); ++nodeIt)
  {
    nodeIt->writePtree(tree);
  }
}

void BasicTree::reset()
{
  for (auto nodeIt = this->begin(); nodeIt != this->end(); ++nodeIt)
  {
    nodeIt->reset();
  }
}

const char* BasicTree::getKeyNodeName() const
{
  auto keyNodeIt = this->begin();
  return keyNodeIt->name;
}

} /* namespace TreeBinding */








namespace XML
{

/*!
 *  \copydoc TreeBinding::Integer
 */
typedef TreeBinding::Integer Integer;

/*!
 *  \copydoc TreeBinding::NodesNum
 */
typedef TreeBinding::NodesNum ItemNum;

/*!
 * \brief Concatenate 3 string literals
 * \param[in] 1st literal
 * \param[in] 2nd literal
 * \param[in] 3rd literal
 */
// may be use x##y##z for visual studio
#define XML_DETAILS_TOKEN_PASTE(x, y, z) x y z 

/*!
 * \brief Concatenate 3 string literals
 * \note  Used this wrapper over XML_DETAILS_TOKEN_PASTE, because it's imposible without it
 * \param[in] 1st literal
 * \param[in] 2nd literal
 * \param[in] 3rd literal
 */
#define XML_DETAILS_CONCAT(x,y,z) XML_DETAILS_TOKEN_PASTE(x,y,z)

/*!
 * \brief XML default path delimeter (string representation)
 */
#define XML_DETAILS_PATH_DELIMETER "/"

/*!
 * \brief XML default path delimeter (char representation)
 */
#define XML_PATH_DELIMETER (*(XML_DETAILS_PATH_DELIMETER))

/*!
 * \brief   XML attribute declaration
 * \warning Each macro call should be placed in different lines
 * \param   ... 1. Attribute name. 
 *              2. Attribute's data type 
 *              3. Attribute are optional/mandatory (mandatory(TreeBinding::NodesNum::MORE_THAN_ONE) by default(if this parameter not passed)). 
 *                 If attribute are optional, pass TreeBinding::NodesNum::NOT_SPECIFIED
 */
#define XML_ATTR(name, ...) TREE_NODE( XML_DETAILS_CONCAT("<xmlattr>", XML_DETAILS_PATH_DELIMETER, name), __VA_ARGS__)

/*!
 * \brief   XML childs declaration
 * \warning Each macro call should be placed in different lines
 * \param   ... 1. Child's data type 
 *              2. Required number of childs elements (TreeBinding::NodesNum::MORE_THAN_ONE by default(if this parameter not passed)). 
 */
#define XML_CHILD_ELEMENTS(...) TREE_NODE("", __VA_ARGS__)

/*!
 * \brief   XML element declaration
 * \warning Each macro call should be placed in different lines
 * \param   ... 1. Element name (in file)
 *              2. Data type name
 */
#define XML_ELEMENT(name, dataType) TREE_TREE(name, dataType)

} /* namespace XML */


  








namespace JSON
{

/*!
 *  \copydoc TreeBinding::Integer
 */
typedef TreeBinding::Integer Integer;

/*!
 *  \copydoc TreeBinding::NodesNum
 */
typedef TreeBinding::NodesNum ItemNum;

/*!
 * \brief   JSON childs declaration
 * \warning Each macro call should be placed in different lines
 * \param[in] ... 1. Child name. 
 * \param[in] ... 2. Child's data type
 * \param[in] ... 3. Field are optional/mandatory (mandatory by default).
 */
#define JSON_CHILD(...) TREE_NODE(__VA_ARGS__)

/*!
 * \brief   JSON array declaration
 * \warning Each macro call should be placed in different lines
 * \param   ... 1. Array name. 
 * \param   ... 2. Array's elements data type
 *              3. Required number of array elements (TreeBinding::NodesNum::MORE_THAN_ONE by default(if this parameter not passed)). 
 */
#define JSON_ARRAY(...) TREE_NODE(__VA_ARGS__)

/*!
 * \brief   JSON element declaration
 * \warning Each macro call should be placed in different lines
 * \param   dataType Elements's data type
 */
#define JSON_ELEMENT(dataType) TREE_TREE(dataType)

} /* namespace JSON */




#endif /* _TREE_BINDING_H_ */

