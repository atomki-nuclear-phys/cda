// $Id$
/**
 * @example pluginloader.cxx
 *
 *          This example shows how to use the dev::Loader class and
 *          will later on show some ideas on how the device objects
 *          can be used.
 *
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision$
 * $Date$
 */

// Qt include(s):
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtGui/QApplication>
#include <QtXml/QDomImplementation>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

// CDA include(s):
#include "msg/Sender.h"
#include "msg/Logger.h"
#include "device/Loader.h"
#include "device/Factory.h"
#include "device/Gui.h"

int main( int argc, char* argv[] ) {

   //
   // Set up application wide and local message logging:
   //
   msg::Sender::addAddress( msg::Address( "127.0.0.1", 49700 ) );
   msg::Sender::instance()->setMinLevel( msg::VERBOSE );
   msg::Logger logger( "pluginloader" );

   logger << msg::INFO << "Plugin loader example started" << msg::endmsg;

   //
   // Start a graphical Qt application:
   //
   QApplication app( argc, argv );

   //
   // Try to load all available device plugins:
   //
   dev::Loader loader;
   if( loader.loadAll() ) {
      logger << msg::INFO << "Successfully loaded all available devices"
             << msg::endmsg;
   } else {
      logger << msg::FATAL << "There was an error loading the devices"
             << msg::endmsg;
   }

   //
   // List the names of all loaded devices:
   //
   QStringList devList = loader.getDeviceNames();
   logger << msg::INFO << "Loaded the following devices:" << std::endl;
   for( QStringList::const_iterator dev = devList.begin();
        dev != devList.end(); ++dev ) {
      logger << "  - " << *dev << std::endl;
   }
   logger << msg::endmsg;

   //
   // If there was at least one device plugin, show its configuration
   // window:
   //
   if( devList.size() ) {

      dev::Factory* factory = loader.getFactory( devList.front() );
      dev::Gui* gui = factory->createGui();
      gui->show();

      app.exec();

      QDomImplementation imp;
      QDomDocument doc = imp.createDocument( "", "Setup",
                                             imp.createDocumentType( "CDA_SETUP",
                                                                     "", "" ) );
      QDomElement rootElement = doc.documentElement();

      QDomElement deviceElement = doc.createElement( "Example" );
      gui->writeConfig( &deviceElement );

      rootElement.appendChild( deviceElement );

      QFile configFile( "example_config.xml" );
      configFile.open( QFile::WriteOnly | QFile::Truncate );
      QTextStream stream( &configFile );
      doc.save( stream, 3 );

   } else {

      logger << msg::FATAL << "No devices could be loaded!" << msg::endmsg;
      return 1;

   }

}
