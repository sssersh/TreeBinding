/*! 
 *  \file  TableParser.h
 *  \brief Tree bindings forward declarations
 */

#ifndef _TREE_BINDING_DECLARATIONS_H_
#define _TREE_BINDING_DECLARATIONS_H_

#include <type_traits>

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
using SubtreesSet = std::vector<std::shared_ptr<T>>;

namespace Details
{

template<typename>
class NodeData;

class TableParser;

template<typename>
struct is_subtrees_set : std::false_type {};

template<typename T>
struct is_subtrees_set<SubtreesSet<T>> : std::true_type{};

} /* namespace Details */

} /* namespace TreeBinding */

#endif /* _TREE_BINDING_DECLARATIONS_H_ */
