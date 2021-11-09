/*!
 *  \file  BasicTree.h
 *  \brief BasicTree class used for iteration in Tree
 */

#ifndef _BASICTREE_H_
#define _BASICTREE_H_

namespace TreeBinding
{

/*!
 * \brief Basic tree
 */
class BasicTree
{

public:

    /*!
     *  \brief   Get iterator on first node
     *  \details Calculate pointer as offset from this equals to size of BasicTree
     *  \return  Iterator on first node
     */
    NodeIterator begin() const
    {
        NodeIterator ret{};
        ret.ptr = (Details::BasicNodeData*)((uint8_t*)this + sizeof(*this));
        return ret;
    }

    /*! \brief   Get iterator on end node
     *  \warning Iterator contain pointer after last node. Only for equal, not unrefernce.
     *  \return  Iterator on end node
     */
    NodeIterator end() const
    {
        NodeIterator ret{}, beg;
        beg = begin();
        ret.ptr = (Details::BasicNodeData*)((uint8_t*)beg.ptr + nodesNum * Details::NodeDataSize);
        return ret;
    }

    /*! \brief   Parse Tree from text representation (XML, JSON, etc.)
     *  \note    It's necessary to define ChildIterator functions and parseNode() function for type T.
     *  \param[in] tree Boost tree which represent current node and it's childs
     *  \param[in] isRoot Tree node is root node
     */
    // true for XML, false for JSON
    void parsePtree(boost::property_tree::ptree &tree, const bool isRoot = true)
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

    void parseTable(std::vector<std::vector<std::wstring>> &table,
                               std::function<boost::optional<size_t>(const std::string&)> const &nameToIndex,
                               std::pair<size_t, size_t> const &rows)
    {
        for (auto nodeIt = this->begin(); nodeIt != this->end(); ++nodeIt)
        {
            nodeIt->parseTable(table, nameToIndex, rows);
        }
    }

    // true for XML, false for JSON
    void writePtree(boost::property_tree::ptree &tree, const bool isRoot = true) const
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

    /*!
     *  \brief   Equal operator
     *  \note    Equal operator for Node compare values, only when both are valid. Otherwise, return true.
     *  \retval  true  Valid fields of derived are same
     *  \retval  false Valid fields of derived are different
     */
    bool operator== (BasicTree const &rhs) const
    {
        bool result = true;
        for (size_t i = 0; i < this->nodesNum; ++i)
        {
            result = result && ((*this)[i] == rhs[i]);
        }
        return result;
    }

    virtual BasicTree& operator= (BasicTree const &rhs)
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
    bool isLeafsValid() const
    {
        return std::all_of(this->begin(), this->end(), [](const Details::BasicNodeData &node)
        {
            return node.isLeaf ? node.validity : true;
        });
    }

    bool isMandatoryLeafsValid() const
    {
        return std::all_of(this->begin(), this->end(), [](const Details::BasicNodeData &node)
        {
            return (node.isLeaf && node.requiredNum != NodesNum::NOT_SPECIFIED ) ? node.validity : true;
        });
    }

    void reset()
    {
        for (auto nodeIt = this->begin(); nodeIt != this->end(); ++nodeIt)
        {
            nodeIt->reset();
        }
    }

    const char* getKeyNodeName() const
    {
        auto keyNodeIt = this->begin();
        return keyNodeIt->name;
    }

    /*!
     * \brief Copy only leafs nodes
     * \param[in] rhs Right hand side value
     */
    void copyLeafs(BasicTree const &rhs)
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
    bool isValid() const
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
    bool containValidNodes() const
    {
        return std::any_of(this->begin(), this->end(), [](const Details::BasicNodeData &node)
        {
            return node.validity;
        });
    }


//  template<typename, typename> friend class Tree;

protected:

    BasicTree(const char* const name) :
        name(name)
    {}

    /*!
     * \brief     Get node by index
     * \param[in] index Index of node
     * \return    Reference to node
     */
    Details::BasicNodeData& operator[](size_t const index) const
    {
        return *(this->begin() + index);
    }

    // deprecated
    Details::BasicNodeData& getSameNode(const Details::BasicNodeData &rhs) const
    {
        for (auto &node : *this)
        {
            if(typeid(node) == typeid(rhs)) return node;
        }
        throw(std::runtime_error("Cannot find same field"));
    }

    size_t            nodesNum; /*!< Number of fields in current tree */
    const char* const name;     /*!< Name of tree                     */

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);

private:
    BasicTree() = delete;
};

/*!
 * \brief serialize BasicTree by boost serializer
 * \param[out] ar      Serialized archive
 * \param[in]  version Data structure version
 */
template<class Archive>
void BasicTree::serialize(Archive & ar, const unsigned int version)
{
    for (auto nodeIt = this->begin(); nodeIt != this->end(); ++nodeIt)
    {
        ar & (*nodeIt);
    }
}

} // namespace TreeBinding

#endif /* _BASICTREE_H_ */
