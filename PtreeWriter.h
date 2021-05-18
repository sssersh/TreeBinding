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
 * \brief NodeData writer to ptree
 */
typedef class PtreeWriter
{
public:

  template<typename DataType>
  static typename std::enable_if_t<!is_subtrees_set<DataType>::value && !std::is_base_of<BasicTree, DataType>::value>
  write(NodeData<DataType> const &node,
        boost::property_tree::ptree &tree);

  template<typename DataType>
  static typename std::enable_if_t<is_subtrees_set<DataType>::value>
    write(NodeData<DataType> const &node,
        boost::property_tree::ptree &tree);

  template<typename DataType>
  static typename std::enable_if_t<std::is_base_of<BasicTree, DataType>::value>
  write(NodeData<DataType> const &node,
        boost::property_tree::ptree &tree);

} PtreeWriter;

template<typename DataType>
typename std::enable_if_t<!is_subtrees_set<DataType>::value && !std::is_base_of<BasicTree, DataType>::value>
PtreeWriter::write(NodeData<DataType> const &node,
                   boost::property_tree::ptree &tree)
{
  if (node.validity)
  {
    const std::string str = Translator::toString(node.value);
    tree.add(boost::property_tree::path(node.name), str);
  }
}

template<typename DataType>
typename std::enable_if_t<is_subtrees_set<DataType>::value>
PtreeWriter::write(NodeData<DataType> const &node,
                   boost::property_tree::ptree &tree)
{
  typedef DataType::value_type::element_type SubtreeElementType;

  if (node.validity)
  {
    auto subtreesSet = (DataType*)node.getValue(); // DataType = SubtreesSet<>
    auto elementName = SubtreeElementType::NameContainer_::getName();

    boost::property_tree::ptree subtree;

    for (auto &i : *subtreesSet)
    {
      i->writePtree(subtree);
    }

    tree.add_child(elementName, subtree);
  }
}

template<typename DataType>
typename std::enable_if_t<std::is_base_of<BasicTree, DataType>::value>
PtreeWriter::write(NodeData<DataType> const &node,
                   boost::property_tree::ptree &tree)
{
  std::runtime_error("PtreeWriter::write Not implement!");
}

} /* namespace Details */

} /* namespace TreeBinding */

#endif /* _TREE_BINDING_PTREE_WRITER_H_ */
