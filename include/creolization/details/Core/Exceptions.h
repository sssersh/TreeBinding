/*!
 *  \file Exceptions.h
 *  \brief Tree binding exceptions
 */

#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

#include "creolization/Details/Core/NodesNum.h"

namespace creolization {

/*!
 * \brief Exception for signal wrong number of childs elements in tree
 */
struct WrongChildsNumException : public std::runtime_error
{
    /*!
     * \brief     WrongChildsNumException constructor
     * \param[in] nodeName Node name
     * \param[in] requiredNum Required number of childs nodes
     * \param[in] actuallyNum Actual number of childs nodes
     */
    WrongChildsNumException(std::string const &nodeName,
                            NodesNum const requiredNum,
                            NodesNum::ValueType const actuallyNum) :
            std::runtime_error(
                    "Invalid number of childs in node " + nodeName +
                    ". Required: " + requiredNum.toString() +
                    ", present: " + std::to_string(actuallyNum))
    {}
};

} // namespace data_binding

#endif /* _EXCEPTIONS_H */
