/*!
 *  \file  NodeData.h
 *  \brief Node data class definition
 */

#ifndef _NODEDATA_H_
#define _NODEDATA_H_

#include "creolisation/Details/Core/BasicNodeData.h"
#include "creolisation/Details/PtreeWriter.h"
#include "creolisation/Details/TableParser.h"
#include "creolisation/Details/Core/Exceptions.h"

namespace creolisation
{

namespace Details
{

// Store pointer to value, not value, to fix size of structure
template<typename DataType>
class NodeData : public BasicNodeData
{
public:
    DataType* value; /*!< Value of parameter */

    NodeData() = delete;
    NodeData(const char* const _name, NodesNum::ValueType const _requiredNum);
    NodeData(NodeData const &rhs);
    virtual ~NodeData();
    virtual const DataType& operator= (DataType const &value);
    virtual const DataType& operator= (DataType const &&value);
//  virtual operator DataType() const;
    virtual operator const DataType&() const;
    virtual bool operator==(DataType const &rhs);

    virtual void  reset   ()                                  override final;
    virtual void  copy    (BasicNodeData const &rhs)          override final;

    virtual void  parsePtree(boost::property_tree::ptree &tree,
                             const char pathDelimeter = *CREOLISATION_DEFAULT_DELIMETER) override final;
    virtual void parseTable(Table<std::wstring> &table,
                            std::function<boost::optional<size_t>(const std::string&)> const &nameToIndex,
                            RowsRange const &rows) override final;

    virtual void writePtree(boost::property_tree::ptree &tree) const override final;
    virtual void serializeData(boost::archive::text_iarchive & ar, const unsigned int version) override final;
    virtual void serializeData(boost::archive::text_oarchive & ar, const unsigned int version) override final;

    // [] is accessible only when DataType is container
//  template<typename KeyType, typename T = DataType>
//  typename T::const_iterator operator[](const KeyType &key) const;

    virtual void* getValue()                            const override final;

    using ValueType = DataType;

protected:

    const NodeData& operator= (NodeData const &rhs);

    // define separate functions for implementation, because SFINAE work only for overloading
    template<typename T = DataType>
    typename std::enable_if<!is_subtrees_set<T>::value && !std::is_base_of<BasicTree, DataType>::value>::type
    parsePtreeImpl(boost::property_tree::ptree &tree, const char pathDelimeter = *CREOLISATION_DEFAULT_DELIMETER);

    template<typename T = DataType>
    typename std::enable_if<is_subtrees_set<T>::value>::type
    parsePtreeImpl(boost::property_tree::ptree &tree, const char pathDelimeter = *CREOLISATION_DEFAULT_DELIMETER);

    template<typename T = DataType>
    typename std::enable_if<std::is_base_of<BasicTree, T>::value>::type
    parsePtreeImpl(boost::property_tree::ptree &tree, const char pathDelimeter = *CREOLISATION_DEFAULT_DELIMETER);

    template<typename T = DataType>
    typename std::enable_if<!creolisation::Details::is_subtrees_set<T>::value>::type
    resetImpl();

    template<typename T = DataType>
    typename std::enable_if<creolisation::Details::is_subtrees_set<T>::value>::type
    resetImpl();

    virtual bool compare (BasicNodeData const &rhs) const override;

    friend class NodeTableParser;
    friend class PtreeWriter;

    typedef boost::property_tree::ptree::path_type path;
}; /* class NodeData */

// defined here for use in Tree constructor
// declared not in class for get opportunity of declaration in header
static const size_t NodeDataSize = sizeof(BasicNodeData) + sizeof(void*);

/*!
 *  \brief  NodeData constructor
 *  \tparam DataType NodeData data type
 */
template<typename DataType>
NodeData<DataType>::NodeData(const char* const _name, NodesNum::ValueType const _requiredNum) :
        BasicNodeData(_name, _requiredNum, !is_subtrees_set<DataType>::value && !std::is_base_of<BasicTree, DataType>::value)
{
    value = new DataType(); // TODO1: move to initialization list and use allocators
}

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
}

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
typename std::enable_if<!creolisation::Details::is_subtrees_set<T>::value>::type
NodeData<DataType>::resetImpl()
{
    validity = false;
}

template<typename DataType>
template<typename T>
typename std::enable_if<creolisation::Details::is_subtrees_set<T>::value>::type
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

/*
template<typename DataType>
template<typename KeyType, typename T>
typename T::const_iterator NodeData<DataType>::operator[](const KeyType &key) const
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
*/

template<typename T>
void NodeData<T>::parsePtree(boost::property_tree::ptree &tree, const char pathDelimeter)
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
typename std::enable_if<!is_subtrees_set<T>::value && !std::is_base_of<BasicTree, DataType>::value>::type
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
            *value = Translator::fromString<DataType>(str);
        }
        catch (std::exception const &) // can't convert from string to taget type
        {
            throw(std::out_of_range("Tree node " + std::string(this->name) + " contain wrong value: \"" + str +
                                    "\", could not convert to " + std::string(typeid(DataType).name()) + "\n"));
        }
        validity = true;
    }
}

// parse subtree array
// pathDelimeter not used
template<typename DataType>
template<typename T>
typename std::enable_if<is_subtrees_set<T>::value>::type
NodeData<DataType>::parsePtreeImpl(boost::property_tree::ptree &tree, const char pathDelimeter)
{
    auto subtreesSet = (DataType*)this->getValue(); // DataType = SubtreesSet<>

    typedef typename DataType::value_type SubtreeElementType;

    if (!std::strcmp("", this->name)) // XML: array of same elements not stored in separate subtree
    {
        static const int XML_ATTR_SUBPTREE_NUM = 1;

        auto elementName = SubtreeElementType::NameContainer_::getName();

        // XML contain 1 pseudo subtree - xmlattr, not reserve for it
        subtreesSet->reserve(tree.size() - XML_ATTR_SUBPTREE_NUM);

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
    if ((requiredNum.isCertain()            && (decltype(requiredNum))subtreesSet->size() != requiredNum) ||
        (NodesNum::MORE_THAN_0 == requiredNum && 0 == subtreesSet->size()))
    {
        throw(WrongChildsNumException(typeid(DataType).name(), requiredNum, subtreesSet->size()));
    }

    validity = true;
}

// parse single subtree
template<typename DataType>
template<typename T>
typename std::enable_if<std::is_base_of<BasicTree, T>::value>::type
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
}

template<typename T>
void NodeData<T>::parseTable(std::vector<std::vector<std::wstring>> &table,
                             std::function<boost::optional<size_t>(const std::string&)> const &nameToIndex,
                             std::pair<size_t, size_t> const &rows)
{
    TableParser::parse(*this, table, nameToIndex, rows);
}

} // namespace details

}  // namespace data_binding

#endif /* _NODEDATA_H_ */
