/*!
 *  \file  NodeIterator.h
 *  \brief NodeIterator class definition
 */

#ifndef _NODEITERATOR_H_
#define _NODEITERATOR_H_

namespace creolisation
{

/*!
 * \brief Iterator for iterate over fields in tree
 */
struct NodeIterator : public std::iterator<std::input_iterator_tag, Details::BasicNodeData>
{
    NodeIterator() = default;
    NodeIterator(const NodeIterator&) = default;
    NodeIterator& operator=(const NodeIterator&) = default;

    NodeIterator& operator+(int const index)
    {
        ptr = (Details::BasicNodeData*)((uint8_t*)ptr + Details::NodeDataSize * index);
        return *this;
    }

    NodeIterator& operator++()
    {
        return *this + 1;
    }

    bool operator== (const NodeIterator& rhs) const
    {
        return this->ptr == rhs.ptr;
    }

//deprecated
    bool operator!= (const NodeIterator& rhs) const
    {
        return !(*this == rhs);
    }

    Details::BasicNodeData& operator*() const
    {
        return *this->ptr;
    }

    Details::BasicNodeData* operator->() const
    {
        return this->ptr;
    }

    Details::BasicNodeData* ptr;

}; // struct NodeIterator

} // namespace data_binding

#endif /* _NODEITERATOR_H_ */
