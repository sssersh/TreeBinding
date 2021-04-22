/*! 
 *  \file TableParser.h
 *  \brief Table parser class definition for NodeData
 */

#ifndef _TREE_BINDING_TABLE_PARSER_H_
#define _TREE_BINDING_TABLE_PARSER_H_

#include <type_traits>
#include "TreeBindingDecl.h"

namespace TreeBinding
{

namespace Details
{

/*!
 * \brief NodeData parser from table 
 */
class TableParser
{
private:

  template <typename> friend class NodeData;

  template<typename T>
  static typename std::enable_if_t<!is_subtrees_set<T>::value && !std::is_base_of<BasicTree, T>::value>
    parseImpl(NodeData<T> &node,
                   std::vector<std::vector<std::wstring>> table,
                   std::function<size_t(std::string &const)> nameToIndex);

  template<typename T>
  static typename std::enable_if_t<is_subtrees_set<T>::value>
    parseImpl(NodeData<T> &node,
                   std::vector<std::vector<std::wstring>> table,
                   std::function<size_t(std::string &const)> nameToIndex);

  template<typename T>
  static typename std::enable_if_t<std::is_base_of<BasicTree, T>::value>
    parseImpl(NodeData<T> &node,
                   std::vector<std::vector<std::wstring>> table,
                   std::function<size_t(std::string &const)> nameToIndex);

};

template<typename T>
typename std::enable_if_t<!is_subtrees_set<T>::value && !std::is_base_of<BasicTree, T>::value>
TableParser::parseImpl(NodeData<T> &node,
                  std::vector<std::vector<std::wstring>> table,
                  std::function<size_t(std::string &const)> nameToIndex)
{ 
  std::cout << "Here";
}

template<typename T>
typename std::enable_if_t<is_subtrees_set<T>::value>
TableParser::parseImpl(NodeData<T> &node,
                  std::vector<std::vector<std::wstring>> table,
                  std::function<size_t(std::string &const)> nameToIndex)
{
  std::cout << "Here";
}

template<typename T>
typename std::enable_if_t<std::is_base_of<BasicTree, T>::value>
TableParser::parseImpl(NodeData<T> &node,
                  std::vector<std::vector<std::wstring>> table,
                  std::function<size_t(std::string &const)> nameToIndex)
{
  std::cout << "Here";
}

} /* namespace Details */

} /* namespace TreeBinding */

#endif /* _TREE_BINDING_TABLE_PARSER_H_ */
