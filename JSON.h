/*! 
 *  \file JSON.h
 *  \brief Macro definitions for JSON implementation
 */

#ifndef _JSON_H_
#define _JSON_H_

#include "TreeBinding/TreeBinding.h"

namespace JSON
{

/*!
 *  \copydoc TreeBinding::Integer
 */
typedef TreeBinding::Integer Integer;

/*!
 *  \copydoc TreeBinding::NodesNum
 */
typedef TreeBinding::NodesNum ItemNum;

/*!
 * \brief   JSON child declaration
 * \warning Each macro call should be placed in different lines
 * \param   ... 1. Child name. 
 * \param   ... 2. Child's data type
 */
#define JSON_CHILD(name, type) TREE_NODE(name, type, 1)

/*!
 * \brief   JSON array declaration
 * \warning Each macro call should be placed in different lines
 * \param   ... 1. Array name. 
 * \param   ... 2. Array's elements data type
 *              3. Required number of array elements (TreeBinding::NodesNum::MORE_THAN_ONE by default(if this parameter not passed)). 
 */
#define JSON_ARRAY(...) TREE_NODE(__VA_ARGS__)

/*!
 * \brief   JSON element declaration
 * \warning Each macro call should be placed in different lines
 * \param   dataType Elements's data type
 */
#define JSON_ELEMENT(dataType) TREE_TREE(dataType)

} /* namespace JSON */

#endif /* _JSON_H_ */
  
