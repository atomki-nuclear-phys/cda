// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/event/Fragment.h"
#   include "cdacore/common/errorcheck.h"
#   include "cdacore/common/Sleep.h"
#else
#   include "event/Fragment.h"
#   include "common/errorcheck.h"
#   include "common/Sleep.h"
#endif

// Local include(s):
#include "Readout.h"

namespace dt5740 {

   Readout::Readout()
      : dev::CaenReadout(), Device(), m_digitizer(),
        m_buffer( NULL ), m_bufferSize( 0 ), m_eventSize( 0 ),
        m_numEvents( 0 ), m_currentEvent( 0 ),
        m_logger( "dt5740::Readout" ) {

   }

   Readout::~Readout() {

      cleanup();
   }

   bool Readout::initialize() {

      // Try opening the connection to the device:
      CHECK( m_digitizer.open( m_connType, m_linkNumber ) );

      // Let's start from a default state, so start with resetting the
      // digitizer:
      CHECK( m_digitizer.reset() );

      // Test the connection to the digitizer by trying to write a value
      // to the test register, and then reading it back:
      CHECK( m_digitizer.writeRegister( REG_SCRATCH, 0x383838 ) );
      uint32_t dummy = 0;
      CHECK( m_digitizer.readRegister( REG_SCRATCH, dummy ) );
#ifdef HAVE_CAEN_LIBS
      // This should only be checked when actually communicating
      // with the hardware.
      CHECK( dummy == 0x383838 );
#endif // HAVE_CAEN_LIBS

      // Print some information about the board for debugging:
      CHECK( m_digitizer.printInfo( msg::DEBUG ) );

      // Set up the triggering:
      CHECK( m_digitizer.setSWTriggerMode( caen::Digitizer::TRIG_AcqOnly ) );
      CHECK( m_digitizer.setExtTriggerMode( extTrigMode() ) );
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {
         CHECK( m_digitizer.setGroupSelfTriggerMode( i,
                                                     trigMode( m_groups[ i ] ) ) );
      }

      // Configure the device wide parameters:
      CHECK( m_digitizer.setRecordLength( getSamples() ) );
      uint32_t recordLength = 0;
      CHECK( m_digitizer.getRecordLength( recordLength ) );
      REPORT_VERBOSE( tr( "Record length set to %1 samples (%2 samples set)" )
                      .arg( recordLength ).arg( getSamples() ) );
      CHECK( m_digitizer.setPostTriggerSize( m_postTrigPercentage ) );
      uint32_t postTrigPerc = 0;
      CHECK( m_digitizer.getPostTriggerSize( postTrigPerc ) );
      REPORT_VERBOSE( tr( "Post trigger percentage set to %1\%" )
                      .arg( postTrigPerc ) );
      CHECK( m_digitizer.writeRegister( REG_GROUP_CONFIG,
                                        groupConfReg() ) );
      uint32_t groupConf = 0;
      CHECK( m_digitizer.readRegister( REG_GROUP_CONFIG,
                                       groupConf ) );
      REPORT_VERBOSE( tr( "Group config register set to %1" )
                      .arg( groupConf ) );
      CHECK( m_digitizer.writeRegister( REG_ACQ_CONTROL,
                                        acqControlReg() ) );
      uint32_t acqControl = 0;
      CHECK( m_digitizer.readRegister( REG_ACQ_CONTROL,
                                       acqControl ) );
      REPORT_VERBOSE( tr( "Acquisition control register set to %1" )
                      .arg( acqControl ) );
      CHECK( m_digitizer.setAcquisitionMode( m_acqMode ) );

      // Configure the group wide parameters:
      uint32_t groupMask = 0;
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {
         CHECK( m_digitizer.setGroupTriggerThreshold( i,
                                                      m_groups[ i ].getTrigThreshold() ) );
         CHECK( m_digitizer.setGroupDCOffset( i, m_groups[ i ].getDCOffset() ) );
         CHECK( m_digitizer.setChannelGroupMask( i, m_groups[ i ].getTrigMask() ) );
         // Check if any channels are active in this group:
         bool groupActive = false;
         for( int j = 0; j < GroupConfig::CHANNELS_IN_GROUP; ++j ) {
            if( m_groups[ i ].getChannel( j ) ) {
               groupActive = true;
               break;
            }
         }
         if( groupActive ) {
            groupMask |= ( 0x1 << i );
         }
      }
      CHECK( m_digitizer.setGroupEnableMask( groupMask ) );

      // Check the status of the groups:
      /*
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {
         uint32_t data = 0;
         CHECK( m_digitizer.readRegister( 0x1080 + i * 0x100,
                                          data ) );
         REPORT_VERBOSE( tr( "Group %1's trigger threshold is %2" )
                         .arg( i ).arg( data ) );
         CHECK( m_digitizer.readRegister( 0x1088 + i * 0x100,
                                          data ) );
         REPORT_VERBOSE( tr( "Group %1's status word is %2" )
                         .arg( i ).arg( data ) );
         CHECK( m_digitizer.readRegister( 0x1098 + i * 0x100,
                                          data ) );
         REPORT_VERBOSE( tr( "Group %1's DC offset is %2" )
                         .arg( i ).arg( data ) );
         CHECK( m_digitizer.readRegister( 0x10a8 + i * 0x100,
                                          data ) );
         REPORT_VERBOSE( tr( "Group %1's trigger mask is %2" )
                         .arg( i ).arg( data ) );
      }
      */

      uint32_t buffMode = 0;
      CHECK( m_digitizer.readRegister( 0x800c, buffMode ) );
      REPORT_VERBOSE( tr( "Buffer mode code: %1" ).arg( buffMode ) );
      uint32_t trigSource = 0;
      CHECK( m_digitizer.readRegister( 0x810c, trigSource ) );
      REPORT_VERBOSE( tr( "Trigger Source Enable Mask: %1" )
                      .arg( trigSource ) );

      // Allocate the memory buffer for the event readout:
      CHECK( m_digitizer.mallocReadoutBuffer( &m_buffer, m_bufferSize ) );
      m_logger << msg::DEBUG
               << tr( "Allocated a %1 byte buffer for the event readout" )
         .arg( m_bufferSize )
               << msg::endmsg;

      // Reset the variables used during event readout:
      m_eventSize = 0;
      m_numEvents = 0;
      m_currentEvent = 0;

      return true;
   }

