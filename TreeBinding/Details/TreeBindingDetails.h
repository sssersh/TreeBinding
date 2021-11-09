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
#include "TreeBinding/Details/TreeBindingDecl.h"
#include "TreeBinding/Details/NodeData.h"
#include "TreeBinding/Details/StringContainer.h"

namespace TreeBinding
{

namespace Details
{

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
#define TREE_BINDING_DETAILS_NODE_2(paramName, dataType) \
    TREE_BINDING_DETAILS_NODE_3(paramName, dataType, TreeBinding::NodesNum::MORE_THAN_0)

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

// Pass empty string to TREE_BINDING_DETAILS_NODE_GET_MACRO() to avoid error
// "ISO C++11 requires at least one argument for the "..." in a variadic macro"
#define TREE_BINDING_DETAILS_NODE_COMMON(...)                       \
  TREE_BINDING_DETAILS_EXPAND(                                      \
    TREE_BINDING_DETAILS_NODE_GET_MACRO(__VA_ARGS__,                \
                                       TREE_BINDING_DETAILS_NODE_3, \
                                       TREE_BINDING_DETAILS_NODE_2, \
                                       ""                           \
                                      )(__VA_ARGS__)                \
                             )



/*! 
 *  \copydoc TREE_BINDING_DETAILS_TREE_1()
 *  \param[in] name Name of tree
 */
#define TREE_BINDING_DETAILS_TREE_2(type, name) \
  TREE_BINDING_DETAILS_STRING_CONTAINER(name);  \
  struct type final : public TreeBinding::Tree < type, TREE_BINDING_DETAILS_STRING_CONTAINER_NAME >


#define TREE_BINDING_DETAILS_TREE_1(type) TREE_BINDING_DETAILS_TREE_2(type, #type)

#define TREE_BINDING_DETAILS_TREE_GET_MACRO(_1, _2, TARGET_MACRO, ...) TARGET_MACRO

// Pass empty string to TREE_BINDING_DETAILS_TREE_GET_MACRO() to avoid error
// "ISO C++11 requires at least one argument for the "..." in a variadic macro"
#define TREE_BINDING_DETAILS_TREE_COMMON(...)                       \
  TREE_BINDING_DETAILS_EXPAND(                                      \
    TREE_BINDING_DETAILS_TREE_GET_MACRO(__VA_ARGS__,                \
                                       TREE_BINDING_DETAILS_TREE_2, \
                                       TREE_BINDING_DETAILS_TREE_1, \
                                       ""                           \
                                      )(__VA_ARGS__)                \
                             )

} /* namespace Details */

} /* namespace TreeBinding */

#endif /* _TREE_BINDING_DETAILS_H_ */
