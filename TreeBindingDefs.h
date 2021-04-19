/*! 
 *  \file  TreeBindingDefs.h
 *  \brief Definitions of functions for boost tree and classes/structures binding
 */

#ifndef _TREE_BINDING_DEFINITIONS_H_
#define _TREE_BINDING_DEFINITIONS_H_

#include <string>

namespace TreeBinding
{

namespace Details
{

/*! 
 *  \brief  NodeData constructor
 *  \tparam DataType NodeData data type
 */
template<typename DataType>
NodeData<DataType>::NodeData(const char* const _name, NodesNum::ValueType const _requiredNum) :
  BasicNodeData(_name, _requiredNum)
{
  value = new DataType(); // TODO: move to initialization list
};

/*! \brief     NodeData constructor
 *  \details   Used virtual function for copy, specialized for this template arguments
 *  \tparam    DataType  NodeData data type
 *  \param[in] rhs Right hand side
 */
template<typename DataType>
NodeData<DataType>::NodeData(NodeData const &rhs) :
  NodeData(rhs.name, rhs.requiredNum)
{
  this->copy(rhs);
};

/*! 
 *  \brief     NodeData assignment operator
 *  \details   Used virtual function for copy, specialized for this template arguments
 *  \tparam    DataType  NodeData data type
 *  \param[in] rhs Right hand side
 */
template<typename DataType>
NodeData<DataType>& const NodeData<DataType>::operator= (NodeData const &rhs)
{
  this->copy(rhs);
  return *this;
}

/*! 
 *  \brief  NodeData desstructor
 *  \tparam DataType NodeData data type
 */
template<typename DataType>
NodeData<DataType>::~NodeData()
{
  delete value;
}

/*! 
 *  \brief     NodeData assignment operator
 *  \tparam    DataType NodeData data type
 *  \param[in] _value Right side value
 *  \return    Assigned value
 */
template<typename DataType>
DataType& NodeData<DataType>::operator= (DataType const _value)
{
  *value = _value;
  validity = true;
  return *value;
} 

/*! 
 *  \brief  NodeData cast to underlying type operator
 *  \tparam DataType NodeData data type
 *  \return Value of field
 */
template<typename DataType>
NodeData<DataType>::operator DataType() const
{
  return *value;
}

/*! 
 *  \brief  Get pointer to value of field
 *  \tparam DataType NodeData data type
 *  \return Pointer to value of field
 */
template<typename DataType>
void* NodeData<DataType>::getValue() const
{
  return (void*)value;
}

/*! 
 *  \brief  Reset validity of value
 *  \tparam DataType NodeData data type
 */
template<typename DataType>
void NodeData<DataType>::reset()
{
  validity = false;
}

/*! 
 *  \brief     Copy field with value and validity
 *  \tparam    DataType NodeData data type
 *  \param[in] rhs Right side value
 */
template<typename DataType>
void NodeData<DataType>::copy(BasicNodeData const &rhs)
{
  *this->value = *static_cast<DataType*>(rhs.getValue());
  validity = rhs.validity;
}

/*!
 *  \brief     Compare values of fields (validity ignored)
 *  \details   Used in compare operator in NodeData
 *  \tparam    DataType    NodeData data type
 *  \param[in] rhs   Right hand side
 *  \retval    true  Values of fields are same
 *  \retval    false Value of fields are different
 */
template<typename DataType>
bool NodeData<DataType>::compare(BasicNodeData const &rhs) const
{
  return *this->value == *static_cast<DataType*>(rhs.getValue());
}


template<typename DataType>
template<typename T = DataType::iterator>
T NodeData<DataType>::begin() const
{
  auto subtreesSet = (DataType*)this->getValue();
  return subtreesSet->begin();
}

template<typename DataType>
template<typename T = DataType::iterator>
T NodeData<DataType>::end() const
{
  auto subtreesSet = (DataType*)this->getValue();
  return subtreesSet->end();
}

template<typename DataType>
template<typename T = DataType::value_type::element_type>
T* NodeData<DataType>::operator[](std::string const &key)
{
  auto subtreesSet = (DataType*)this->getValue();
  return std::find_if(subtreesSet->begin(), subtreesSet->end(), [&](std::shared_ptr<T> const it) 
  {  
    auto keyField = it->begin();
    return *((std::string*)(*keyField)->getValue()) == key;
  })->get();
}

template<typename T>
void NodeData<T>::parsePtree(boost::property_tree::ptree &tree, const char pathDelimeter = Details::DEFAULT_DELIMETER)
{
  parsePtreeImpl<T>(tree, pathDelimeter);
}

// parse leaf
template<typename DataType>
template<typename T = DataType>
typename std::enable_if_t<!is_subtrees_set<T>::value>
NodeData<DataType>::parsePtreeImpl(boost::property_tree::ptree &tree, const char pathDelimeter)
{
  try
  {
    // Not used direct get<T>, because it's necessary for forward instance of boost translator_between
    std::string str = tree.get<std::string>(path(this->name, pathDelimeter));
    Translator::fromString(str, value);
  }
  catch (boost::exception const &e) // not found such node in ptree
  {
    if (requiredNum.isCertain() || (NodesNum::MORE_THAN_0 == requiredNum))
    {
      throw(WrongChildsNumException(typeid(DataType).name(), requiredNum, 0));
    }
  }
  catch (std::exception const &e) // can't convert from string to taget type
  {
    throw(std::out_of_range("Tree node " + std::string(this->name) + " contain wrong value: \"" + "\"\n"));
  }
  validity = true;
};

// parse subtree array
// pathDelimeter not used
template<typename DataType>
template<typename T = DataType>
typename std::enable_if_t<is_subtrees_set<T>::value>
NodeData<DataType>::parsePtreeImpl(boost::property_tree::ptree &tree, const char pathDelimeter)
{
  auto subtreesSet = (DataType*)this->getValue(); // DataType = SubtreesSet<>

  if (!std::strcmp("", this->name)) // XML: array of same elements not stored in separate subtree
  {
    typedef DataType::value_type::element_type SubtreeElementType;
    static const int XML_ATTR_SUBTREE_NUM = 1;

    auto elementName = SubtreeElementType::NameContainer_::getName();

    // XML contain 1 pseudo subtree - xmlattr, not reserve for it
    subtreesSet->reserve(tree.size() - XML_ATTR_SUBTREE_NUM);

    // find if and error, if not found 
    for (auto& j : tree)
    {
      if (!std::strcmp(j.first.c_str(), elementName))
      {
        subtreesSet->emplace_back(new SubtreeElementType()); 
        auto subtreeElement = subtreesSet->back();
        subtreeElement->parsePtree(j.second, false);
      }
    }
  }
  else // for JSON name of array stored in Subtree
  {
    auto subtree = tree.get_child(this->name);
    for (auto &i : subtree)
    {
    }
  }

  /* Check num */
  if ((requiredNum.isCertain()            && subtreesSet->size() != requiredNum) ||
    (NodesNum::MORE_THAN_0 == requiredNum && 0 == subtreesSet->size()))
  {
    throw(WrongChildsNumException(typeid(DataType).name(), requiredNum, subtreesSet->size()));
  }
  
  validity = true;
};

// for print size
template<int diff, int basicNodeSize>
struct CheckSize
{
  static_assert(diff % basicNodeSize == 0, "Tree contains incorrect nodes");
  static_assert((diff >= basicNodeSize || diff == 0), "Tree contains incorrect nodes"); // 0 - for empty tree
};

} /* namespace Details */

/*! \brief   Tree constructor
 *  \details Calculate number of values: size of Derived - size of current Tree / size of field specialization
 *  \tparam  Derived Derived class
 */
template<typename NameContainer, typename Derived>
Tree<NameContainer, Derived>::Tree() :
  BasicTree(NameContainer::getName())
{
  Details::CheckSize<sizeof(Derived) - sizeof(Tree<NameContainer, Derived>), Details::NodeDataSize> check;

  nodesNum = (sizeof(Derived) - sizeof(Tree<NameContainer, Derived>)) / Details::NodeDataSize;
}

} /* namespace TreeBinding */

#endif /* _TREE_BINDING_DEFINITIONS_H_ */
