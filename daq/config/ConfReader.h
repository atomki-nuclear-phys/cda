// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DAQ_CONFIG_CONFREADER_H
#define CDA_DAQ_CONFIG_CONFREADER_H

// Qt include(s):
#include <QtCore/QBuffer>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Logger.h"
#else
#   include "msg/Logger.h"
#endif

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QIODevice )
class Address;

namespace conf {

   using QT_PREPEND_NAMESPACE( QIODevice );

   class ConfReader {

   public:
      ConfReader();

      bool readFrom( const Address& address );

      QIODevice* buffer();

   private:
      QBuffer m_buffer;

      mutable msg::Logger m_logger;

   }; // class ConfReader

} // namespace conf

#endif // CDA_DAQ_CONFIG_CONFREADER_H
