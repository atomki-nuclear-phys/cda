// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/event/Fragment.h"
#   include "cdacore/common/errorcheck.h"
#else
#   include "event/Fragment.h"
#   include "common/errorcheck.h"
#endif

// Local include(s):
#include "Readout.h"

namespace dt5740 {

   Readout::Readout()
      : dev::CaenReadout(), Device(), m_digitizer(),
        m_logger( "dt5740::Readout" ) {

   }

   Readout::~Readout() {

      finalize();
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
      CHECK( dummy == 0x383838 );

      // Print some information about the board for debugging:
      CHECK( m_digitizer.printInfo( msg::DEBUG ) );

      // Set up the triggering:
      CHECK( m_digitizer.setSWTriggerMode( caen::Digitizer::TRIG_Disabled ) );
      CHECK( m_digitizer.setExtTriggerMode( extTrigMode() ) );
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {
         CHECK( m_digitizer.setGroupSelfTriggerMode( i,
                                                     trigMode( m_groups[ i ] ) ) );
      }

      // Configure the device wide parameters:
      CHECK( m_digitizer.setRecordLength( getSamples() ) );
      CHECK( m_digitizer.setPostTriggerSize( m_postTrigPercentage ) );
      CHECK( m_digitizer.writeRegister( REG_GROUP_CONFIG,
                                        groupConfReg() ) );

      // Configure the group wide parameters:
      uint32_t groupMask = 0;
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {
         CHECK( m_digitizer.setGroupTriggerThreshold( i, m_groups[ i ].getTrigThreshold() ) );
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

      // Configure the channels:
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {
         for( int j = 0; j < GroupConfig::CHANNELS_IN_GROUP; ++j ) {
            
         }
      }

      return true;
   }

   bool Readout::finalize() {

      // Clear and close the digitizer:
      CHECK( m_digitizer.reset() );
      CHECK( m_digitizer.close() );

      return true;
   }

   bool Readout::start() {

      return true;
   }

   bool Readout::stop() {

      return true;
   }

   ev::Fragment* Readout::readEvent() const {

      ev::Fragment* result = new ev::Fragment();

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

} // namespace dt5740
