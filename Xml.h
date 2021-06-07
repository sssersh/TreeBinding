/*! 
 *  \file Xml.h
 *  \brief Macro definitions for XML implementation 
 */

#ifndef _XML_H_
#define _XML_H_

#include "TreeBinding/TreeBinding.h"

namespace XML
{

/*!
 *  \copydoc TreeBinding::Integer
 */
typedef TreeBinding::Integer Integer;

/*!
 *  \copydoc TreeBinding::NodesNum
 */
typedef TreeBinding::NodesNum ItemNum;



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
#define XML_ATTR(name, ...) TREE_NODE( XML_DETAILS_CONCAT("<xmlattr>", XML_DETAILS_PATH_DELIMETER, name), __VA_ARGS__)

/*!
 * \brief   XML child declaration
 * \warning Each macro call should be placed in different lines
 * \param   ... 1. Child's data type 
 *              2. Required number of childs elements (TreeBinding::NodesNum::MORE_THAN_ONE by default(if this parameter not passed)). 
 */
#define XML_CHILD_ELEMENTS(...) TREE_NODE("", __VA_ARGS__)


/*!
 * \brief   XML element declaration
 * \warning Each macro call should be placed in different lines
 * \param   ... 1. Element name (in file)
 *              2. Data type name
 */
#define XML_ELEMENT(name, dataType) TREE_TREE(name, dataType)

} /* namespace XML */

#endif /* _XML_H_ */
  
