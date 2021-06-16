/*! 
 *  \file  TreeBindingDefs.h
 *  \brief Definitions of functions for boost tree and classes/structures binding
 */

#ifndef _TREE_BINDING_DEFINITIONS_H_
#define _TREE_BINDING_DEFINITIONS_H_

#include <string>
#include <type_traits>

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
  BasicNodeData(_name, _requiredNum, !is_subtrees_set<DataType>::value && !std::is_base_of<BasicTree, DataType>::value)
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
const NodeData<DataType>& NodeData<DataType>::operator= (NodeData const &rhs)
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
 *  \param[in] value Right side value
 *  \return    Assigned value
 */
template<typename DataType>
const DataType& NodeData<DataType>::operator= (DataType const &value)
{
  *this->value = value;
  validity = true;
  return *this->value;
} 

template<typename DataType>
const DataType& NodeData<DataType>::operator= (DataType const &&value)
{
  *this->value = value;
  validity = true;
  return *this->value;
}

/*! 
 *  \brief  NodeData cast to underlying type operator
 *  \tparam DataType NodeData data type
 *  \return Value of field
 */
/*
template<typename DataType>
NodeData<DataType>::operator DataType() const
{
  return *value;
}
*/

template<typename DataType>
NodeData<DataType>::operator const DataType&() const
{
  return *value;
}

