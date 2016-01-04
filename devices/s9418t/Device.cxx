// $Id$

// Qt include(s):
#include <QtCore/QIODevice>
#include <QtCore/QDataStream>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/errorcheck.h"
#else
#   include "common/errorcheck.h"
#endif

// Local include(s):
#include "Device.h"

namespace s9418t {

   Device::Device()
      : m_address( 0 ), m_vsn( 0 ), m_calib( 0 ), m_range( 0 ),
        m_offset( 0 ), m_opMode( CommonStart ),
        m_logger( "s9418t::Device" ) {

   }

   bool Device::readConfig( QIODevice& dev ) {

      m_logger << msg::VERBOSE
               << tr( "Reading the configuration from binary input" )
               << msg::endmsg;

      clear();

      QDataStream input( &dev );
      input.setVersion( QDataStream::Qt_4_0 );

      input >> m_address;
      input >> m_vsn;
      input >> m_calib;
      input >> m_range;
      input >> m_offset;

      short opmode;
      input >> opmode;
      if( opmode == 0 ) {
         m_opMode = CommonStart;
      } else if( opmode == 1 ) {
         m_opMode = CommonStop;
      } else {
         REPORT_ERROR( tr( "Operating mode not recognised" ) );
         return false;
      }

      quint32 number_of_channels;
      input >> number_of_channels;

      REPORT_VERBOSE( tr( " - Address        : %1\n"
                          " - VSN            : %2\n"
                          " - Calib amplitude: %3\n"
                          " - Meas. range    : %4\n"
                          " - Meas. offset   : %5\n"
                          " - Operation mode : %6" )
                      .arg( m_address, 8, 16 ).arg( m_vsn ).arg( m_calib )
                      .arg( m_range ).arg( m_offset )
                      .arg( m_opMode == CommonStart ?
                               "common start" : "common stop" ) );

      for( quint32 i = 0; i < number_of_channels; ++i ) {
         std::unique_ptr< ChannelConfig > channel( new ChannelConfig() );
         CHECK( channel->readConfig( dev ) );
         if( ( channel->getChannel() >= 0 ) &&
             ( channel->getChannel() < NUMBER_OF_CHANNELS ) ) {
            if( m_channels[ channel->getChannel() ] ) {
               m_logger << msg::WARNING
                        << tr( "Re-defining channel number: %1" )
                  .arg( channel->getChannel() )
                        << msg::endmsg;
            }
            m_channels[ channel->getChannel() ].swap( channel );
         } else {
            REPORT_ERROR( tr( "The configuration of a channel couldn't "
                              "be read!" ) );
            return false;
         }
      }

      return true;
   }

   bool Device::writeConfig( QIODevice& dev ) const {

      m_logger << msg::VERBOSE
               << tr( "Writing the configuration to binary output" )
               << msg::endmsg;

      QDataStream output( &dev );
      output.setVersion( QDataStream::Qt_4_0 );

      output << m_address;
      output << m_vsn;
      output << m_calib;
      output << m_range;
      output << m_offset;

      short opmode = 0;
      if( m_opMode == CommonStart ) {
         opmode = 0;
      } else if( m_opMode == CommonStop ) {
         opmode = 1;
      } else {
         m_logger << msg::ERROR
                  << tr( "Operating mode not recognised" )
                  << msg::endmsg;
         return false;
      }
      output << opmode;

      // Count the number of configured channels:
      quint32 number_of_channels = 0;
      for( int i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
         if( m_channels[ i ] ) ++number_of_channels;
      }

      // Write out how many channels' configuration is about to be
      // written out:
      output << number_of_channels;

      // Write the configuration of the channels:
      for( int i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
         if( ! m_channels[ i ] ) continue;
         CHECK( m_channels[ i ]->writeConfig( dev ) );
      }

      return true;
   }

