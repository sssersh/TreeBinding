/*! 
 *  \file TreeBinding.cpp
 *  \brief Implementation of tree and classes/structures binding
 */

#include "TreeBinding/TreeBinding.h"

namespace TreeBinding
{

namespace Details
{

/*!
 * \brief     BasicNodeData constructor
 * \param[in] name        Name of node
 * \param[in] requiredNum Required numbers of node
 * \param[in] isLeaf      Node is leaf (not subtree container)
 */
BasicNodeData::BasicNodeData(const char* const name, NodesNum::ValueType const requiredNum, bool const isLeaf) :
  name(name),
  requiredNum(requiredNum),
  validity(false),
  isLeaf(isLeaf)
{}

/*!
 * \brief     BasicNodeData compare operator
 * \note      Compare nodes only if both are valid.
 *            Comparison function are declared in derived class.
 * \param[in] rhs Right hand side value
 * \retval    true Both are valid and are equals
 * \retval    false One/both are invalid or are not equals
 */
bool BasicNodeData::operator== (BasicNodeData const &rhs) const
{
  if (this->validity && rhs.validity)
  {
    return this->compare(rhs);
  }
  else return true;
}

/*!
 * \brief     BasicNodeData assignment operator
 * \note      Assignment function are declared in derived class.
 * \param[in] rhs Right hand side value
 */
void BasicNodeData::operator= (BasicNodeData const &rhs)
{
  this->copy(rhs);
}

} /* namespace Details */

/*!
 * \brief  Show that number of nodes is certain or not
 * \retval true Number of nodes is certain
 * \retval false Number of nodes is float
 */
bool NodesNum::isCertain() const
{
  return value >= 0;
}

/*!
 * \brief  Get number of nodes in string representation
 * \return Number of nodes in string representation
 */
std::string NodesNum::toString() const
{
  switch (value)
  {
  case NOT_SPECIFIED:
    return "not specified";
  case MORE_THAN_0:
    return "more than 0";
  default:
    return std::to_string(value);
  }
}

/*!
 * \brief     WrongChildsNumException constructor
 * \param[in] nodeName Node name
 * \param[in] requiredNum Required number of childs nodes
 * \param[in] actuallyNum Actual number of childs nodes
 */
WrongChildsNumException::WrongChildsNumException
  (std::string         const &nodeName , 
   NodesNum            const  requiredNum, 
   NodesNum::ValueType const  actuallyNum) :
   std::runtime_error("Invalid number of childs in node " + nodeName + ". Required: " + requiredNum.toString() +
                     ", present: " + std::to_string(actuallyNum))
{}

BasicTree::BasicTree(const char* const name) :
  name(name)
{}

/*!
 * \brief     Get node by index
 * \param[in] index Index of node
 * \return    Reference to node
 */
Details::BasicNodeData& BasicTree::operator[](size_t const index) const
{
  return *(this->begin() + index);
}

/*!
 * \brief Copy only leafs nodes
 * \param[in] rhs Right hand side value
 */
void BasicTree::copyLeafs(BasicTree const &rhs)
{
  /*
  for (auto &node : *this)
  {
    if (node.isLeaf) node = rhs.getSameNode(node);
  }
  */
  for (size_t i = 0; i < this->nodesNum; ++i)
  {
    if (rhs[i].isLeaf) (*this)[i] = rhs[i];
  }
}

/*!
 * \brief  Check that tree is valid
 * \note   Tree is valid if all nodes are valid
 * \retval true All nodes are valid
 * \retval false Otherwise
 */
bool BasicTree::isValid() const
{
  return std::all_of(this->begin(), this->end(), [](const Details::BasicNodeData &node) 
  { 
    return node.validity; 
  });
}

/*!
 * \brief  Check that tree contain valid nodes
 * \retval true Tree contain valid nodes
 * \retval false Otherwise
 */
bool BasicTree::containValidNodes() const
{
  return std::any_of(this->begin(), this->end(), [](const Details::BasicNodeData &node)
  {
    return node.validity;
  });
}

// deprecated
Details::BasicNodeData& BasicTree::getSameNode(const Details::BasicNodeData &rhs) const
{
  for (auto &node : *this)
  {
    if(typeid(node) == typeid(rhs)) return node;
  }
  throw(std::runtime_error("Cannot find same field"));
}


BasicTree::NodeIterator& BasicTree::NodeIterator::operator+(int const index)
{
  ptr = (Details::BasicNodeData*)((uint8_t*)ptr + Details::NodeDataSize * index);
  return *this;
}

BasicTree::NodeIterator& BasicTree::NodeIterator::operator++()
{
  return *this + 1;
}

bool BasicTree::NodeIterator::operator== (const BasicTree::NodeIterator& rhs) const
{
  return this->ptr == rhs.ptr;
}

//deprecated
bool BasicTree::NodeIterator::operator!= (const BasicTree::NodeIterator& rhs) const
{
  return !(*this == rhs);
}

Details::BasicNodeData& BasicTree::NodeIterator::operator*() const
{
  return *this->ptr;
}

Details::BasicNodeData* BasicTree::NodeIterator::operator->() const
{
  return this->ptr;
}

/*! 
 *  \brief   Get iterator on first node
 *  \details Calculate pointer as offset from this equals to size of BasicTree
 *  \return  Iterator on first node
 */
BasicTree::NodeIterator BasicTree::begin() const
{
  BasicTree::NodeIterator ret{};
  ret.ptr = (Details::BasicNodeData*)((uint8_t*)this + sizeof(*this));
  return ret;
}

/*! \brief   Get iterator on end node
 *  \warning Iterator contain pointer after last node. Only for equal, not unrefernce.
 *  \return  Iterator on end node
 */
BasicTree::NodeIterator BasicTree::end() const
{
  BasicTree::NodeIterator ret{}, beg;
  beg = begin();
  ret.ptr = (Details::BasicNodeData*)((uint8_t*)beg.ptr + nodesNum * Details::NodeDataSize);
  return ret;
}

/*! 
 *  \brief   Equal operator
 *  \note    Equal operator for Node compare values, only when both are valid. Otherwise, return true.
 *  \retval  true  Valid fields of derived are same
 *  \retval  false Valid fields of derived are different
 */
bool BasicTree::operator== (BasicTree const &rhs) const
{
  bool result = true;
  for (size_t i = 0; i < this->nodesNum; ++i)
  {
    result = result && ((*this)[i] == rhs[i]);
  }
  return result;
}

BasicTree& BasicTree::operator= (BasicTree const &rhs)
{
  for (size_t i = 0; i < this->nodesNum; ++i)
  {
    (*this)[i] = rhs[i];
  }
  return *this;
}

/*! \brief   Get validity
 *  \details Calculate validity as logical multiply of all fields of derived
 *  \warning Validity for childs not calculated
 *  \retval  true  All fields of derived Tree are valid
 *  \retval  false Not all fields of derived Tree are valid
 */
bool BasicTree::isLeafsValid() const
{
  return std::all_of(this->begin(), this->end(), [](const Details::BasicNodeData &node)
  {
    return node.isLeaf ? node.validity : true;
  });
}

bool BasicTree::isMandatoryLeafsValid() const
{
  return std::all_of(this->begin(), this->end(), [](const Details::BasicNodeData &node)
  {
    return (node.isLeaf && node.requiredNum != NodesNum::NOT_SPECIFIED ) ? node.validity : true;
  });
}

/*! \brief   Parse Tree from text representation (XML, JSON, etc.)
 *  \note    It's necessary to define ChildIterator functions and parseNode() function for type T.
 *  \param[in] tree Boost tree which represent current node and it's childs
 *  \param[in] isRoot Tree node is root node
 */
void BasicTree::parsePtree(boost::property_tree::ptree &tree, const bool isRoot)
{
  if (isRoot)
  {
    tree = tree.back().second;
  }

  /* Parse nodes */
  for (auto nodeIt = this->begin(); nodeIt != this->end(); ++nodeIt)
  {
    nodeIt->parsePtree(tree);
  }
}

void BasicTree::parseTable(std::vector<std::vector<std::wstring>> &table,
  std::function<boost::optional<size_t>(const std::string&)> const &nameToIndex,
  std::pair<size_t, size_t> const &rows)
{
  for (auto nodeIt = this->begin(); nodeIt != this->end(); ++nodeIt)
  {
    nodeIt->parseTable(table, nameToIndex, rows);
  }
}

void BasicTree::writePtree(boost::property_tree::ptree &tree, const bool isRoot) const
{
  /*
  if (isRoot)
  {
    tree = tree.back().second;
  }
  */

  for (auto nodeIt = this->begin(); nodeIt != this->end(); ++nodeIt)
  {
    nodeIt->writePtree(tree);
  }
}

void BasicTree::reset()
{
  for (auto nodeIt = this->begin(); nodeIt != this->end(); ++nodeIt)
  {
    nodeIt->reset();
  }
}

const char* BasicTree::getKeyNodeName() const
{
  auto keyNodeIt = this->begin();
  return keyNodeIt->name;
}

} /* namespace TreeBinding */
