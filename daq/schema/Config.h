// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DAQ_SCHEMA_CONFIG_H
#define CDA_DAQ_SCHEMA_CONFIG_H

// Qt include(s):
#include <QtCore/QtGlobal>

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QIODevice )
QT_FORWARD_DECLARE_CLASS( QDomElement )

namespace daq {

   using QT_PREPEND_NAMESPACE( QIODevice );
   using QT_PREPEND_NAMESPACE( QDomElement );

   class Config {

   public:
      virtual ~Config() {}

      virtual bool readConfig( QIODevice* dev ) = 0;
      virtual bool writeConfig( QIODevice* dev ) const = 0;

      virtual bool readConfig( const QDomElement& node ) = 0;
      virtual bool writeConfig( QDomElement& node ) const = 0;

   }; // class Config

} // namespace daq

#endif // CDA_DAQ_SCHEMA_CONFIG_H
