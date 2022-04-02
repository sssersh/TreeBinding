/*!
 *  \file  BasicNodeData.h
 *  \brief BasicNodeData class used for iteration in Tree
 */

#ifndef _BASICNODEDATA_H_
#define _BASICNODEDATA_H_

#include "TreeBinding/Details/Core/NodesNum.h"

namespace TreeBinding
{

namespace Details
{

/*!
 * \brief   Default path delimeter in ptree
 * \details Use macro, not constant, to keep opportunity concatenate delimeter with another string literals.
 *          Used only for parsing XML, because for parsing XML used pseudo-tree xmlattr, and it's necessary
 *          separate "xmlattr" and attribute name. For other cases used only name, without delimeter.
 *          Use "/" because it's prohibited symbol for attribute/element name in XML.
 */
#define TREE_BINDING_DEFAULT_DELIMETER "/"

// Base class used for iteration in Tree
class BasicNodeData : public Archivable
{
public:

    /*!
     * \brief     BasicNodeData constructor
     * \param[in] name        Name of node
     * \param[in] requiredNum Required numbers of node
     * \param[in] isLeaf      Node is leaf (not subtree container)
     */
    BasicNodeData(const char* const name, NodesNum::ValueType const requiredNum, bool const isLeaf) :
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
    bool operator== (BasicNodeData const &rhs) const
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
    void operator= (BasicNodeData const &rhs)
    {
        this->copy(rhs);
    }

    virtual ~BasicNodeData() = default;

    virtual void* getValue ()                            const = 0;
    virtual void  reset    ()                                  = 0;
    virtual bool  compare  (BasicNodeData const &rhs)    const = 0;
    virtual void  copy     (BasicNodeData const &rhs)          = 0;
    virtual void  parsePtree(boost::property_tree::ptree &tree, const char pathDelimeter = *TREE_BINDING_DEFAULT_DELIMETER) = 0;
    virtual void parseTable(Table<std::wstring> &table,
                            std::function<boost::optional<size_t>(const std::string&)> const &nameToIndex,
                            RowsRange const &rows) = 0;

    virtual void writePtree(boost::property_tree::ptree &tree) const = 0;

    const char* const name;        /*!< Node name                        */
    const NodesNum    requiredNum; /*!< Required number of nodes in tree */
    bool              validity;    /*!< Value of node is valid           */
    bool              isLeaf;      /*!< Value is leaf (not subtree containter and not Tree) */

    template <typename T> operator T&();

protected:
    friend class boost::serialization::access;

    friend class NodeTableParser;
    friend class PtreeWriter;
    friend class BasicTree;
    template <typename, typename> friend struct Tree;

private:
    BasicNodeData() = delete;
    BasicNodeData(BasicNodeData const &rhs) = delete;
};

// for cast unreferenced iterator to target type
template<typename T>
BasicNodeData::operator T&()
{
    return static_cast<T&>(*static_cast<NodeData<T>*>(this));
}

} // namespace details

} // namespace data_binding

#endif /* _BASICNODEDATA_H_ */
