// $Id$

// Qt include(s):
#include <QtCore/QIODevice>
#include <QtCore/QDataStream>
#include <QtXml/QDomElement>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/errorcheck.h"
#else
#   include "common/errorcheck.h"
#endif

// Local include(s):
#include "Device.h"

namespace ad2249a {

   //
   // Make sure that the following Qt classes are available in the
   // current namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QIODevice );
   using QT_PREPEND_NAMESPACE( QDataStream );
   using QT_PREPEND_NAMESPACE( QDomNode );
   using QT_PREPEND_NAMESPACE( QDomElement );

   Device::Device()
      : m_slot( -1 ), m_generateLam( false ),
        m_logger( "ad2249a::Device" ) {

   }

   bool Device::readConfig( QIODevice& dev ) {

      REPORT_VERBOSE( tr( "Reading configuration from binary input" ) );

      clear();

      QDataStream input( &dev );
      input.setVersion( QDataStream::Qt_4_0 );
      input >> m_slot;
      input >> m_generateLam;
      quint32 number_of_channels;
      input >> number_of_channels;

      REPORT_VERBOSE( tr( " - Slot        : %1\n"
                          " - GenerateLAM : %2\n"
                          " - Subaddresses: %3" )
                      .arg( m_slot ).arg( m_generateLam )
                      .arg( number_of_channels ) );

      for( quint32 i = 0; i < number_of_channels; ++i ) {
         UniquePtr< ChannelConfig >::Type channel( new ChannelConfig() );
         if( ! channel->readConfig( dev ) ) {
            REPORT_ERROR( tr( "The configuration of a channel couldn't be "
                              "read!" ) );
            return false;
         }
         if( ( channel->getSubaddress() >= 0 ) &&
             ( channel->getSubaddress() < NUMBER_OF_SUBADDRESSES ) ) {
            if( m_channels[ channel->getSubaddress() ] ) {
               m_logger << msg::WARNING
                        << tr( "Redefining channel number: %1" )
                  .arg( channel->getSubaddress() )
                        << msg::endmsg;
            }
            UniquePtr< ChannelConfig >::swap(
                     m_channels[ channel->getSubaddress() ], channel );
         } else {
            REPORT_ERROR( tr( "There was a problem reading the configuration "
                              "of one channel" ) );
            return false;
         }
      }

      return true;
   }

   bool Device::writeConfig( QIODevice& dev ) const {

      REPORT_VERBOSE( tr( "Writing configuration to binary output" ) );

      QDataStream output( &dev );
      output.setVersion( QDataStream::Qt_4_0 );
      output << m_slot;
      output << m_generateLam;

      // Count the number of configured channels:
      quint32 number_of_channels = 0;
      for( int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i ) {
         if( m_channels[ i ] ) ++number_of_channels;
      }

      // Write the number of channels to follow:
      output << number_of_channels;

      // Write the channel configurations:
      for( int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i ) {
         if( m_channels[ i ] ) {
            CHECK( m_channels[ i ]->writeConfig( dev ) );
         }
      }

      return true;
   }

   bool Device::readConfig( const QDomElement& element ) {

      REPORT_VERBOSE( tr( "Reading configuration from XML input" ) );

      clear();

      bool ok;

      m_slot = element.attribute( "Slot", "0" ).toInt( &ok );
      CHECK( ok );

      m_generateLam = element.attribute( "GenerateLAM", "0" ).toShort( &ok );
      CHECK( ok );

      REPORT_VERBOSE( tr( " - Slot       : %1\n"
                          " - GenerateLAM: %2" )
                      .arg( m_slot ).arg( m_generateLam ) );

      for( int i = 0; i < element.childNodes().size(); ++i ) {

         // Only process "Channel" type child-nodes:
         if( element.childNodes().at( i ).nodeName() != "Channel" ) {
            continue;
         }

         // Read in this channel's configuration:
         UniquePtr< ChannelConfig >::Type channel( new ChannelConfig() );
         if( ! channel->readConfig(
                element.childNodes().at( i ).toElement() ) ) {
            REPORT_ERROR( tr( "The configuration of a channel couldn't be "
                              "read!" ) );
            return false;
         }

         // Store this object:
         if( ( channel->getSubaddress() >= 0 ) &&
             ( channel->getSubaddress() < NUMBER_OF_SUBADDRESSES ) ) {
            if( m_channels[ channel->getSubaddress() ] ) {
               m_logger << msg::WARNING
                        << tr( "Redefining channel number: %1" )
                  .arg( channel->getSubaddress() )
                        << msg::endmsg;
            }
            UniquePtr< ChannelConfig >::swap(
                     m_channels[ channel->getSubaddress() ], channel );
         } else {
            REPORT_ERROR( tr( "There was a problem reading the configuration "
                              "of one channel" ) );
            return false;
         }
      }

      return true;
   }

   bool Device::writeConfig( QDomElement& element ) const {

      REPORT_VERBOSE( tr( "Writing configuration to XML output" ) );

      element.setAttribute( "Slot", m_slot );
      element.setAttribute( "GenerateLAM", m_generateLam );

      //
      // Create a new node for the configuration of each channel:
      //
      for( int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i ) {
         if( m_channels[ i ] ) {
            QDomElement ch_element =
               element.ownerDocument().createElement( "Channel" );
            CHECK( m_channels[ i ]->writeConfig( ch_element ) );
            element.appendChild( ch_element );
         }
      }

      return true;
   }

   const QString& Device::deviceName() const {

      static const QString name( "AD2249A" );
      return name;
   }

   unsigned int Device::getID() const {

      return m_slot;
   }

   void Device::setID( unsigned int value ) {

      m_slot = value;
      return;
   }

   void Device::clear() {

      m_slot = 0;
      m_generateLam = false;
      for( int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i ) {
         m_channels[ i ].reset();
      }

      return;
   }

} // namespace ad2249a
