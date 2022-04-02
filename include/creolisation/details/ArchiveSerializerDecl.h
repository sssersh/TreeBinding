/*!
 *  \file  ArchiveSerializerDecl.h
 *  \brief Declarations for converter of NodeData to boost archive
 */

#ifndef _CREOLISATION_ARCHIVE_SERIALIZER_DECL_H_
#define _CREOLISATION_ARCHIVE_SERIALIZER_DECL_H_

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "creolisation/Details/Core/creolisationDecl.h"

namespace creolisation
{

namespace Details
{

class Archivable
{
public:

  virtual void serializeData(boost::archive::text_iarchive & ar, const unsigned int version) = 0;
  virtual void serializeData(boost::archive::text_oarchive & ar, const unsigned int version) = 0;

};

} /* namespace details */

} /* namespace data_binding */

#endif /* _CREOLISATION_ARCHIVE_SERIALIZER_DECL_H_ */
