/*!
 *  \file  Tree.h
 *  \brief Definition of class Tree
 */

#ifndef _TREE_H_
#define _TREE_H_

#include <string>
#include <type_traits>
#include "TreeBinding/Details/Core/Exceptions.h"

namespace TreeBinding
{

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

// for print size
template<int diff, int basicNodeSize>
struct CheckSize
{
    static_assert(diff % basicNodeSize == 0, "Tree contains incorrect nodes");
    static_assert((diff >= basicNodeSize || diff == 0), "Tree contains incorrect nodes"); // 0 - for empty tree
};

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

/*!
 *  \copydoc TREE_BINDING_DETAILS_TREE_1()
 *  \param[in] name Name of tree
 */
#define TREE_BINDING_DETAILS_TREE_2(type, name)         \
    TREE_BINDING_DETAILS_STRING_CONTAINER(name, type);  \
    struct type final : public TreeBinding::Tree < type, TREE_BINDING_DETAILS_STRING_CONTAINER_NAME(type) >


#define TREE_BINDING_DETAILS_TREE_1(type) TREE_BINDING_DETAILS_TREE_2(type, #type)

#define TREE_BINDING_DETAILS_TREE(...)      \
    TREE_BINDING_DETAILS_OVERLOAD_MACRO(    \
        TREE_BINDING_DETAILS_TREE_1,        \
        TREE_BINDING_DETAILS_TREE_2,        \
        "Not contain overload with 3 args", \
        "Not contain overload with 4 args", \
        __VA_ARGS__ )

} /* namespace TreeBinding */

#endif /* _TREE_H_ */
