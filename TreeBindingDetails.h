/*! 
 *  \file  TreeBindingDetails.h
 *  \brief Private classes for boost tree and classes/structures binding
 */

#ifndef _TREE_BINDING_DETAILS_H_
#define _TREE_BINDING_DETAILS_H_

#include <memory>
#include <string>
#include <cinttypes>
#include <type_traits>
#include <vector> // subtree containter

namespace TreeBinding
{

/*!
 * \brief  Containter for subtree elements
 * \tparam Type of subtree elements
 */
template<typename T>
using SubtreesSet = std::vector<std::shared_ptr<T>>;

typedef struct NodesNum
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
} NodesNum;

class BasicTree;

template<typename, typename>
struct Tree;

namespace Details
{

static const char DEFAULT_DELIMETER = '/';

// Base class used for iteration in Tree
typedef class BasicNodeData
{
public:

  BasicNodeData() = delete;
  BasicNodeData(const char* const _name, NodesNum::ValueType const num);
  BasicNodeData(BasicNodeData const &rhs) = delete;
  virtual ~BasicNodeData() = default;
  void operator= (BasicNodeData const &rhs);

  virtual void* getValue ()                            const = 0;
  virtual void  reset    ()                                  = 0;
  virtual bool  compare  (BasicNodeData const &rhs)    const = 0;
  virtual void  copy     (BasicNodeData const &rhs)          = 0;
  virtual void  parsePtree(boost::property_tree::ptree &tree, const char pathDelimeter = Details::DEFAULT_DELIMETER) = 0;

  const char* const name;        /*!< Node name                        */
  const NodesNum    requiredNum; /*!< Required number of nodes in tree */
  bool              validity;    /*!< Value of node is valid           */

protected:
  bool operator== (BasicNodeData const &rhs) const;

  friend class BasicTree;
  template <typename, typename> friend struct Tree;
} BasicNodeData;

template<typename>
struct is_subtrees_set : std::false_type {};

template<typename T>
struct is_subtrees_set<SubtreesSet<T>> : std::true_type{};
 
// Store pointer to value, not value, to fix size of structure
template<typename DataType>
class NodeData : public BasicNodeData
{
public:
  DataType* value; /*!< Value of parameter */

  NodeData() = delete;
  NodeData(const char* const _name, NodesNum::ValueType const _requiredNum);
  NodeData(NodeData const &rhs);
  NodeData& const operator= (NodeData const &rhs);
  virtual ~NodeData();
  DataType& operator= (DataType const _value);
  virtual operator DataType() const;

  virtual void* getValue()                            const override final;
  virtual void  reset   ()                                  override final;
  virtual void  copy    (BasicNodeData const &rhs)          override final;
  virtual void  parsePtree(boost::property_tree::ptree &tree, 
                           const char pathDelimeter = Details::DEFAULT_DELIMETER) override final;

  // begin(), end() and [] is accessible only when DataType is container
  template<typename T = DataType::iterator>
  T begin() const;

  template<typename T = DataType::iterator>
  T end() const;

  // Use first leaf as key(string)
  template<typename T = DataType::value_type::element_type>
  T* operator[](std::string const &key);

protected:

  // define separate functions for implementation, because SFINAE work only for overloading
  template<typename T = DataType>
  typename std::enable_if_t<!is_subtrees_set<T>::value>
    parsePtreeImpl(boost::property_tree::ptree &tree, const char pathDelimeter = Details::DEFAULT_DELIMETER);

  template<typename T = DataType>
  typename std::enable_if_t<is_subtrees_set<T>::value>
    parsePtreeImpl(boost::property_tree::ptree &tree, const char pathDelimeter = Details::DEFAULT_DELIMETER);

  virtual bool compare (BasicNodeData const &rhs) const override;

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
struct Node final : public NodeData< std::conditional_t< std::is_base_of<BasicTree, DataType>::value,
                                                         SubtreesSet< DataType >,
                                                         DataType
                                                       > 
                                   >
{
  using InferetedDataType = typename
    std::conditional_t< std::is_base_of<BasicTree, DataType>::value,
                        SubtreesSet< DataType >,
                        DataType
                      >;

  Node() : NodeData<InferetedDataType>(NameContainer::getName(), RequiredNum) {};
};

static_assert(sizeof(Node<int, int, 0>) == NodeDataSize, "Fatal error: incorrect alignment in Node.");

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
 *  \copydoc BOOST_TREE_WRAPPER_FIELD_2()
 *  \param[in] num Required number of fields
 */
#define TREE_BINDING_DETAILS_NODE_3(paramName, dataType, num) \
  TREE_BINDING_DETAILS_STRING_CONTAINER(paramName);           \
  TreeBinding::Details::Node < TREE_BINDING_DETAILS_STRING_CONTAINER_NAME, dataType, num >

/*! \brief     Declaration of reflection field (mandatory)
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
#define TREE_BINDING_DETAILS_GET_MACRO(_1, _2, _3, TARGET_MACRO, ...) TARGET_MACRO

#define TREE_BINDING_DETAILS_NODE_COMMON(...)                   \
  TREE_BINDING_DETAILS_EXPAND(                                  \
    TREE_BINDING_DETAILS_GET_MACRO(__VA_ARGS__,                 \
                                   TREE_BINDING_DETAILS_NODE_3, \
                                   TREE_BINDING_DETAILS_NODE_2  \
                                  )(__VA_ARGS__)                \
                             )

} /* namespace Details */

} /* namespace TreeBinding */

#endif /* _TREE_BINDING_DETAILS_H_ */