   bool Device::readConfig( const QDomElement& element ) {

      REPORT_VERBOSE( tr( "Reading the configuration from XML input" ) );

      clear();

      bool ok;

      m_address = element.attribute( "Address", "0" ).toUInt( &ok, 16 );
      CHECK( ok );

      m_vsn = element.attribute( "VSN", "0" ).toShort( &ok );
      CHECK( ok );

      m_calib = element.attribute( "Calib", "0" ).toShort( &ok );
      CHECK( ok );

      m_range = element.attribute( "Range", "0" ).toShort( &ok );
      CHECK( ok );

      m_offset = element.attribute( "Offset", "0" ).toShort( &ok );
      CHECK( ok );

      const QString opmode = element.attribute( "OpMode", "CommonStart" );
      if( opmode == "CommonStart" ) {
         m_opMode = CommonStart;
      } else if( opmode == "CommonStop" ) {
         m_opMode = CommonStop;
      } else {
         REPORT_ERROR( tr( "There was a problem reading the operating mode" ) );
         return false;
      }

      REPORT_VERBOSE( tr( " - Address        : %1\n"
                          " - VSN            : %2\n"
                          " - Calib amplitude: %3\n"
                          " - Meas. range    : %4\n"
                          " - Meas. offset   : %5\n"
                          " - Operation mode : %6" )
                      .arg( m_address, 8, 16 ).arg( m_vsn ).arg( m_calib )
                      .arg( m_range ).arg( m_offset )
                      .arg( m_opMode == CommonStart ?
                               "common start" : "common stop" ) );

      for( int i = 0; i < element.childNodes().size(); ++i ) {

         // Only process "Channel" type child nodes:
         if( element.childNodes().at( i ).nodeName() != "Channel" ) {
            continue;
         }

         // Read in the channel's configuration:
         std::unique_ptr< ChannelConfig > channel( new ChannelConfig() );
         CHECK( channel->readConfig(
                   element.childNodes().at( i ).toElement() ) );

         if( ( channel->getChannel() >= 0 ) &&
             ( channel->getChannel() < NUMBER_OF_CHANNELS ) ) {
            if( m_channels[ channel->getChannel() ] ) {
               m_logger << msg::WARNING
                        << tr( "Re-defining channel number: %1" )
                  .arg( channel->getChannel() )
                        << msg::endmsg;
            }
            m_channels[ channel->getChannel() ].swap( channel );
         } else {
            REPORT_ERROR( tr( "The configuration of a channel couldn't "
                              "be read!" ) );
            return false;
         }
      }

      return true;
   }

   bool Device::writeConfig( QDomElement& element ) const {

      REPORT_VERBOSE( tr( "Writing the configuration to XML output" ) );

      element.setAttribute( "Address", QString::number( m_address, 16 ) );
      element.setAttribute( "VSN", m_vsn );
      element.setAttribute( "Calib", m_calib );
      element.setAttribute( "Range", m_range );
      element.setAttribute( "Offset", m_offset );
      if( m_opMode == CommonStart ) {
         element.setAttribute( "OpMode", "CommonStart" );
      } else if( m_opMode == CommonStop ) {
         element.setAttribute( "OpMode", "CommonStop" );
      } else {
         REPORT_ERROR( tr( "Operating mode not recognised" ) );
         return false;
      }

      //
      // Create a new node for the configuration of each channel:
      //
      for( int i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
         if( ! m_channels[ i ] ) continue;

         QDomElement ch_element =
            element.ownerDocument().createElement( "Channel" );
         CHECK( m_channels[ i ]->writeConfig( ch_element ) );
         element.appendChild( ch_element );
      }

      return true;
   }

   const QString& Device::deviceName() const {

      static const QString name( "S9418T" );
      return name;
   }

   unsigned int Device::getID() const {

      return m_address;
   }

   void Device::setID( unsigned int value ) {

      m_address = value;
      return;
   }

   void Device::clear() {

      m_address = 0;
      m_vsn = 0;
      m_calib = 0;
      m_range = 0;
      m_offset = 0;
      m_opMode = CommonStart;
      for( int i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
         m_channels[ i ].reset();
      }

      return;
   }

} // namespace s9418t
