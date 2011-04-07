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

namespace hbook {

   Crate::Crate()
      : dev::Crate< dev::Disk >( &dev::Factory::createDisk ),
        m_nmgr(), m_logger( "hbook::Crate" ) {

   }

   Crate::~Crate() {

      m_nmgr.closeFile();
   }

   bool Crate::initialize( const QString& fileName ) {

      m_nmgr.clear();

      for( std::map< int, dev::Disk* >::iterator device = m_devices.begin();
           device != m_devices.end(); ++device ) {

         if( ! device->second->initialize( m_nmgr ) ) {
            m_logger << msg::ERROR
                     << tr( "There was a problem initializing one of "
                            "the devices" ) << msg::endmsg;
            return false;
         }

      }

      if( ! m_nmgr.openFile( fileName ) ) {
         m_logger << msg::ERROR
                  << tr( "The output file couldn't be opened" )
                  << msg::endmsg;
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
      const std::vector< ev::Fragment >& fragments = event.getFragments();

      // Loop over the fragments:
      std::vector< ev::Fragment >::const_iterator fragment_itr = fragments.begin();
      std::vector< ev::Fragment >::const_iterator fragment_end = fragments.end();
      for( ; fragment_itr != fragment_end; ++fragment_itr ) {

         // Find the device that is expecting this event fragment:
         std::map< int, dev::Disk* >::iterator device =
            m_devices.find( fragment_itr->getModuleNumber() );
         if( device == m_devices.end() ) {
            m_logger << msg::ERROR
                     << tr( "Failed to assign fragment with "
                            "module number: %1" ).arg( fragment_itr->getModuleNumber() )
                     << msg::endmsg;
            return false;
         }

         // Give this fragment to the device that we just found:
         if( ! device->second->writeEvent( *fragment_itr, m_nmgr ) ) {
            m_logger << msg::ERROR
                     << tr( "There was a problem writing the data "
                            "from device in slot: %1" ).arg( fragment_itr->getModuleNumber() )
                     << msg::endmsg;
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
