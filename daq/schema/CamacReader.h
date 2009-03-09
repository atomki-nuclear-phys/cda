// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DAQ_SCHEMA_CAMACREADER_H
#define CDA_DAQ_SCHEMA_CAMACREADER_H

// STL include(s):
#include <vector>

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/Address.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "common/Address.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "Application.h"

namespace daq {

   class CamacReader : public Application {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( daq::CamacReader )

   public:
      CamacReader();

      void clear();

      const std::vector< Address >& getClients() const;
      void addClient( const Address& address );

      virtual bool readConfig( QIODevice* dev );
      virtual bool writeConfig( QIODevice* dev ) const;

      virtual bool readConfig( const QDomElement& node );
      virtual bool writeConfig( QDomElement& node ) const;

   private:
      std::vector< Address > m_clients;

      mutable msg::Logger m_logger;

   }; // class CamacReader

} // namespace daq

#endif // CDA_DAQ_SCHEMA_CAMACREADER_H
