/*!
 *  \file NodesNum.h
 *  \brief Type for represent number of fields in Tree object
 */

#ifndef _NODESNUM_H_
#define _NODESNUM_H_

namespace creolization
{

/*!
 *  \brief Type for represent number of fields in Tree object
 */
struct NodesNum {
    typedef int32_t ValueType;

    static const ValueType NOT_SPECIFIED = -1; /*!< Number of nodes not specified (optional) */
    static const ValueType MORE_THAN_0   = -2; /*!< Number of nodes should be more than 0    */

    NodesNum(ValueType const value) :
            value(value) {}

    operator ValueType() const {
        return value;
    }

    /*!
     * \brief  Show that number of nodes is certain or not
     * \retval true Number of nodes is certain
     * \retval false Number of nodes is float
     */
    bool isCertain() const {
        return value >= 0;
    }

    /*!
     * \brief  Get number of nodes in string representation
     * \return Number of nodes in string representation
     */
    std::string toString() const {
        switch (value) {
            case NOT_SPECIFIED:
                return "not specified";
            case MORE_THAN_0:
                return "more than 0";
            default:
                return std::to_string(value);
        }
    }

private:
    NodesNum() = delete;

    ValueType value;
}; // struct NodesNum

} // namespace data_binding

#endif /* _NODESNUM_H_ */
