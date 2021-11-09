/*! 
 *  \file  TreeBindingDefs.h
 *  \brief Definitions of functions for boost tree and classes/structures binding
 */

#ifndef _TREE_BINDING_DEFINITIONS_H_
#define _TREE_BINDING_DEFINITIONS_H_

#include <string>
#include <type_traits>
#include "TreeBinding/Details/Exceptions.h"

namespace TreeBinding
{

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

#endif /* _TREE_BINDING_DEFINITIONS_H_ */
