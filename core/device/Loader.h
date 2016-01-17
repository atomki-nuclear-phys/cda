// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICE_LOADER_H
#define CDA_CORE_DEVICE_LOADER_H

// STL include(s):
#include <map>

// Qt include(s):
#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QtCore/QStringList>

// CDA include(s):
#include "../msg/Logger.h"
#include "../common/Export.h"

namespace dev {

   // Forward declaration(s):
   class Factory;

   // Make sure that the Qt classes are in scope:
   using QT_PREPEND_NAMESPACE( QString );
   using QT_PREPEND_NAMESPACE( QStringList );

   /**
    *  @short Device plugin loader
    *
    *         This class can be used to load device plugins in an
    *         application. The code handling each CAMAC device should
    *         end up in its own plugin (shared library). This class
    *         is able to look for the plugins and try to correctly
    *         load them.
    *
    *         In principle it should be even pretty platform-independent.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class CDACORE_EXPORT Loader {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( dev::Loader )

   public:
      /// Access the singleton instance
      static Loader* instance( const QString& path = "" );

      /// Destructor
      ~Loader();

      /// Function checking if a certain type of device is loaded
      bool isLoaded( const QString& name ) const;
      /// Function loading all available device plugins
      bool loadAll();
      /// Function loading a specific device plugin
      bool load( const QString& plugin_name );
      /// Function "loading" all the statically linked plugins
      bool loadStaticPlugins();

      /// Get the names of the loaded devices
      QStringList getDeviceNames() const;
      /// Get the factory for a specific device
      Factory&    getFactory( const QString& name ) const;

   private:
      /// Constructor specifying the directory of the plugins
      Loader( const QString& path = "" );

      static Loader* m_instance; ///< The singleton instance

      QString                       m_path;      ///< Plugin directory
      std::map< QString, Factory* > m_deviceMap; ///< Loaded devices
      mutable msg::Logger           m_logger;    ///< Private logger object

   }; // class Loader

} // namespace dev

#endif // CDA_CORE_DEVICE_LOADER_H
