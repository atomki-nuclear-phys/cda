
// Local include(s).
#include "Device.h"

// CDA include(s).
#include "common/errorcheck.h"

// Qt include(s).
#include <QtCore/QString>
#include <QtCore/QDataStream>
#include <QtXml/QDomElement>

namespace v812 {

   Device::Device()
   : m_vmeAddress( 0x0 ), m_outputWidth1( 0 ), m_outputWidth2( 0 ),
     m_deadTime1( 0 ), m_deadTime2( 0 ), m_majorityThreshold( 1 ),
     m_channels(), m_logger( "v812::Device" ) {

   }

   StatusCode Device::readConfig( QIODevice& dev ) {

      // Tell the user what's happening.
      REPORT_VERBOSE( tr( "Reading configuration from binary input" ) );

      // Clear the current configuration.
      clear();

      // Read the device level configuration.
      QDataStream input( &dev );
      input.setVersion( QDataStream::Qt_4_0 );
      input >> m_vmeAddress;
      input >> m_outputWidth1;
      input >> m_outputWidth2;
      input >> m_deadTime1;
      input >> m_deadTime2;
      input >> m_majorityThreshold;
      quint32 number_of_channels;
      input >> number_of_channels;

      // Print the device level configuration.
      REPORT_VERBOSE( tr( " - VME Address        : %1\n"
                          " - Output width 0-7   : %2\n"
                          " - Output width 8-15  : %3\n"
                          " - Dead time 0-7      : %4\n"
                          " - Dead time 8-15     : %5\n"
                          " - Majority threshold : %6\n"
                          " - Channels           : %7" )
                      .arg( m_vmeAddress, 4, 16 )
                      .arg( m_outputWidth1 ).arg( m_outputWidth2 )
                      .arg( m_deadTime1 ).arg( m_deadTime2 )
                      .arg( m_majorityThreshold )
                      .arg( number_of_channels ) );

      // Read the configuration of all enabled channels.
      for( quint32 i = 0; i < number_of_channels; ++i ) {
         std::unique_ptr< ChannelConfig > channel( new ChannelConfig() );
         CHECK( channel->readConfig( dev ) );
         if( ( channel->getChannel() >= 0 ) &&
             ( channel->getChannel() < NUMBER_OF_CHANNELS ) ) {
            if( m_channels[ channel->getChannel() ] ) {
               m_logger << msg::WARNING
                        << tr( "Redefining channel number: %1" )
                           .arg( channel->getChannel() )
                        << msg::endmsg;
            }
            m_channels[ channel->getChannel() ].swap( channel );
         } else {
            REPORT_ERROR( tr( "There was a problem reading the configuration "
                              "of one channel" ) );
            return StatusCode::FAILURE;
         }
      }

      // Return gracefully.
      return StatusCode::SUCCESS;
   }

   StatusCode Device::writeConfig( QIODevice& dev ) const {

      // Tell the user what's happening.
      REPORT_VERBOSE( tr( "Writing configuration to binary output" ) );

      // Write the device level configuration.
      QDataStream output( &dev );
      output.setVersion( QDataStream::Qt_4_0 );
      output << m_vmeAddress;
      output << m_outputWidth1;
      output << m_outputWidth2;
      output << m_deadTime1;
      output << m_deadTime2;
      output << m_majorityThreshold;

      // Count the number of configured channels.
      quint32 number_of_channels = 0;
      for( int i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
         if( m_channels[ i ] ) ++number_of_channels;
      }

      // Write the number of channels to follow.
      output << number_of_channels;

      // Write the channel configurations.
      for( int i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
         if( m_channels[ i ] ) {
            CHECK( m_channels[ i ]->writeConfig( dev ) );
         }
      }

      // Return gracefully.
      return StatusCode::SUCCESS;
   }

