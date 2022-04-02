/*!
 *  \file  Tree.h
 *  \brief Definition of class Tree
 */

#ifndef _TYPE_H_
#define _TYPE_H_

#include <string>
#include <type_traits>
#include "creolisation/Details/Core/Exceptions.h"

namespace creolisation
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
//  template<typename T1 = CheckSize<sizeof(T) - sizeof(Tree<NameContainer, T>), details::NodeDataSize>>
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
 *  \copydoc CREOLISATION_DETAILS_TYPE_1()
 *  \param[in] name Name of tree
 */
#define CREOLISATION_DETAILS_TYPE_2(type, name)         \
    CREOLISATION_DETAILS_STRING_CONTAINER(name, type);  \
    struct type final : public creolisation::Tree < type, CREOLISATION_DETAILS_STRING_CONTAINER_NAME(type) >


#define CREOLISATION_DETAILS_TYPE_1(type) CREOLISATION_DETAILS_TYPE_2(type, #type)

#define CREOLISATION_DETAILS_TYPE(...)      \
    CREOLISATION_DETAILS_OVERLOAD_MACRO(    \
        CREOLISATION_DETAILS_TYPE_1,        \
        CREOLISATION_DETAILS_TYPE_2,        \
        "Not contain overload with 3 args", \
        "Not contain overload with 4 args", \
        __VA_ARGS__ )

} /* namespace data_binding */

#endif /* _TYPE_H_ */
