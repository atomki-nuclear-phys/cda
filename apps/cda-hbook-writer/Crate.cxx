// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Factory.h"
#   include "cdacore/common/errorcheck.h"
#else
#   include "device/Factory.h"
#   include "common/errorcheck.h"
#endif

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

      // Access the fragments coming from the different modules that
      // are used in data acquisition:
      const ev::Event::Base_t& fragments = event.getFragments();

      // Loop over the fragments:
      ev::Event::Base_t::const_iterator fragment_itr = fragments.begin();
      ev::Event::Base_t::const_iterator fragment_end = fragments.end();
      for( ; fragment_itr != fragment_end; ++fragment_itr ) {

         // Find the device that is expecting this event fragment:
         DeviceMap_t::iterator device =
            m_devices.find( ( *fragment_itr )->getModuleID() );
         if( device == m_devices.end() ) {
            REPORT_ERROR( tr( "Failed to assign fragment with "
                              "module ID: %1" )
                          .arg( ( *fragment_itr )->getModuleID() ) );
            return false;
         }

         // Give this fragment to the device that we just found:
         if( ! device->second->writeEvent( *( *fragment_itr ), m_nmgr ) ) {
            REPORT_ERROR( tr( "There was a problem writing the data "
                              "from device with ID: %1" )
                          .arg( ( *fragment_itr )->getModuleID() ) );
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
