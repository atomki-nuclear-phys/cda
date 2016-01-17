// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DAQ_CONFIG_CONFREADER_H
#define CDA_DAQ_CONFIG_CONFREADER_H

// Qt include(s):
#include <QtCore/QBuffer>
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Logger.h"
#else
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "../common/Export.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QIODevice )
class Address;

namespace conf {

   // Bring the Qt class(es) into the current scope:
   using QT_PREPEND_NAMESPACE( QIODevice );

   /**
    *  @short Class retrieving the configuration from a server
    *
    *         This class can be used in the clients that need the configuration,
    *         to read it from a configuration server. Once read, the configuration
    *         data can be accessed using the ConfReader::buffer() function.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class CDADAQ_EXPORT ConfReader {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( conf::ConfReader )

   public:
      /// Constructor
      ConfReader();

      /// Read the configuration from the specified address
      bool readFrom( const Address& address );

      /// Access the binary configuration
      QIODevice& buffer();

   private:
      QBuffer m_buffer; ///< Buffer caching the read configuration

      mutable msg::Logger m_logger; ///< Private logger object

   }; // class ConfReader

} // namespace conf

#endif // CDA_DAQ_CONFIG_CONFREADER_H
