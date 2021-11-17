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
#include "TreeBinding/Details/Core/TreeBindingDecl.h"
#include "TreeBinding/Details/Core/NodeData.h"
#include "TreeBinding/Details/Core/StringContainer.h"
#include "TreeBinding/Details/Core/MacroUtils.h"

namespace TreeBinding
{

namespace Details
{

enum class ContainerRequired
{
    YES,
    NO
};

template<ContainerRequired containerRequired, NodesNum::ValueType nodesNum>
struct containerIsRequired
{
    static const bool value = containerRequired == ContainerRequired::YES && nodesNum != 1;
};

template<ContainerRequired containerRequired, NodesNum::ValueType nodesNum, typename DataType>
struct NodeDataType
{
    typedef typename
    std::conditional<
            containerIsRequired<containerRequired, nodesNum>::value,
            SubtreesSet< DataType >,
            DataType
    >::type type;
};

/*!
 *  \brief  Tree node
 *  \tparam NameContainer String wrapper class, which contain field name
 *  \tparam DataType      NodeData data type
 *  \tparam RequiredNum   Required number of fields
 */
template< ContainerRequired containerRequired ,
          typename NameContainer,
          typename DataType     ,
          NodesNum::ValueType RequiredNum = NodesNum::MORE_THAN_0
>
struct Node final : public NodeData< typename NodeDataType<containerRequired, RequiredNum, DataType>::type >
{
    using DeducedDataType = typename NodeDataType<containerRequired, RequiredNum, DataType>::type;

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
static_assert(sizeof(Node<ContainerRequired::YES, AssertName, int, 0>) == NodeDataSize, "Fatal error: incorrect alignment in Node.");

/*!
 *  \copydoc TREE_BINDING_DETAILS_NODE_2()
 *  \param[in] num Required number of fields
 */
#define TREE_BINDING_DETAILS_NODE_4(containerRequired, paramName, dataType, num) \
    TREE_BINDING_DETAILS_STRING_CONTAINER(paramName, __LINE__);                  \
    TreeBinding::Details::Node <                                                 \
        containerRequired,                                                       \
        TREE_BINDING_DETAILS_STRING_CONTAINER_NAME(__LINE__),                    \
        dataType,                                                                \
        num                                                                      \
        >

/*!
 *  \brief     Declaration of reflection field (mandatory)
 *  \details   Declare and pass to field specialization wrapper class with uniq name, which contain name of field
 *  \warning   Each macro call should be placed in different lines
 *  \param[in] paramName Name of field
 *  \param[in] dataType Underlied type of field
 */
#define TREE_BINDING_DETAILS_NODE_3(containerRequired, paramName, dataType) \
    TREE_BINDING_DETAILS_NODE_4(containerRequired, paramName, dataType, TreeBinding::NodesNum::MORE_THAN_0)


#define TREE_BINDING_DETAILS_NODE(...)      \
    TREE_BINDING_DETAILS_OVERLOAD_MACRO(    \
        "Not contain overload with 1 arg",  \
        "Not contain overload with 2 args", \
        TREE_BINDING_DETAILS_NODE_3,        \
        TREE_BINDING_DETAILS_NODE_4,        \
        __VA_ARGS__ )

} /* namespace Details */

} /* namespace TreeBinding */

#endif /* _NODE_H_ */
