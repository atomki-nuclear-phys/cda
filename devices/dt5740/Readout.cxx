// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/event/Fragment.h"
#   include "cdacore/common/errorcheck.h"
#   include "cdacore/common/Sleep.h"
#   include "cdacore/caen/StopAcquisition.h"
#else
#   include "event/Fragment.h"
#   include "common/errorcheck.h"
#   include "common/Sleep.h"
#   include "caen/StopAcquisition.h"
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

      // Print some information about the board for debugging:
      CHECK( m_digitizer.printInfo( msg::DEBUG ) );

      // Some basic settings:
      REPORT_VERBOSE( tr( "Record length will be: %1" )
                      .arg( getSamples() ) );
      CHECK( m_digitizer.setRecordLength( getSamples() ) );
      REPORT_VERBOSE( tr( "Group enable mask will be: %1" )
                      .arg( groupMask() ) );
      CHECK( m_digitizer.setGroupEnableMask( groupMask() ) );

      // Configure each channel group:
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {
         REPORT_VERBOSE( tr( "Setting trigger threshold for group %1 to %2" )
                         .arg( i ).arg( m_groups[ i ].getTrigThreshold() ) );
         CHECK( m_digitizer.setGroupTriggerThreshold( i,
                                               m_groups[ i ].getTrigThreshold() ) );
         REPORT_VERBOSE( tr( "Setting self trigger mode for group %1 to %2" )
                         .arg( i ).arg( trigMode( m_groups[ i ] ) ) );
         CHECK( m_digitizer.setGroupSelfTriggerMode( i,
                                                     trigMode( m_groups[ i ] ) ) );
         REPORT_VERBOSE( tr( "Setting channel mask of group %1 to %2" )
                         .arg( i ).arg( m_groups[ i ].getTrigMask() ) );
         CHECK( m_digitizer.setChannelGroupMask( i, m_groups[ i ].getTrigMask() ) );
         REPORT_VERBOSE( tr( "Setting DC offset of group %1 to %2" )
                         .arg( i ).arg( m_groups[ i ].getDCOffset() ) );
         CHECK( m_digitizer.setGroupDCOffset( i, m_groups[ i ].getDCOffset() ) );
      }

      // Set the device wide trigger parameters:
      CHECK( m_digitizer.setSWTriggerMode( caen::Digitizer::TRIG_AcqOnly ) );
      REPORT_VERBOSE( tr( "Setting external trigger mode to %1" )
                      .arg( extTrigMode() ) );
      CHECK( m_digitizer.setExtTriggerMode( extTrigMode() ) );
      CHECK( m_digitizer.setPostTriggerSize( m_postTrigPercentage ) );

      CHECK( m_digitizer.setAcquisitionMode( m_acqMode ) );
      CHECK( m_digitizer.writeRegister( REG_GROUP_CONFIG,
                                        groupConfReg() ) );
      CHECK( m_digitizer.writeRegister( REG_ACQ_CONTROL,
                                        acqControlReg() ) );
      CHECK( m_digitizer.writeRegister( REG_IO_CONTROL,
                                        ioControlReg() ) );

      // Set the maximum number of events to read at once:
      CHECK( m_digitizer.setMaxNumEventsBLT( 50 ) );

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
      REPORT_VERBOSE( tr( "Memory cleaned up" ) );

      // Clear and close the digitizer:
      CHECK( m_digitizer.close() );
      REPORT_VERBOSE( tr( "Digitizer closed" ) );

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
      REPORT_VERBOSE( tr( "Acquisition stopped" ) );

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
#ifndef HAVE_CAEN_LIBS
            // When in testing mode, let's not wait around here...
            m_numEvents = 1;
#endif // HAVE_CAEN_LIBS
            // If there are events available, exit the loop:
            if( m_numEvents ) {
               break;
            }
            // If the acquisition is to be stopped, return an empty
            // fragment:
            if( g_stopAcquisition ) {
               REPORT_VERBOSE( tr( "The acquisition is to be stopped. Returning "
                                   "empty event fragment." ) );
               return result;
            }
            // If not, then wait a bit and then try again:
            common::SleepMin();
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

   uint32_t Readout::groupMask() const {

      uint32_t result = 0;

      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {
         // Check if any channels are active in this group:
         bool groupActive = false;
         for( int j = 0; j < GroupConfig::CHANNELS_IN_GROUP; ++j ) {
            if( m_groups[ i ].getChannel( j ) ) {
               groupActive = true;
               break;
            }
         }
         if( groupActive ) {
            result |= ( 0x1 << i );
         }
      }

      return result;
   }

   uint32_t Readout::groupConfReg() const {

      uint32_t result = 0x10;

      if( m_gateMode == GATE_SingleShot ) {
         result |= 0x1;
      }
      if( m_trigOvlpEnabled ) {
         result |= 0x2;
      }
      if( m_patGenEnabled ) {
         result |= 0x8;
      }
      if( m_trigMode == TRG_InputUnderThreshold ) {
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

   uint32_t Readout::ioControlReg() const {

      uint32_t result = 0;

      if( m_signalType == SGNL_TTL ) {
         result |= 0x1;
      }
      if( m_highImpedanceGPO ) {
         result |= 0x2;
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
