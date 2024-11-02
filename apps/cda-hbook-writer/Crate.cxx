// $Id$

// CDA include(s):
#include "device/Factory.h"
#include "common/errorcheck.h"

// Local include(s):
#include "Crate.h"

namespace hbook {

   Crate::Crate()
      : dev::Crate< dev::ICernlibDisk >(),
        m_nmgr(), m_logger( "hbook::Crate" ) {

   }

   Crate::~Crate() {

      m_nmgr.closeFile();
   }

   bool Crate::initialize( const QString& fileName ) {

      m_nmgr.clear();

      //
      // Create the output variables of each device:
      //
      DeviceMap_t::const_iterator dev_itr = m_devices.begin();
      DeviceMap_t::const_iterator dev_end = m_devices.end();
      for( ; dev_itr != dev_end; ++dev_itr ) {
         CHECK( dev_itr->second->initialize( m_nmgr ) );
      }

      if( ! m_nmgr.openFile( fileName ) ) {
         REPORT_ERROR( tr( "The output file couldn't be opened" ) );
         return false;
      } else {
         m_logger << msg::DEBUG << tr( "The output file was opened" )
                  << msg::endmsg;
      }

      return true;
   }

   bool Crate::writeEvent( const ev::Event& event ) {

      // Loop over the fragments coming from the different modules that
      // are used in data acquisition:
      for( auto& fragment : event.getFragments() ) {

         // Find the device that is expecting this event fragment:
         DeviceMap_t::iterator device =
            m_devices.find( fragment->getModuleID() );
         if( device == m_devices.end() ) {
            REPORT_ERROR( tr( "Failed to assign fragment with "
                              "module ID: %1" )
                          .arg( fragment->getModuleID() ) );
            return false;
         }

         // Give this fragment to the device that we just found:
         if( ! device->second->writeEvent( *fragment, m_nmgr ) ) {
            REPORT_ERROR( tr( "There was a problem writing the data "
                              "from device with ID: %1" )
                          .arg( fragment->getModuleID() ) );
            return false;
         }

      }

      // Persistify the current event:
      m_nmgr.saveEvent();

      return true;
   }

   bool Crate::finalize() {

      m_nmgr.closeFile();

      return true;
   }

} // namespace hbook
