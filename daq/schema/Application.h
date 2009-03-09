// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DAQ_SCHEMA_APPLICATION_H
#define CDA_DAQ_SCHEMA_APPLICATION_H

// STL include(s):
#include <vector>

// Qt include(s):
#include <QtCore/QString>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/Address.h"
#else
#   include "common/Address.h"
#endif

// Local include(s):
#include "Config.h"

namespace daq {

   class Application : public Config {

   public:
      Application();

      void clear();

      const QString& getName() const;
      void setName( const QString& name );

      const std::vector< Address >& getMsgServers() const;
      void addMsgServer( const Address& address );

   private:
      QString m_name;
      std::vector< Address > m_msgServers;

   }; // class Application

} // namespace daq

#endif // CDA_DAQ_SCHEMA_APPLICATION_H
