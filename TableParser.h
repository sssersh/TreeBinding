/*! 
 *  \file TableParser.h
 *  \brief Table parser class definition for NodeData
 */

#ifndef _TREE_BINDING_TABLE_PARSER_H_
#define _TREE_BINDING_TABLE_PARSER_H_

#include <algorithm>
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
 * \note  Use indexes for range, because sort table
 */
typedef struct TableParser
{

  template<typename DataType>
  static typename std::enable_if_t<!is_subtrees_set<DataType>::value && !std::is_base_of<BasicTree, DataType>::value>
    parse(NodeData<DataType> &node,
          Table<std::wstring> const &table,
          std::function<size_t(std::string &const)> const &columnNameToIndex,
          RowsRange const &rows);

  template<typename DataType>
  static typename std::enable_if_t<is_subtrees_set<DataType>::value>
    parse(NodeData<DataType> &node,
          Table<std::wstring> &table,
          std::function<size_t(std::string &const)> const &columnNameToIndex,
          RowsRange const &rows);

  template<typename DataType>
  static typename std::enable_if_t<std::is_base_of<BasicTree, DataType>::value>
    parse(NodeData<DataType> &node,
          Table<std::wstring> &table,
          std::function<size_t(std::string &const)> const &columnNameToIndex,
          RowsRange const &rows);

} TableParser;

// parse leaf
template<typename DataType>
typename std::enable_if_t<!is_subtrees_set<DataType>::value && !std::is_base_of<BasicTree, DataType>::value>
TableParser::parse(NodeData<DataType> &node,
                   Table<std::wstring> const &table,
                   std::function<size_t(std::string &const)> const &columnNameToIndex,
                   RowsRange const &rows)
{
  auto columnIndex = columnNameToIndex(std::string(node.name));
  auto rowIndex = rows.first; // all rows for this column must have same value, use first

  using convert_type = std::codecvt_utf8<wchar_t>;
  std::wstring_convert<convert_type, wchar_t> converter;

  const std::string str = converter.to_bytes(table[rowIndex][columnIndex]);
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
                   Table<std::wstring> &table,
                   std::function<size_t(std::string &const)> const &columnNameToIndex,
                   RowsRange const &rows)
{
  auto subtreesSet = (DataType*)(node.getValue()); // DataType = SubtreesSet<>

  typedef DataType::value_type::element_type SubtreeElementType;

  auto keyFieldName   = std::string(SubtreeElementType().getKeyNodeName());
  auto keyColumnIndex = columnNameToIndex(keyFieldName);

  using convert_type = std::codecvt_utf8<wchar_t>;
  std::wstring_convert<convert_type, wchar_t> converter;

  auto _begin = table.begin() + rows.first;
  auto _end   = table.begin() + rows.second + 1;
  std::sort(_begin, _end, [&](Row<std::wstring> const &row1, Row<std::wstring> const &row2)
  {
    return row1[keyColumnIndex] < row2[keyColumnIndex];
  });

  std::map<std::string, RowsRange> uniqKeys;

  auto rangeBegin = table.begin() + rows.first;;
  auto rangeEnd = rangeBegin + 1;
  while (true)
  {
    if (rangeEnd == _end || rangeBegin->at(keyColumnIndex) != rangeEnd->at(keyColumnIndex))
    {
      uniqKeys.insert(std::pair<std::string, RowsRange>(
        converter.to_bytes(rangeBegin->at(keyColumnIndex)),
        RowsRange(std::distance(_begin, rangeBegin),
                  std::distance(_begin, rangeEnd) - 1
        )
        ));
      if (rangeEnd == _end) break;
      else rangeBegin = rangeEnd++;
    }
    else ++rangeEnd;
  }

  for (auto &i : uniqKeys)
  {
    subtreesSet->emplace_back(new SubtreeElementType());
    auto& subtreeElement = subtreesSet->back();
    subtreeElement->parseTable(table, columnNameToIndex, i.second);
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
                   Table<std::wstring> &table,
                   std::function<size_t(std::string &const)> const &columnNameToIndex,
                   RowsRange const &rows)
{
  std::cout << "Here";
}

} /* namespace Details */

} /* namespace TreeBinding */

#endif /* _TREE_BINDING_TABLE_PARSER_H_ */
