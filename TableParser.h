/*! 
 *  \file TableParser.h
 *  \brief Table parser class definition for NodeData
 */

#ifndef _TREE_BINDING_TABLE_PARSER_H_
#define _TREE_BINDING_TABLE_PARSER_H_

#include <type_traits>
#include <map>
#include <vector>
#include <locale>
#include <codecvt>
#include "TreeBindingDecl.h"

namespace TreeBinding
{

namespace Details
{

/*!
 * \brief NodeData parser from table 
 */
typedef class TableParser
{

public:

  template<typename DataType>
  static typename std::enable_if_t<!is_subtrees_set<DataType>::value && !std::is_base_of<BasicTree, DataType>::value>
    parse(NodeData<DataType> &node,
                   std::vector<std::vector<std::wstring>> const &table,
                   std::function<size_t(std::string &const)> const &nameToIndex,
                   std::vector<size_t> const &rows);

  template<typename DataType>
  static typename std::enable_if_t<is_subtrees_set<DataType>::value>
    parse(NodeData<DataType> &node,
                   std::vector<std::vector<std::wstring>> const &table,
                   std::function<size_t(std::string &const)> const &nameToIndex,
                   std::vector<size_t> const &rows);

  template<typename DataType>
  static typename std::enable_if_t<std::is_base_of<BasicTree, DataType>::value>
    parse(NodeData<DataType> &node,
                   std::vector<std::vector<std::wstring>> const &table,
                   std::function<size_t(std::string &const)> const &nameToIndex,
                   std::vector<size_t> const &rows);

} TableParser;

// parse leaf
template<typename DataType>
typename std::enable_if_t<!is_subtrees_set<DataType>::value && !std::is_base_of<BasicTree, DataType>::value>
TableParser::parse(NodeData<DataType> &node,
                   std::vector<std::vector<std::wstring>> const &table,
                   std::function<size_t(std::string &const)> const &nameToIndex,
                   std::vector<size_t> const &rows)
{
  auto columnIndex = nameToIndex(std::string(node.name));
  auto rowIndex = rows[0]; // all rows for this column must have same value, use first

  using convert_type = std::codecvt_utf8<wchar_t>;
  std::wstring_convert<convert_type, wchar_t> converter;

  std::string str = converter.to_bytes(table[rowIndex][columnIndex]);
  try
  {
    Translator::fromString(str, node.value);
  }
  catch (std::exception const &e) // can't convert from string to taget type
  {
    throw(std::out_of_range("Tree node " + std::string(node.name) + " contain wrong value: \"" + str +
      "\", could not convert to " + std::string(typeid(DataType).name()) + "\n"));
  }
  node.validity = true;
}

// parse subtree array
template<typename DataType>
typename std::enable_if_t<is_subtrees_set<DataType>::value>
TableParser::parse(NodeData<DataType> &node,
                   std::vector<std::vector<std::wstring>> const &table,
                   std::function<size_t(std::string &const)> const &nameToIndex,
                   std::vector<size_t> const &rows)
{
  auto subtreesSet = (DataType*)(node.getValue()); // T = SubtreesSet<>

  typedef DataType::value_type::element_type SubtreeElementType;

  auto elementName = SubtreeElementType::NameContainer_::getName();
  auto columnIndex = nameToIndex(std::string(elementName));
  auto rowsNum     = table.size();

  using convert_type = std::codecvt_utf8<wchar_t>;
  std::wstring_convert<convert_type, wchar_t> converter;

  std::map<std::string, std::vector<size_t>> uniqKeys{};

  auto addToUniqKeys = [&](size_t const rowIndex)
  {
    auto& it = uniqKeys.find(converter.to_bytes(table[rowIndex][columnIndex]));
    if (it == uniqKeys.end())
    {
      uniqKeys.insert(std::pair<std::string, std::vector<size_t>>(
        converter.to_bytes(table[rowIndex][columnIndex]),
        std::vector<size_t>(1, rowIndex)
        )
        );
    }
    else
    {
      it->second.push_back(rowIndex);
    }
  };

  if (rows.empty()) // iterate over all rows
  {
    for (size_t rowIndex = 0; rowIndex < rowsNum; ++rowIndex)
    {
      addToUniqKeys(rowIndex);
    }
  }
  else // iterate only over passed rows
  {
    for (auto &rowIndex : rows)
    {
      addToUniqKeys(rowIndex);
    }
  }

  for (auto &i : uniqKeys)
  {
    subtreesSet->emplace_back(new SubtreeElementType());
    auto& subtreeElement = subtreesSet->back();
    subtreeElement->parseTable(table, nameToIndex, i.second);
  }

  /*
  // Check num 
  if ((requiredNum.isCertain() && subtreesSet->size() != requiredNum) ||
    (NodesNum::MORE_THAN_0 == requiredNum && 0 == subtreesSet->size()))
  {
    throw(WrongChildsNumException(typeid(DataType).name(), requiredNum, subtreesSet->size()));
  }
  */

  node.validity = true;
}

template<typename DataType>
typename std::enable_if_t<std::is_base_of<BasicTree, DataType>::value>
TableParser::parse(NodeData<DataType> &node,
                   std::vector<std::vector<std::wstring>> const &table,
                   std::function<size_t(std::string &const)> const &nameToIndex,
                   std::vector<size_t> const &rows)
{
  std::cout << "Here";
}

} /* namespace Details */

} /* namespace TreeBinding */

#endif /* _TREE_BINDING_TABLE_PARSER_H_ */
