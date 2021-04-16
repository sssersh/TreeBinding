/*! \file Xml.h
 *  \brief Classes for implement XML
 */

#ifndef _XML_H_
#define _XML_H_

#include "TreeBinding.h"

namespace XML
{

/*!
 *  \brief Type for store integer fields of XML attributes
 */
typedef TreeBinding::Integer Integer;

#define XML_DETAILS_TOKEN_PASTE(x, y, z) x##y##z
#define XML_DETAILS_CONCAT(x,y,z) XML_DETAILS_TOKEN_PASTE(x,y,z)

#define XML_DETAILS_PATH_DELIMETER "/"
#define XML_PATH_DELIMETER (*(XML_DETAILS_PATH_DELIMETER))

/*!
 * \brief   XML attribute declaration
 * \warning Each macro call should be placed in different lines
 * \param   ... 1. Attribute name. 
 *              2. Attribute's data type 
 *              3. Attribute are optional/mandatory (mandatory(TreeBinding::NodesNum::MORE_THAN_ONE) by default(if this parameter not passed)). 
 *                 If attribute are optional, pass TreeBinding::NodesNum::NOT_SPECIFIED
 */
//#define XML_ATTR(name, ...) TREE_LEAF( TREE_BINDING_CONCAT("<xmlattr>/", name ), __VA_ARGS__)
#define XML_ATTR(name, ...) TREE_LEAF( XML_DETAILS_CONCAT("<xmlattr>", XML_DETAILS_PATH_DELIMETER, name), __VA_ARGS__)


/*!
 * \brief   XML child declaration
 * \warning Each macro call should be placed in different lines
 * \param   ... 1. Child's data type 
 *              2. Required number of childs elements (TreeBinding::FiledsNum::MORE_THAN_ONE by default(if this parameter not passed)). 
 */

// TODO: check child is inherited from Node
// TODO: implement overloading for number of elements
#define XML_CHILD_ELEMENTS(type) TREE_SUBTREES_SET("", type)


#define XML_ELEMENT(_name, dataType) TREE_TREE(_name, dataType)

// TODO: if name not passed, than use name of parameter

// TODO: add to enum option/mandatory "key" - is mandatory and is key

} /* namespace XML */

#endif /* _XML_H_ */
  
