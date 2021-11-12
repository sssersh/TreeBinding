/*!
 *  \file  Node.h
 *  \brief Tree node class and macroses definitions
 */

#ifndef _NODE_H_
#define _NODE_H_

//#include <memory>
//#include <string>
//#include <cinttypes>
//#include <type_traits>
#include "TreeBinding/Details/TreeBindingDecl.h"
#include "TreeBinding/Details/NodeData.h"
#include "TreeBinding/Details/StringContainer.h"
#include "TreeBinding/Details/MacroUtils.h"

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
struct Node final : public NodeData< typename std::conditional< std::is_base_of<BasicTree, DataType>::value && RequiredNum != 1,
        SubtreesSet< DataType >,
        DataType
>::type
>
{
    using DeducedDataType = typename
    std::conditional< std::is_base_of<BasicTree, DataType>::value && RequiredNum != 1,
            SubtreesSet< DataType >,
            DataType
    >::type;

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
#define TREE_BINDING_DETAILS_NODE_3(paramName, dataType, num)   \
    TREE_BINDING_DETAILS_STRING_CONTAINER(paramName, TREE_BINDING_DEFAULT_UNIQUE_SUFFIX); \
    TreeBinding::Details::Node < TREE_BINDING_DETAILS_STRING_CONTAINER_NAME(TREE_BINDING_DEFAULT_UNIQUE_SUFFIX), dataType, num >

/*!
 *  \brief     Declaration of reflection field (mandatory)
 *  \details   Declare and pass to field specialization wrapper class with uniq name, which contain name of field
 *  \warning   Each macro call should be placed in different lines
 *  \param[in] paramName Name of field
 *  \param[in] dataType Underlied type of field
 */
#define TREE_BINDING_DETAILS_NODE_2(paramName, dataType) \
    TREE_BINDING_DETAILS_NODE_3(paramName, dataType, TreeBinding::NodesNum::MORE_THAN_0)


#define TREE_BINDING_DETAILS_NODE(...)     \
    TREE_BINDING_DETAILS_OVERLOAD_MACRO(   \
        "Not contain overload with 1 arg", \
        TREE_BINDING_DETAILS_NODE_2,       \
        TREE_BINDING_DETAILS_NODE_3,       \
        __VA_ARGS__ )

} /* namespace Details */

} /* namespace TreeBinding */

#endif /* _NODE_H_ */
