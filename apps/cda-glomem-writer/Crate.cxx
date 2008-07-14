// $Id$

// CERNLIB include(s):
extern "C" {
#   include <cfortran/cfortran.h>
#   include <cfortran/hbook.h>
}

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Factory.h"
#else
#   include "device/Factory.h"
#endif

// Local include(s):
#include "Crate.h"

// Size of the PAW global memory:
#define PAWC_SIZE 100000

namespace glomem {

   /**
    * The constructor has to carefully create the base class. Apart from
    * this, it doesn't have to do much.
    */
   Crate::Crate()
      : dev::Crate< dev::Hist >( &dev::Factory::createHist ),
        m_logger( "Crate" ) {

      m_logger << msg::VERBOSE << "Object constructed" << msg::endmsg;

   }

   /**
    * The destructor doesn't really have to do anything...
    */
   Crate::~Crate() {

      m_logger << msg::VERBOSE << "Object destructed" << msg::endmsg;

   }

   /**
    * This function instructs all the configured devices to create their
    * monitoring histograms in a PAW global memory. The PAW global memory
    * name is hard-wired to be "CDA" for the moment, but it could become
    * configurable later on. (Although this configuration is never used in
    * other applications that support it... :-/)
    *
    * @returns <code>true</code> if the operation was successful,
    *          <code>false</code> otherwise
    */
   bool Crate::initialize() {

      m_logger << msg::INFO << "Initializing histograms in global memory"
               << msg::endmsg;

      //
      // Initialise the global memory:
      //
      HLIMAP( PAWC_SIZE, "CDA" );

      //
      // Let the devices create their own monitoring histograms:
      //
      unsigned int counter = 0;
      for( std::map< int, dev::Hist* >::iterator device = m_devices.begin();
           device != m_devices.end(); ++device ) {

         if( ! device->second->initialize( counter ) ) {
            m_logger << msg::ERROR << "There was a problem initializing one of "
                     << "the devices" << msg::endmsg;
            return false;
         }

      }

      return true;

   }

   /**
    * This function fills all the monitoring histograms with the contents
    * of one full event. It inspects each event fragment separately and
    * tries to assign them to modules in the crate. If a fragment from
    * a non-existent module is found, an error is printed. This is most
    * likely a sign of mis-configuration, so in this case the execution
    * should be stopped...
    *
    * @param event The event that should be filled in the histograms
    * @returns <code>true</code> if the operation was successful,
    *          <code>false</code> otherwise
    */
   bool Crate::displayEvent( const ev::Event& event ) {

      m_logger << msg::VERBOSE << "Received new event" << msg::endmsg;

      const std::vector< ev::Fragment >& fragments = event.getFragments();

      for( std::vector< ev::Fragment >::const_iterator fragment =
              fragments.begin(); fragment != fragments.end(); ++fragment ) {

         std::map< int, dev::Hist* >::iterator device =
            m_devices.find( fragment->getModuleNumber() );
         if( device == m_devices.end() ) {
            m_logger << msg::ERROR << "Failed to assign fragment with "
                     << "module number: " << fragment->getModuleNumber()
                     << msg::endmsg;
            return false;
         }

         device->second->displayEvent( *fragment );

      }

      return true;

   }

} // namespace glomem