   StatusCode Device::readConfig( const QDomElement& element ) {

      // Tell the user what's happening.
      REPORT_VERBOSE( tr( "Reading configuration from XML input" ) );

      // Clear the current cofiguration.
      clear();

      // Read the device level configuration.
      bool ok = true;
      m_vmeAddress = element.attribute( "VMEAddress", "" ).toInt( &ok, 16 );
      CHECK( ok );
      m_outputWidth1 =
         static_cast<uint8_t>(element.attribute( "OutputWidth_0_7", "0" ).toUShort( &ok ));
      CHECK( ok );
      m_outputWidth2 =
         static_cast<uint8_t>(element.attribute( "OutputWidth_8_15", "0" ).toUShort( &ok ));
      CHECK( ok );
      m_deadTime1 =
         static_cast<uint8_t>(element.attribute( "DeadTime_0_7", "0" ).toUShort( &ok ));
      CHECK( ok );
      m_deadTime2 =
         static_cast<uint8_t>(element.attribute( "DeadTime_8_15", "0" ).toUShort( &ok ));
      CHECK( ok );
      m_majorityThreshold =
         element.attribute( "MajorityThreshold", "1" ).toInt( &ok );
      CHECK( ok );

      // Print the device level configuration.
      REPORT_VERBOSE( tr( " - VME Address        : %1\n"
                          " - Output width 0-7   : %2\n"
                          " - Output width 8-15  : %3\n"
                          " - Dead time 0-7      : %4\n"
                          " - Dead time 8-15     : %5\n"
                          " - Majority threshold : %6" )
                      .arg( m_vmeAddress, 4, 16 )
                      .arg( m_outputWidth1 ).arg( m_outputWidth2 )
                      .arg( m_deadTime1 ).arg( m_deadTime2 )
                      .arg( m_majorityThreshold ) );

      // Investigate all child nodes of this element.
      for( int i = 0; i < element.childNodes().size(); ++i ) {

         // Only process "Channel" type child-nodes.
         if( element.childNodes().at( i ).nodeName() != "Channel" ) {
            continue;
         }

         // Construct a channel object with this configuration.
         std::unique_ptr< ChannelConfig > channel( new ChannelConfig() );
         const auto& chElement = element.childNodes().at( i ).toElement();
         CHECK( channel->readConfig( chElement ) );

         // Make sure that the reading was successful.
         if( ( channel->getChannel() >= 0 ) &&
             ( channel->getChannel() < NUMBER_OF_CHANNELS ) ) {
            if( m_channels[ channel->getChannel() ] ) {
               m_logger << msg::WARNING
                        << tr( "Redefining channel number: %1" )
                           .arg( channel->getChannel() )
                        << msg::endmsg;
            }
            m_channels[ channel->getChannel() ].swap( channel );
         } else {
            REPORT_ERROR( tr( "There was a problem reading the configuration "
                              "of one channel" ) );
            return StatusCode::FAILURE;
         }
      }

      // Return gracefully.
      return StatusCode::SUCCESS;
   }

   StatusCode Device::writeConfig( QDomElement& element ) const {

      // Tell the user what's happening.
      REPORT_VERBOSE( tr( "Writing configuration to XML output" ) );

      // Set the device level properties.
      element.setAttribute( "VMEAddress",
                            QString( "%1" ).arg( m_vmeAddress, 4, 16,
                                                 QChar( '0' ) ) );
      element.setAttribute( "OutputWidth_0_7",
                            static_cast<unsigned int>(m_outputWidth1) );
      element.setAttribute( "OutputWidth_8_15",
                            static_cast<unsigned int>(m_outputWidth2) );
      element.setAttribute( "DeadTime_0_7",
                            static_cast<unsigned int>(m_deadTime1) );
      element.setAttribute( "DeadTime_8_15",
                            static_cast<unsigned int>(m_deadTime2) );
      element.setAttribute( "MajorityThreshold", m_majorityThreshold );

      // Create a new node for the configuration of each channel.
      for( int i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
         if( m_channels[ i ] ) {
            QDomElement ch_element =
               element.ownerDocument().createElement( "Channel" );
            CHECK( m_channels[ i ]->writeConfig( ch_element ) );
            element.appendChild( ch_element );
         }
      }

      // Return gracefully.
      return StatusCode::SUCCESS;
   }

   const QString& Device::deviceName() const {

      static const QString name( "V812" );
      return name;
   }

   unsigned int Device::getID() const {

      return m_vmeAddress;
   }

   void Device::setID( unsigned int value ) {

      m_vmeAddress = value;
      return;
   }

   void Device::clear() {

      // Set the device level setting(s) to default values.
      m_vmeAddress = 0;
      m_outputWidth1 = 0;
      m_outputWidth2 = 0;
      m_deadTime1 = 0;
      m_deadTime2 = 0;
      m_majorityThreshold = 1;

      // Remove all the channels.
      for( auto& ch : m_channels ) {
         ch.reset();
      }

      // Return gracefully.
      return;
   }

} // namespace v812
