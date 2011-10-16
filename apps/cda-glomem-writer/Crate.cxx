// $Id$

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

namespace glomem {

   /**
    * The constructor has to carefully create the base class. Apart from
    * this, it doesn't have to do much.
    */
   Crate::Crate()
      : dev::Crate< dev::CernlibHist >(),
        m_hmgr(), m_logger( "glomem::Crate" ) {

      m_logger << msg::VERBOSE << tr( "Object constructed" )
               << msg::endmsg;
   }

   /**
    * The destructor doesn't really have to do anything...
    */
   Crate::~Crate() {

      m_logger << msg::VERBOSE << tr( "Object destructed" )
               << msg::endmsg;
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

      m_logger << msg::INFO
               << tr( "Initializing histograms in global memory" )
               << msg::endmsg;

      //
      // Initialise the global memory:
      //
      m_hmgr.initialize();

      //
      // Let the devices create their own monitoring histograms:
      //
      std::map< unsigned int, dev::CernlibHist* >::const_iterator dev_itr =
         m_devices.begin();
      std::map< unsigned int, dev::CernlibHist* >::const_iterator dev_end =
         m_devices.end();
      for( ; dev_itr != dev_end; ++dev_itr ) {

         if( ! dev_itr->second->initialize( m_hmgr ) ) {
            m_logger << msg::ERROR
                     << tr( "There was a problem initializing one of "
                            "the devices" )
                     << msg::endmsg;
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

      m_logger << msg::VERBOSE << tr( "Received new event" )
               << msg::endmsg;

      const std::vector< ev::Fragment >& fragments = event.getFragments();

      std::vector< ev::Fragment >::const_iterator frag_itr = fragments.begin();
      std::vector< ev::Fragment >::const_iterator frag_end = fragments.end();
      for( ; frag_itr != frag_end; ++frag_itr ) {

         std::map< unsigned int, dev::CernlibHist* >::iterator device =
            m_devices.find( frag_itr->getModuleID() );
         if( device == m_devices.end() ) {
            m_logger << msg::ERROR
                     << tr( "Failed to assign fragment with "
                            "module ID: %1" ).arg( frag_itr->getModuleID() )
                     << msg::endmsg;
            return false;
         }

         if( ! device->second->displayEvent( *frag_itr, m_hmgr ) ) {
            m_logger << msg::ERROR
                     << tr( "There was a problem displaying the data "
                            "from device with ID: %1" )
               .arg( frag_itr->getModuleID() )
                     << msg::endmsg;
            return false;
         }

      }

      return true;
   }

} // namespace glomem
