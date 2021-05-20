/*!
 *  \file  ArchiveSerializerDecl.h
 *  \brief Declarations for converter of NodeData to boost archive
 */

#ifndef _TREE_BINDING_ARCHIVE_SERIALIZER_DECL_H_
#define _TREE_BINDING_ARCHIVE_SERIALIZER_DECL_H_

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "TreeBindingDecl.h"

namespace TreeBinding
{

namespace Details
{

class Archivable
{
public:

  virtual void serializeData(boost::archive::text_iarchive & ar, const unsigned int version) = 0;
  virtual void serializeData(boost::archive::text_oarchive & ar, const unsigned int version) = 0;

};

} /* namespace Details */

} /* namespace TreeBinding */

#endif /* _TREE_BINDING_ARCHIVE_SERIALIZER_DECL_H_ */
