/*! 
 *  \file Xml.h
 *  \brief Macro definitions for XML implementation 
 */

#ifndef _XML_H_
#define _XML_H_

#include <boost/property_tree/xml_parser.hpp>
#include "creolization/serializable_types.h"

namespace XML
{

/*!
 *  \copydoc data_binding::NodesNum
 */
typedef creolization::NodesNum ItemNum;

/*!
 * \brief   XML attribute declaration
 * \warning Each macro call should be placed in different lines
 * \param   ... 1. Attribute name. 
 *              2. Attribute's data type 
 *              3. Attribute are optional/mandatory (mandatory(data_binding::NodesNum::MORE_THAN_ONE) by default(if this parameter not passed)).
 *                 If attribute are optional, pass data_binding::NodesNum::NOT_SPECIFIED
 */
#define XML_ATTR(name, ...) CREOLIZATION_FIELD( "<xmlattr>" CREOLIZATION_DEFAULT_DELIMETER name , __VA_ARGS__)

/*!
 * \brief   XML child declaration
 * \warning Each macro call should be placed in different lines
 * \param   ... 1. Child's data type 
 *              2. Required number of childs elements (data_binding::NodesNum::MORE_THAN_ONE by default(if this parameter not passed)).
 */
#define XML_CHILD_ELEMENTS(...) CREOLIZATION_FIELD_ARRAY("", __VA_ARGS__)

/*!
 * \brief   XML element declaration
 * \warning Each macro call should be placed in different lines
 * \param   ... 1. Data type name
 *              2. Element name (in file). "type" by default.
 */
#define XML_ELEMENT(...) CREOLIZATION_TYPE(__VA_ARGS__)

} /* namespace XML */

#endif /* _XML_H_ */
  
