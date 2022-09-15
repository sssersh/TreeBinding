/*!
 *  \file Converter.h
 *  \brief
 */

#ifndef _CONVERTER_H_
#define _CONVERTER_H_

#include "creolization/Details/Core/creolizationDecl.h"

namespace creolization {

namespace Details {

/*!
 * \brief Interface for converter
 */
template<typename DataType, typename ConverterImplementation>
struct Converter {

    template<typename... Args>
    static
    typename std::enable_if<isLeaf<DataType>::value>::type
    convert(Args... args)
    {
        ConverterImplementation::convertLeaf(args...);
    }

    template<typename... Args>
    static
    typename std::enable_if<is_subtrees_set<DataType>::value>::type
    convert(Args... args)
    {
        ConverterImplementation::convertArray(args...);
    }

    template<typename... Args>
    static
    typename std::enable_if<std::is_base_of<BasicTree, DataType>::value>::type
    convert(Args... args)
    {
        ConverterImplementation::convertSingleObject(args...);
    }

};

} // namespace Details

} // namespace creolization

#endif /* _CONVERTER_H_ */
