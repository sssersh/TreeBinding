/*!
 *  \file  PtreeWriter.h
 *  \brief Converter of NodeData to boost ptree
 */

#ifndef _TREE_BINDING_PTREE_WRITER_H_
#define _TREE_BINDING_PTREE_WRITER_H_

#include "TreeBindingDecl.h"

namespace TreeBinding
{

namespace Details
{

/*!
 * \brief NodeData parser from table 
 * \note  Use indexes for range, because sort table
 */
typedef struct PtreeWriter
{

  template<typename DataType>
  static typename std::enable_if_t<!is_subtrees_set<DataType>::value && !std::is_base_of<BasicTree, DataType>::value>
  write(NodeData<DataType> &node,
        boost::property_tree::ptree &tree);

  template<typename DataType>
  static typename std::enable_if_t<is_subtrees_set<DataType>::value>
  write(NodeData<DataType> &node,
        boost::property_tree::ptree &tree);

  template<typename DataType>
  static typename std::enable_if_t<std::is_base_of<BasicTree, DataType>::value>
  write(NodeData<DataType> &node,
        boost::property_tree::ptree &tree);

} PtreeWriter;

} /* namespace Details */

} /* namespace TreeBinding */

#endif /* _TREE_BINDING_PTREE_WRITER_H_ */
