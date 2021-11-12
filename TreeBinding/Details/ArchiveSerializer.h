/*!
 *  \file  ArchiveSerializerDecl.h
 *  \brief Converter of NodeData to boost archive
 */

#ifndef _TREE_BINDING_ARCHIVE_SERIALIZER_H_
#define _TREE_BINDING_ARCHIVE_SERIALIZER_H_

#include <boost/serialization/vector.hpp>
#include "TreeBinding/Details/ArchiveSerializerDecl.h"

// Should be declared in global namespace
#define TREE_BINDING_STRONG_TYPEDEF_SERIALIZE_DECLARATION(type)          \
  namespace boost {                                                      \
  namespace serialization {                                              \
  template<class Archive>                                                \
  void serialize(Archive & ar, type & value, const unsigned int version) \
  {                                                                      \
    ar & value.t;                                                        \
  }                                                                      \
  } /* namespace serialization */                                        \
  } /* namespace boost        */

namespace boost {
namespace serialization {

using namespace TreeBinding;
using namespace Details;

/*
template<class Archive>
void serialize(Archive & ar, BasicTree& tree, const unsigned int version)
{
  for (auto nodeIt = tree.begin(); nodeIt != tree.end(); ++nodeIt)
  {
    ar & (*nodeIt);
  }
}

template<class Archive>
void serialize(Archive & ar, const BasicTree& tree, const unsigned int version)
{
  for (auto nodeIt = tree.begin(); nodeIt != tree.end(); ++nodeIt)
  {
    ar & (*nodeIt);
  }
}
*/

/*
template<class Archive, typename NameContainer, typename T>
typename std::enable_if_t<std::is_base_of<BasicTree, Tree<NameContainer, T>>::value>
serialize(Archive & ar, Tree<NameContainer, T>& tree, const unsigned int version)
{
  for (auto nodeIt = tree.begin(); nodeIt != tree.end(); ++nodeIt)
  {
    ar & (*nodeIt);
  }
}
*/

template<class Archive>
void serialize(Archive & ar, BasicNodeData& node, const unsigned int version)
{
  ar & node.validity;
  ar & node.isLeaf;
  node.serializeData(ar, version);
}

/*
// especially for BasicNodeData
template<class Archive, typename Node>
typename std::enable_if_t<std::is_same<BasicNodeData, Node>::value> 
serialize(Archive & ar, Node& tree, const unsigned int version)
{
  ar & validity;
  serializeData(ar, version);
}
*/

// for implementations of NodeData<>
template<class Archive, typename DataType>
typename std::enable_if<!std::is_same<BasicNodeData, NodeData<DataType>>::value && std::is_base_of<BasicNodeData, NodeData<DataType>>::value>::type
serialize(Archive & ar, NodeData<DataType>& node, const unsigned int version)
{
  ar & *(node.value);
}

} // namespace serialization
} // namespace boost

namespace TreeBinding
{

namespace Details
{

template<typename DataType>
void NodeData<DataType>::serializeData(boost::archive::text_oarchive & ar, const unsigned int version)
{
  boost::serialization::serialize(ar, *this, version);
}

template<typename DataType>
void NodeData<DataType>::serializeData(boost::archive::text_iarchive & ar, const unsigned int version)
{
  boost::serialization::serialize(ar, *this, version);
}

} /* namespace Details */

} /* namespace TreeBinding */

#endif /* _TREE_BINDING_ARCHIVE_SERIALIZER_H_ */
