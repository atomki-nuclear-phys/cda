// $Id$

// System include(s):
#include <cstdlib>
#include <iostream>

// Qt include(s):
#include <QtCore/QPluginLoader>
#include <QtCore/QDir>
#include <QtCore/QLibrary>

// Local include(s):
#include "Loader.h"
#include "Factory.h"

namespace dev {

   /**
    * You can either specify a concrete directory path under which
    * device plugins should be searched for, or the class can try
    * to find the "general" device plugin directory from the environment
    * setup. For the latter, the <code>CDASYS</code> environment
    * variable has to be set.
    *
    * @param path Name of the directory holding the plugins
    */
   Loader::Loader( const QString& path )
      : m_path( path ), m_logger( "dev::Loader" ) {

      //
      // Check if a directory name has been specified:
      //
      if( m_path.isEmpty() ) {
         //
         // Get the CDASYS environment variable. Remember that the
         // return value is in our responsibility...
         //
         char* env_path = getenv( "CDASYS" );
         if( ! env_path ) {
            m_path = CDASYS;
            m_path.append( "/dev" );
            m_logger << msg::DEBUG << "Setting device plugin directory "
                     << "to: " << m_path << msg::endmsg;
         } else {
            m_path = env_path;
            m_path.append( "/dev" );
            m_logger << msg::DEBUG << "Setting device plugin directory "
                     << "to: " << m_path << msg::endmsg;
         }
         //
         // Delete the return value of getenv():
         //
         // Mac OS X complains about this call, so it's removed for the moment.
         //         if( env_path ) free( env_path );
      }

   }

   /**
    * The destructor deletes all device factories instantiated
    * by this object.
    */
   Loader::~Loader() {

      for( std::map< QString, Factory* >::iterator it = m_deviceMap.begin();
           it != m_deviceMap.end(); ++it ) {
         if( it->second ) delete it->second;
      }

   }

   /**
    * The function only makes a simple search in an internal
    * <code>std::map</code> to look for the specified device name...
    *
    * @param name Name of a CAMAC device
    * @returns <code>true</code> if the device is loaded and available,
    *          <code>false</code> otherwise
    */
   bool Loader::isLoaded( const QString& name ) const {

      if( m_deviceMap.find( name ) != m_deviceMap.end() ) {
         return true;
      } else {
         return false;
      }

   }

   /**
    * This function tries to find all shared libraries in the specified
    * directory and load them one-by-one. It uses the Loader::load function
    * internally to load each plugin.
    *
    * @returns <code>true</code> if the operation was successful,
    *          <code>false</code> otherwise
    */
   bool Loader::loadAll() {

      //
      // First of all, load all the statically linked plugins:
      //
      if( ! loadStaticPlugins() ) {
         m_logger << msg::ERROR << "There was a problem loading the static "
                  << "plugins" << msg::endmsg;
         return false;
      } else {
         m_logger << msg::DEBUG << "Loaded the static plugins" << msg::endmsg;
      }

      //
      // Get the names of all physical files in the specified directory:
      //
      QDir pluginDir( m_path );
      QStringList files = pluginDir.entryList( QDir::Files | QDir::NoSymLinks |
                                               QDir::Readable );

      //
      // Loop over all file names:
      //
      for( QStringList::const_iterator pname = files.begin();
           pname != files.end(); ++pname ) {

         m_logger << msg::VERBOSE << "Evaluating: "
                  << *pname << msg::endmsg;

         //
         // Try to load the files that look like shared libraries:
         //
         if( QLibrary::isLibrary( *pname ) ) {

            m_logger << msg::VERBOSE << "Trying to load plugin: "
                     << *pname << msg::endmsg;

            if( this->load( *pname ) ) {
               m_logger << msg::VERBOSE << *pname << " loaded"
                        << msg::endmsg;
            } else {
               m_logger << msg::WARNING << *pname << " could not be loaded"
                        << msg::endmsg;
            }

         }

      }

      //
      // Always return true:
      //
      return true;

   }