   bool Readout::finalize() {

      // Free up the allocated readout buffer:
      CHECK( cleanup() );

      // Clear and close the digitizer:
      CHECK( m_digitizer.reset() );
      CHECK( m_digitizer.close() );

      return true;
   }

   bool Readout::start() {

      // Start the acquisition:
      CHECK( m_digitizer.startAcquisition() );
      // Send a SW trigger, otherwise the device doesn't seem to start
      // normal operation...
      CHECK( m_digitizer.sendSWTrigger() );

      return true;
   }

   bool Readout::stop() {

      // Stop the acquisition:
      CHECK( m_digitizer.stopAcquisition() );

      return true;
   }

   ev::Fragment* Readout::readEvent() {

      // Create the new event fragment:
      ev::Fragment* result = new ev::Fragment();
      result->setModuleID( getID() );

      // Read a new (set of) event(s) if the event buffer is empty:
      if( ! m_numEvents ) {
         // Wait until there are events in the hardware's memory:
         for( ; ; ) {
            // Get the number of available events:
            uint32_t events = 0;
            if( ! m_digitizer.readRegister( REG_EVENT_STORED,
                                            events ) ) {
               REPORT_ERROR( tr( "Couldn't read number of available events" ) );
               return result;
            }
#ifndef HAVE_CAEN_LIBS
            // When in testing mode, let's not wait around here...
            events = 1;
#endif // HAVE_CAEN_LIBS
            // If there are events available, exit the loop:
            if( events ) {
               break;
            }
            // If not, then wait a bit and then try again:
            common::SleepMin();
         }

         // Pull all events into the computer's memory:
         if( ! m_digitizer.readData( caen::Digitizer::READ_SlaveTerminatedMBLT,
                                     m_buffer, m_eventSize ) ) {
            REPORT_ERROR( tr( "Couldn't read data from the device" ) );
            return result;
         }

         // Decode how many events we just retrieved:
         if( ! m_digitizer.getNumEvents( m_buffer, m_eventSize,
                                         m_numEvents ) ) {
            REPORT_ERROR( tr( "Couldn't get the number of events for the "
                              "current readout buffer" ) );
            return result;
         }
         m_currentEvent = 0;
      }

      // Read out the next event from the buffer:
      if( ! m_digitizer.getEvent( m_buffer, m_bufferSize,
                                  m_currentEvent, m_eventInfo,
                                  m_event ) ) {
         REPORT_ERROR( tr( "Couldn't decode event from buffer" ) );
         return result;
      }

      // Encode the information into the event fragment:
      if( ! encode( m_eventInfo, m_event, *result ) ) {
         REPORT_ERROR( tr( "Couldn't encode event into event fragment" ) );
         return result;
      }

      // Switch to the next event:
      ++m_currentEvent;

      // If the buffer is out of events, tell the function that it should
      // do a new readout the next time it's called:
      if(  m_currentEvent >= m_numEvents ) {
         m_numEvents = 0;
      }

      return result;
   }

