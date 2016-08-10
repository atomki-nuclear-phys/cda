// $Id$

// Qt include(s):
#include <QtCore/QDataStream>
#include <QtXml/QDomElement>

// CDA include(s):
#include "common/errorcheck.h"

// Local include(s):
#include "Crate.h"

namespace caen_vme_reader {

   Crate::Crate()
      : dev::Crate< dev::ICaenVmeReadout >( "CAENVME", true ),
        m_vmeBus(), m_type( caen::VmeBus::BOARD_V1718 ), m_linkNumber( 0 ),
        m_boardNumber( 0 ),
        m_initialized( false ),
        m_logger( "caen_vme_reader::Crate" ) {

   }

   Crate::~Crate() {

      // Force a finalization, as that often needs to close devices and
      // free up memory:
      if( m_initialized ) {
         m_logger << msg::WARNING
                  << tr( "Object getting deleted without being finalized!" )
                  << msg::endmsg;
         finalize();
      }
   }

   bool Crate::initialize() {

      // Open the connection to the VME bus:
      CHECK( m_vmeBus.open( m_type, m_linkNumber, m_boardNumber ) );

      // Initialize all the devices:
      for( auto& itr : m_devices ) {
         CHECK( itr.second->initialize( m_vmeBus ) );
      }

      // Remember the object's internal state:
      m_initialized = true;

      // Show that we were successful:
      return true;
   }

   bool Crate::finalize() {

      // Don't do anything if the object is not initialized:
      if( ! m_initialized ) {
         m_logger << msg::WARNING
                  << tr( "Object not in initialized state, not finalizing" )
                  << msg::endmsg;
         return true;
      }

      // Finalize all the devices:
      for( auto& itr : m_devices ) {
         CHECK( itr.second->finalize() );
      }

      // Close the VME bus:
      CHECK( m_vmeBus.close() );

      // Remember the object's internal state:
      m_initialized = false;

      // Show that we were successful:
      return true;
   }

   bool Crate::start() {

      // Start all the devices:
      for( auto& itr : m_devices ) {
         CHECK( itr.second->start() );
      }

      // Show that we were successful:
      return true;
   }

   bool Crate::stop() {

      // Stop all the devices:
      for( auto& itr : m_devices ) {
         CHECK( itr.second->stop() );
      }

      // Show that we were successful:
      return true;
   }

   ev::Event Crate::readEvent() const {

      // The event object to return:
      ev::Event event;

      // Read out all the devices:
      for( auto& itr : m_devices ) {
         event.addFragment( itr.second->readEvent() );
      }

      // Return the event:
      return event;
   }

   bool Crate::devicesAreInSync() const {

      // First a simple security check:
      if( ! m_devices.size() ) {
         // Not sure what's happening here, but sure...
         return true;
      }

      // Get the number of events processed by the first device:
      const size_t eventsProcessed =
         m_devices.begin()->second->eventsProcessed();

      // Check if every device reports the same:
      for( const auto& itr : m_devices ) {
         if( eventsProcessed != itr.second->eventsProcessed() ) {
            m_logger << msg::DEBUG
                     << tr( "First device (%1) reports %2 events" )
                        .arg( m_devices.begin()->second->deviceName() )
                        .arg( eventsProcessed ) << std::endl
                     << tr( "Device %1 reports %2 events" )
                        .arg( itr.second->deviceName() )
                        .arg( itr.second->eventsProcessed() ) << msg::endmsg;
            return false;
         }
      }

      // If yes, then they are in sync:
      return true;
   }

   StatusCode Crate::readCrateConfig( QIODevice& dev ) {

      // Create the object used for reading the data:
      QDataStream input( &dev );
      input.setVersion( QDataStream::Qt_4_0 );

      // Read the connector type:
      quint16 type = 0;
      input >> type;
      m_type = static_cast< caen::VmeBus::BoardType >( type );

      // Read the link number:
      qint16 linkNumber = 0;
      input >> linkNumber;
      m_linkNumber = linkNumber;

      // Read the board number:
      qint16 boardNumber = 0;
      input >> boardNumber;
      m_boardNumber = boardNumber;

      // Tell the user what happened:
      REPORT_VERBOSE( tr( "Read the following VME controller parameters:\n"
                          "  type        : %1\n"
                          "  link number : %2\n"
                          "  board number: %3" ).arg( m_type )
                      .arg( m_linkNumber ).arg( m_boardNumber ) );

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   StatusCode Crate::readCrateConfig( const QDomElement& node ) {

      // A helper variable:
      bool ok = true;

      // Read the controller type:
      const int type = node.attribute( "ControllerType", "0" ).toInt( &ok );
      CHECK( ok );
      m_type = static_cast< caen::VmeBus::BoardType >( type );

      // Read the link number:
      const int linkNumber = node.attribute( "LinkNumber", "0" ).toInt( &ok );
      CHECK( ok );
      m_linkNumber = static_cast< short >( linkNumber );

      // Read the board number:
      const int boardNumber = node.attribute( "BoardNumber", "0" ).toInt( &ok );
      CHECK( ok );
      m_boardNumber = static_cast< short >( boardNumber );

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

} // namespace caen_reader
