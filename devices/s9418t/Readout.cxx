// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/vme/VmeBus.h"
#   include "cdacore/event/Fragment.h"
#else
#   include "vme/VmeBus.h"
#   include "event/Fragment.h"
#endif

// Local include(s):
#include "Readout.h"

namespace s9418t {

   Readout::Readout()
      : m_logger( "s9418t::Readout" ){

   }

   bool Readout::start( vme::VmeBus& bus ) const {

      if( m_calib == CALIB_DISABLED_VALUE ) {
         if( m_opMode == CommonStart ) {
            // Start the data acquisition in common start mode:
            bus.write( m_address + CONTROL_REGISTER_ADDR,
                       START_ACQ_VALUE );
         } else if( m_opMode == CommonStop ) {
            // Start the data acquisition in common stop mode:
            bus.write( m_address + CONTROL_REGISTER_ADDR,
                       ( short )( START_ACQ_VALUE + COMMON_STOP_MASK ) );
         } else {
            REPORT_ERROR( tr( "Operating mode not recognized" ) );
            return false;
         }
      } else {
         // Start calibration:
         bus.write( m_address + AUTOCALIB_REGISTER_ADDR,
                    m_calib );
      }

      return true;
   }

   bool Readout::stop( vme::VmeBus& bus ) const {

      // Reset/stop the device:
      bus.write( m_address + CONTROL_REGISTER_ADDR,
                 STOP_ACQ_VALUE );

      return true;
   }

   bool Readout::initialize( vme::VmeBus& bus ) const {

      // Reset/stop the device:
      bus.write( m_address + CONTROL_REGISTER_ADDR,
                 STOP_ACQ_VALUE );

      // Set up the VSN of the device:
      short data = ( ( m_vsn & 0xff00 ) >> 8 ) & 0x00ff;
      bus.write( m_address + VSN_H_REGISTER_ADDR, data );
      data = m_vsn & 0x00ff;
      bus.write( m_address + VSN_L_REGISTER_ADDR, data );

      // Do not generate a bus error on read:
      bus.write( m_address + STATUS_REGISTER_ADDR, ( short ) 1 );

      // Set up the used offset:
      bus.write( m_address + OFFSET_REGISTER_ADDR, m_offset );

      // Set up the used range:
      bus.write( m_address + RANGE_REGISTER_ADDR, m_range );

      // Initialize the channels:
      for( int i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
         if( m_channels[ i ] ) {
            bus.write( m_address + i * 0x2,
                       m_channels[ i ]->getThreshold() );
         } else {
            bus.write( m_address + i * 0x2, ( short ) 255 );
         }
      }

      return true;
   }

   std::unique_ptr< ev::Fragment >
   Readout::readEvent( vme::VmeBus& bus ) const {

      // Create the event fragment object:
      std::unique_ptr< ev::Fragment > fragment( new ev::Fragment() );
      fragment->setModuleID( m_address );

      // Temporary data used in the readout:
      int tempData;

      // Read out one event from the device FIFO:
      for( ; ; ) {

         // Read one 32-bit number from the device's FIFO:
         if( bus.read( m_address + FIFO_ADDR, &tempData ) ) {
            m_logger << msg::ERROR
                     << tr( "Failed reading from the event FIFO" )
                     << msg::endmsg;
            break;
         }

         // If the FIFO is empty:
         if( tempData == FIFO_EMPTY_VALUE ) {
            bus.wait();
            continue;
         }
         // If we reached the end of the event:
         else if( tempData & FIFO_EEVENT_MASK ) {
            break;
         }
         // If we see the footer of the event:
         else if( tempData & FIFO_FOOTER_MASK ) {
            continue;
         }

         // Extract the data from the word:
         fragment->addDataWord( ( ( tempData & FIFO_DWORD_MASK ) >> 4 ) |
                                ( tempData & FIFO_DATA_MASK ) );
      }

      // Return the data fragment:
      return fragment;
   }

} // namespace s9418t