   caen::Digitizer::TriggerMode Readout::extTrigMode() const {

      if( m_extTrigEnabled && m_extTrigOutEnabled ) {
         return caen::Digitizer::TRIG_AcqAndExt;
      } else if( m_extTrigEnabled ) {
         return caen::Digitizer::TRIG_AcqOnly;
      } else if( m_extTrigOutEnabled ) {
         return caen::Digitizer::TRIG_ExtOnly;
      } else {
         return caen::Digitizer::TRIG_Disabled;
      }
   }

   caen::Digitizer::TriggerMode Readout::trigMode( const GroupConfig& group ) {

      if( group.getTrigEnabled() && group.getTrigOutEnabled() ) {
         return caen::Digitizer::TRIG_AcqAndExt;
      } else if( group.getTrigEnabled() ) {
         return caen::Digitizer::TRIG_AcqOnly;
      } else if( group.getTrigOutEnabled() ) {
         return caen::Digitizer::TRIG_ExtOnly;
      } else {
         return caen::Digitizer::TRIG_Disabled;
      }
   }

   uint32_t Readout::groupConfReg() const {

      uint32_t result = 0x10;

      if( m_gateMode == SingleShotGate ) {
         result |= 0x1;
      }
      if( m_trigOvlpEnabled ) {
         result |= 0x2;
      }
      if( m_patGenEnabled ) {
         result |= 0x8;
      }
      if( m_trigMode == TriggerOnInputUnderThreshold ) {
         result |= 0x40;
      }

      return result;
   }

   uint32_t Readout::acqControlReg() const {

      uint32_t result = 0x0;

      if( m_acqMode == caen::Digitizer::ACQ_SIn_Controlled ) {
         result |= 0x1;
      }
      if( m_evCountMode == EV_CountAllTriggers ) {
         result |= 0x8;
      }
      if( m_clockSource == CLK_External ) {
         result |= 0x40;
      }

      return result;
   }

   bool Readout::cleanup() {

      if( m_buffer && m_bufferSize ) {
         // Free up the allocated readout buffer:
         CHECK( m_digitizer.freeReadoutBuffer( &m_buffer ) );
         m_buffer = NULL;
         m_bufferSize = 0;
      }

      return true;
   }

} // namespace dt5740
