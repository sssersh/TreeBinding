/*! 
 *  \file  TableParser.h
 *  \brief Tree bindings forward declarations
 */

#ifndef _TREE_BINDING_DECLARATIONS_H_
#define _TREE_BINDING_DECLARATIONS_H_

#include <type_traits>
#include <vector> // subtree containter

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

// second index single_include in range
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

} /* namespace details */

} /* namespace data_binding */

#endif /* _TREE_BINDING_DECLARATIONS_H_ */
