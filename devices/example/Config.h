// Dear emacs, this is -*- c++ -*-
#ifndef CDA_DEVICES_EXAMPLE_CONFIG_H
#define CDA_DEVICES_EXAMPLE_CONFIG_H

// CDA include(s):
#include "device/Config.h"
#include "msg/Logger.h"

namespace camac {

   //
   // Make sure that the following Qt classes are available in the
   // DEV namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QIODevice );
   using QT_PREPEND_NAMESPACE( QDomNode );

   class Config : virtual public dev::Config {

   public:
      Config();

      /// Function reading the configuration in binary format
      virtual bool readConfig( QIODevice* dev );
      /// Function writing the configuration in binary format
      virtual bool writeConfig( QIODevice* dev ) const;

      /// Function reading the configuration in XML format
      virtual bool readConfig( const QDomNode* node );
      /// Function writing the configuration in XML format
      virtual bool writeConfig( QDomNode* node ) const;

   protected:
      bool    m_booleanProperty; ///< An example boolean property
      int     m_integerProperty; ///< An example integer property
      double  m_doubleProperty;  ///< An example double property
      QString m_stringProperty;  ///< An example string property

   private:
      mutable msg::Logger m_logger; ///< Logger member

   }; // class Config

} // namespace camac

#endif // CDA_DEVICES_EXAMPLE_CONFIG_H
