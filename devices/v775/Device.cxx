// $Id$

// Qt include(s):
#include <QtCore/QString>
#include <QtCore/QDataStream>
#include <QtXml/QDomElement>

// CDA include(s):
#include "common/errorcheck.h"

// Local include(s):
#include "Device.h"

namespace v775 {

   Device::Device()
      : m_vmeAddress( 0 ), m_zeroSuppressionEnabled( false ),
        m_overflowSuppressionEnabled( false ),
        m_validSuppressionEnabled( false ),
        m_commonStopEnabled( false ),
        m_fullScaleRangeValue( 0xff ),
        m_channels(),
        m_logger( "v775::Device" ) {

   }

   StatusCode Device::readConfig( QIODevice& dev ) {

      // Tell the user what's happening:
      REPORT_VERBOSE( tr( "Reading configuration from binary input" ) );

      // Clear the current configuration:
      clear();

      // Read the device level configuration:
      QDataStream input( &dev );
      input.setVersion( QDataStream::Qt_4_0 );
      input >> m_vmeAddress;
      input >> m_zeroSuppressionEnabled;
      input >> m_overflowSuppressionEnabled;
      input >> m_validSuppressionEnabled;
      input >> m_commonStopEnabled;
      input >> m_fullScaleRangeValue;
      quint32 number_of_channels;
      input >> number_of_channels;

      // Print the device level configuration:
      REPORT_VERBOSE( tr( " - VME Address         : %1\n"
                          " - Zero suppression    : %2\n"
                          " - Overflow suppression: %3\n"
                          " - Valid suppression   : %4\n"
                          " - Common stop         : %5\n"
                          " - Full Scale Range    : %6\n"
                          " - Channels            : %7" )
                      .arg( m_vmeAddress, 4, 16 )
                      .arg( m_zeroSuppressionEnabled )
                      .arg( m_overflowSuppressionEnabled )
                      .arg( m_validSuppressionEnabled )
                      .arg( m_commonStopEnabled )
                      .arg( m_fullScaleRangeValue, 2, 16 )
                      .arg( number_of_channels ) );

      // Read the configuration of all enabled channels:
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

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   StatusCode Device::writeConfig( QIODevice& dev ) const {

      // Tell the user what's happening:
      REPORT_VERBOSE( tr( "Writing configuration to binary output" ) );

      // Write the device level configuration:
      QDataStream output( &dev );
      output.setVersion( QDataStream::Qt_4_0 );
      output << m_vmeAddress;
      output << m_zeroSuppressionEnabled;
      output << m_overflowSuppressionEnabled;
      output << m_validSuppressionEnabled;
      output << m_commonStopEnabled;
      output << m_fullScaleRangeValue;

      // Count the number of configured channels:
      quint32 number_of_channels = 0;
      for( int i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
         if( m_channels[ i ] ) ++number_of_channels;
      }

      // Write the number of channels to follow:
      output << number_of_channels;

      // Write the channel configurations:
      for( int i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
         if( m_channels[ i ] ) {
            CHECK( m_channels[ i ]->writeConfig( dev ) );
         }
      }

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   StatusCode Device::readConfig( const QDomElement& element ) {

      // Tell the user what's happening:
      REPORT_VERBOSE( tr( "Reading configuration from XML input" ) );

      // Clear the current cofiguration:
      clear();

      // A helper variable to check for string parsing errors:
      bool ok = true;

      // Read the VME address:
      m_vmeAddress = element.attribute( "VMEAddress", "0" ).toInt( &ok, 16 );
      CHECK( ok );

      // Read the zero suppression flag:
      m_zeroSuppressionEnabled = element.attribute( "ZeroSuppression",
                                                    "0" ).toInt( &ok );
      CHECK( ok );

      // Read the overflow suppression flag:
      m_overflowSuppressionEnabled = element.attribute( "OverflowSuppression",
                                                        "0" ).toInt( &ok );
      CHECK( ok );

      // Read the zero suppression flag:
      m_validSuppressionEnabled = element.attribute( "ValidSuppression",
                                                     "0" ).toInt( &ok );
      CHECK( ok );

      // Read the common stop flag:
      m_commonStopEnabled = element.attribute( "CommonStop",
                                               "0" ).toInt( &ok );
      CHECK( ok );

      // Read the Full Scale Range register value:
      m_fullScaleRangeValue = element.attribute( "FullScaleRange",
                                                 "255" ).toInt( &ok );
      CHECK( ok );

      // Print the device level configuration:
      REPORT_VERBOSE( tr( " - VME Address: %1\n"
                          " - Zero suppression    : %2\n"
                          " - Overflow suppression: %3\n"
                          " - Valid suppression   : %4\n"
                          " - Common stop         : %5\n"
                          " - Full Scale Range    : %6" )
                      .arg( m_vmeAddress, 4, 16 )
                      .arg( m_zeroSuppressionEnabled )
                      .arg( m_overflowSuppressionEnabled )
                      .arg( m_validSuppressionEnabled )
                      .arg( m_commonStopEnabled )
                      .arg( m_fullScaleRangeValue, 2, 16 ) );

      // Investigate all child nodes of this element:
      for( int i = 0; i < element.childNodes().size(); ++i ) {

         // Only process "Channel" type child-nodes:
         if( element.childNodes().at( i ).nodeName() != "Channel" ) {
            continue;
         }

         // Construct a channel object with this configuration:
         std::unique_ptr< ChannelConfig > channel( new ChannelConfig() );
         const auto& chElement = element.childNodes().at( i ).toElement();
         CHECK( channel->readConfig( chElement ) );

         // Make sure that the reading was successful:
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

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   StatusCode Device::writeConfig( QDomElement& element ) const {

      // Tell the user what's happening:
      REPORT_VERBOSE( tr( "Writing configuration to XML output" ) );

      // Set the device level properties:
      element.setAttribute( "VMEAddress",
                            QString( "%1" ).arg( m_vmeAddress, 4, 16,
                                                 QChar( '0' ) ) );
      element.setAttribute( "ZeroSuppression", m_zeroSuppressionEnabled );
      element.setAttribute( "OverflowSuppression",
                            m_overflowSuppressionEnabled );
      element.setAttribute( "ValidSuppression", m_validSuppressionEnabled );
      element.setAttribute( "CommonStop", m_commonStopEnabled );
      element.setAttribute( "FullScaleRange", m_fullScaleRangeValue );

      // Create a new node for the configuration of each channel:
      for( int i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
         if( m_channels[ i ] ) {
            QDomElement ch_element =
               element.ownerDocument().createElement( "Channel" );
            CHECK( m_channels[ i ]->writeConfig( ch_element ) );
            element.appendChild( ch_element );
         }
      }

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   const QString& Device::deviceName() const {

      static const QString name( "V775" );
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

      // Set the device level setting(s) to default values:
      m_vmeAddress = 0;
      m_zeroSuppressionEnabled = false;
      m_overflowSuppressionEnabled = false;
      m_validSuppressionEnabled = false;
      m_commonStopEnabled = false;
      m_fullScaleRangeValue = 0xff;

      // Remove all the channels:
      for( auto& ch : m_channels ) {
         ch.reset();
      }

      // Return gracefully:
      return;
   }

   Device::ChannelData::ChannelData( const caen::VmeDevice::DataWord& dw )
      : m_data( 0 ) {

      // Construct the internal word:
      m_data = ( ( dw.data & 0xffff ) |
                 ( ( dw.channel << 16 ) & 0xff0000 ) |
                 ( dw.underThreshold ? 0x1000000 : 0 ) |
                 ( dw.overflow ? 0x2000000 : 0 ) );
   }

   Device::ChannelData::ChannelData( ev::Fragment::Payload_t::value_type data )
      : m_data( data ) {

   }

   int Device::ChannelData::channel() const {

      return ( ( m_data >> 16 ) & 0xff );
   }

   int Device::ChannelData::data() const {

      return ( m_data & 0xffff );
   }

   bool Device::ChannelData::underThreshold() const {

      return ( m_data & 0x1000000 );
   }

   bool Device::ChannelData::overflow() const {

      return ( m_data & 0x2000000 );
   }

} // namespace v775
