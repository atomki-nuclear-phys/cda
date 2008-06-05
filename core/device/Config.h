// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICE_CONFIG_H
#define CDA_CORE_DEVICE_CONFIG_H

// Qt include(s):
#include <QtCore/QtGlobal>

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QIODevice )
QT_FORWARD_DECLARE_CLASS( QDomNode )

namespace dev {

   //
   // Make sure that the following Qt classes are available in the
   // DEV namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QIODevice );
   using QT_PREPEND_NAMESPACE( QDomNode );

   /**
    *  @short Base interface for configuring a CAMAC device
    *
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Config {

   public:
      virtual ~Config() {}

      /// Function reading the configuration in binary format
      virtual bool readConfig( QIODevice* dev ) = 0;
      /// Function writing the configuration in binary format
      virtual bool writeConfig( QIODevice* dev ) const = 0;

      /// Function reading the configuration in XML format
      virtual bool readConfig( const QDomNode* node ) = 0;
      /// Function writing the configuration in XML format
      virtual bool writeConfig( QDomNode* node ) const = 0;

   }; // class Config

} // namespace dev

#endif // CDA_CORE_DEVICE_CONFIG_H