template<typename DataType>
bool NodeData<DataType>::operator==(DataType const &rhs)
{
  return *(this->value) == rhs;
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
/*
template<typename DataType>
void NodeData<DataType>::reset()
{
  validity = false;
}
*/

template<typename DataType>
void NodeData<DataType>::reset()
{
  resetImpl<DataType>();
}

template<typename DataType>
template<typename T>
typename std::enable_if_t<!TreeBinding::Details::is_subtrees_set<T>::value>
NodeData<DataType>::resetImpl()
{
  validity = false;
}

template<typename DataType>
template<typename T>
std::enable_if_t<TreeBinding::Details::is_subtrees_set<T>::value>
NodeData<DataType>::resetImpl()
{
  value->clear();
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
  *(this->value) = *static_cast<DataType*>(rhs.getValue());
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
template<typename KeyType, typename T>
T NodeData<DataType>::operator[](const KeyType &key) const
{
  static_assert(std::is_assignable< KeyType&, KeyType >::value, "Key type for operator [] should be assignable");

  return std::find_if(this->value->cbegin(), this->value->cend(), [&](typename DataType::const_reference element)
  {  
    auto keyField = std::find_if(element.begin(), element.end(), [&](const BasicNodeData &node)
    {
      return dynamic_cast<const NodeData<KeyType>*>(&node);
    });
    if (keyField == element.end()) throw std::runtime_error(std::string("There are not key field in ") + typeid(typename DataType::value_type).name() + "\n");
    return static_cast<NodeData<KeyType>&>(*keyField) == key;
  });
}

template<typename T>
void NodeData<T>::parsePtree(boost::property_tree::ptree &tree, const char pathDelimeter = Details::DEFAULT_DELIMETER)
{
  parsePtreeImpl<T>(tree, pathDelimeter);
}

template<typename T>
void NodeData<T>::writePtree(boost::property_tree::ptree &tree) const
{
  PtreeWriter::write(*this, tree);
}

// parse leaf
template<typename DataType>
template<typename T>
typename std::enable_if_t<!is_subtrees_set<T>::value && !std::is_base_of<BasicTree, DataType>::value>
NodeData<DataType>::parsePtreeImpl(boost::property_tree::ptree &tree, const char pathDelimeter)
{
  std::string str;
  try
  {
    // Not used direct get<T>, because it's necessary for forward instance of boost translator_between
    str = tree.get<std::string>(path(this->name, pathDelimeter));
  }
  catch (boost::exception const &) // not found such node in ptree
  {
    if (requiredNum.isCertain() || (NodesNum::MORE_THAN_0 == requiredNum))
    {
      throw(WrongChildsNumException(std::string(this->name) + " (" + typeid(DataType).name() + ")", requiredNum, 0));
    }
  }

  if (str.empty() && !this->requiredNum.isCertain())
  {
    this->validity = false;
  }
  else
  {
    try
    {
      Translator::fromString(str, value);
    }
    catch (std::exception const &) // can't convert from string to taget type
    {
      throw(std::out_of_range("Tree node " + std::string(this->name) + " contain wrong value: \"" + str +
        "\", could not convert to " + std::string(typeid(DataType).name()) + "\n"));
    }
    validity = true;
  }
};

// parse subtree array
// pathDelimeter not used
template<typename DataType>
template<typename T>
typename std::enable_if_t<is_subtrees_set<T>::value>
NodeData<DataType>::parsePtreeImpl(boost::property_tree::ptree &tree, const char pathDelimeter)
{
  auto subtreesSet = (DataType*)this->getValue(); // DataType = SubtreesSet<>

  typedef DataType::value_type SubtreeElementType;

  if (!std::strcmp("", this->name)) // XML: array of same elements not stored in separate subtree
  {
    static const int XML_ATTR_SUBTREE_NUM = 1;

    auto elementName = SubtreeElementType::NameContainer_::getName();

    // XML contain 1 pseudo subtree - xmlattr, not reserve for it
    subtreesSet->reserve(tree.size() - XML_ATTR_SUBTREE_NUM);

    // find if and error, if not found 
    for (auto& j : tree)
    {
      if (!std::strcmp(j.first.c_str(), elementName))
      {
        subtreesSet->emplace_back(); 
        auto& subtreeElement = subtreesSet->back();
        subtreeElement.parsePtree(j.second, false);
      }
    }
  }
  else // for JSON name of array stored in Subtree
  {
    try
    {
      auto &subtree = tree.get_child(this->name);
      for (auto &j : subtree)
      {
        subtreesSet->emplace_back();
        auto& subtreeElement = subtreesSet->back();
        subtreeElement.parsePtree(j.second, false);
      }
    }
    catch (const std::exception &)
    {
      if (requiredNum.isCertain()) throw;
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

// parse single subtree
template<typename DataType>
template<typename T>
typename std::enable_if_t<std::is_base_of<BasicTree, T>::value>
NodeData<DataType>::parsePtreeImpl(boost::property_tree::ptree &tree, const char pathDelimeter)
{
  auto subtree = (DataType*)this->getValue();
  size_t num = 0;

  if (!std::strcmp("", this->name)) // XML: array of same elements not stored in separate subtree
  {
    auto elementName = DataType::NameContainer_::getName();

    // find if and error, if not found 
    for (auto& j : tree)
    {
      if (!std::strcmp(j.first.c_str(), elementName))
      {
        subtree->parsePtree(j.second, false);
        num++;
      }
    }
  }
  else // for JSON name of array stored in Subtree
  {
    auto &tree_ = tree.get_child(this->name);
    subtree->parsePtree(tree_, false);
    num = 1;
  }

  /* Check num */
  if ((requiredNum.isCertain() && num != requiredNum) ||
      (NodesNum::MORE_THAN_0 == requiredNum && 0 == num))
  {
    throw(WrongChildsNumException(typeid(DataType).name(), requiredNum, num));
  }

  validity = true;
};

template<typename T>
void NodeData<T>::parseTable(std::vector<std::vector<std::wstring>> &table,
                             std::function<size_t(const std::string&)> const &nameToIndex,
                             std::pair<size_t, size_t> const &rows)
{
  TableParser::parse(*this, table, nameToIndex, rows);
}

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
template<typename Derived, typename NameContainer>
template<typename T>
Tree<Derived, NameContainer>::Tree() :
  BasicTree(NameContainer::getName())
{
  nodesNum = (sizeof(Derived) - sizeof(Tree<NameContainer, Derived>)) / Details::NodeDataSize;
}

} /* namespace TreeBinding */

#endif /* _TREE_BINDING_DEFINITIONS_H_ */
