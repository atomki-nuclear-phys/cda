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
            m_logger << msg::ERROR << "There was a problem initializing one of "
                     << "the devices" << msg::endmsg;
            return false;
         }

      }

      if( ! m_nmgr.openFile( fileName ) ) {
         m_logger << msg::ERROR << "The output file couldn't be opened"
                  << msg::endmsg;
         return false;
      } else {
         m_logger << msg::DEBUG << "The output file was opened"
                  << msg::endmsg;
      }

      return true;

   }

   bool Crate::writeEvent( const ev::Event& event ) {

      const std::vector< ev::Fragment >& fragments = event.getFragments();

      for( std::vector< ev::Fragment >::const_iterator fragment =
              fragments.begin(); fragment != fragments.end(); ++fragment ) {

         std::map< int, dev::Disk* >::iterator device =
            m_devices.find( fragment->getModuleNumber() );
         if( device == m_devices.end() ) {
            m_logger << msg::ERROR << "Failed to assign fragment with "
                     << "module number: " << fragment->getModuleNumber()
                     << msg::endmsg;
            return false;
         }

         if( ! device->second->writeEvent( *fragment, m_nmgr ) ) {
            m_logger << msg::ERROR << "There was a problem writing the data "
                     << "from a device" << msg::endmsg;
            return false;
         }

      }

      m_nmgr.saveEvent();

      return true;

   }

   bool Crate::finalize() {

      m_nmgr.closeFile();

      return true;

   }

}