   /**
    * This function is doing the heavy work. It tries to load the specified
    * library using QPluginLoader. If successful, it tries to create a
    * dev::Factory object from the plugin and cache a pointer to it.
    *
    * @param plugin_name Name of the shared library to load
    * @returns <code>true</code> if the operation was successful,
    *          <code>false</code> otherwise
    */
   bool Loader::load( const QString& plugin_name ) {

      //
      // Try to load the specified library:
      //
      QPluginLoader ploader( m_path + "/" + plugin_name );
      QObject* plugin = ploader.instance();
      if( ! plugin ) {
         m_logger << msg::ERROR << "Couldn't load plugin with name: "
                  << plugin_name << msg::endmsg;
         return false;
      } else {
         m_logger << msg::DEBUG << "Loaded plugin with name: "
                  << plugin_name << msg::endmsg;
      }

      //
      // Check if the library provides the needed interface:
      //
      dev::Factory* factory = qobject_cast< dev::Factory* >( plugin );
      if( ! factory ) {
         m_logger << msg::ERROR << plugin_name << " doesn't provide the "
                  << "dev::Factory interface!" << msg::endmsg;
         delete factory;
         return false;
      } else {
         m_logger << msg::DEBUG << "Accessed dev::Factory interface in plugin: "
                  << plugin_name << msg::endmsg;
      }

      //
      // Save a pointer to the dev::Factory implemented by the plugin
      // and report the successive plugin loading:
      //
      m_deviceMap[ factory->shortName() ] = factory;
      m_logger << msg::INFO << "Loaded device \"" << factory->longName()
               << "\" from plugin: " << plugin_name << msg::endmsg;

      return true;

   }

   /**
    * There are two types of plugins in Qt: dynamic and static plugins.
    * The static plugins are compiled into a static library and are linked
    * statically to the application. Unfortunately CERNLIB doesn't seem
    * to play nice with dynamic plugins, so the CDA applications will
    * have to link all the plugins statically.
    *
    * The function loops over all the plugins that have been statically
    * linked to the application, and puts their information in the same
    * map that is used for the dynamic plugins. So the user of this class
    * should see no differences between using a dynamic or a static plugin.
    *
    * @returns <code>true</code> if the operation was successful,
    *          <code>false</code> otherwise
    */
   bool Loader::loadStaticPlugins() {

      //
      // Get a list of all static plugins, and loop over them:
      //
      QObjectList plugin_list = QPluginLoader::staticInstances();
      for( QObjectList::iterator plugin = plugin_list.begin();
           plugin != plugin_list.end(); ++plugin ) {

         //
         // Check if the library provides the needed interface:
         //
         dev::Factory* factory = qobject_cast< dev::Factory* >( *plugin );
         if( ! factory ) {
            m_logger << msg::ERROR << "Found a static plugin not providing "
                     << "the needed interface" << msg::endmsg;
            delete factory;
            return false;
         } else {
            m_logger << msg::VERBOSE << "Accessed dev::Factory interface in "
                     << "static plugin" << msg::endmsg;
         }

         //
         // Save a pointer to the dev::Factory implemented by the plugin
         // and report the successive plugin loading:
         //
         m_deviceMap[ factory->shortName() ] = factory;
         m_logger << msg::INFO << "Loaded device \"" << factory->longName()
                  << "\" from static plugin" << msg::endmsg;

      }

      return true;

   }

   /**
    * @returns A string list with the names of the available devices
    */
   QStringList Loader::getDeviceNames() const {

      QStringList nameList;

      for( std::map< QString, Factory* >::const_iterator it =
              m_deviceMap.begin();
           it != m_deviceMap.end(); ++it ) {
         nameList.push_back( it->first );
      }

      return nameList;

   }

   /**
    * @returns A pointer to the device factory if it is available,
    *          or <code>0</code> if it isn't
    */
   Factory* Loader::getFactory( const QString& name ) const {

      if( ! isLoaded( name ) ) {
         m_logger << msg::ERROR<< "No device with name \""
                  << name << "\" currently loaded" << msg::endmsg;
         return 0;
      }

      std::map< QString, Factory* >::const_iterator it =
         m_deviceMap.find( name );

      return it->second;

   }

} // namespace dev
