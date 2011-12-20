// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/errorcheck.h"
#else
#   include "common/errorcheck.h"
#endif

// Local include(s):
#include "Crate.h"

namespace root {

   Crate::Crate()
      : dev::Crate< dev::RootDisk >(),
        m_nmgr(), m_logger( "root::Crate" ) {

   }

   Crate::~Crate() {

      m_nmgr.closeFile();
   }

   bool Crate::initialize( const QString& fileName ) {

      // Open the output file:
      CHECK( m_nmgr.openFile( fileName ) );

      //
      // Create the output variables of each device:
      //
      std::map< unsigned int, dev::RootDisk* >::const_iterator dev_itr =
         m_devices.begin();
      std::map< unsigned int, dev::RootDisk* >::const_iterator dev_end =
         m_devices.end();
      for( ; dev_itr != dev_end; ++dev_itr ) {
         CHECK( dev_itr->second->initialize( m_nmgr ) );
      }

      return true;
   }

   bool Crate::writeEvent( const ev::Event& event ) {

      // Access the fragments coming from the different modules that
      // are used in data acquisition:
      const std::vector< std::tr1::shared_ptr< ev::Fragment > >& fragments =
         event.getFragments();

      // Loop over the fragments:
      std::vector< std::tr1::shared_ptr< ev::Fragment > >::const_iterator
         fragment_itr = fragments.begin();
      std::vector< std::tr1::shared_ptr< ev::Fragment > >::const_iterator
         fragment_end = fragments.end();
      for( ; fragment_itr != fragment_end; ++fragment_itr ) {

         // Find the device that is expecting this event fragment:
         std::map< unsigned int, dev::RootDisk* >::iterator device =
            m_devices.find( ( *fragment_itr )->getModuleID() );
         if( device == m_devices.end() ) {
            REPORT_ERROR( tr( "Failed to assign fragment with "
                              "module ID: %1" )
                          .arg( ( *fragment_itr )->getModuleID() ) );
            return false;
         }

         // Give this fragment to the device that we just found:
         if( ! device->second->writeEvent( *( *fragment_itr ) ) ) {
            REPORT_ERROR( tr( "There was a problem writing the data "
                              "from device with ID: %1" )
                          .arg( ( *fragment_itr )->getModuleID() ) );
            return false;
         }

      }

      // Persistify the current event:
      CHECK( m_nmgr.saveEvent() );

      return true;
   }

   bool Crate::finalize() {

      CHECK( m_nmgr.closeFile() );

      return true;
   }

} // namespace root
