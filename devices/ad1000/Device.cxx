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

namespace ad1000 {

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
      : m_slot( -1 ), m_generateLam( false ), m_channel(),
        m_logger( "ad1000::Device" ) {

   }

   bool Device::readConfig( QIODevice& dev ) {

      REPORT_VERBOSE( tr( "Reading configuration from binary input" ) );

      clear();

      // Read the properties of this class:
      QDataStream input( &dev );
      input.setVersion( QDataStream::Qt_4_0 );
      input >> m_slot;
      input >> m_generateLam;

      REPORT_VERBOSE( tr( " - Slot        : %1\n"
                          " - GenerateLAM : %2" )
                      .arg( m_slot ).arg( m_generateLam ) );

      // Now read the properties of the one channel:
      CHECK( m_channel.readConfig( dev ) );

      return true;
   }

   bool Device::writeConfig( QIODevice& dev ) const {

      REPORT_VERBOSE( tr( "Writing configuration to binary output" ) );

      // Write the properties of this class:
      QDataStream output( &dev );
      output.setVersion( QDataStream::Qt_4_0 );
      output << m_slot;
      output << m_generateLam;

      // Write the properties of the channel:
      CHECK( m_channel.writeConfig( dev ) );

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

      REPORT_VERBOSE( tr( " - Slot        : %1\n"
                          " - GenerateLAM : %2" )
                      .arg( m_slot ).arg( m_generateLam ) );

      bool channelFound = false;
      for( int i = 0; i < element.childNodes().size(); ++i ) {

         // Only process "Channel" type child-nodes:
         if( element.childNodes().at( i ).nodeName() != "Channel" ) {
            continue;
         }

         // Sanity check:
         if( channelFound ) {
            m_logger << msg::WARNING
                     << tr( "Multiple channel configurations detected. "
                            "Will use the first one!" )
                     << msg::endmsg;
            continue;
         }

         // Another sanity check:
         CHECK( element.childNodes().at( i ).isElement() );

         // Now read the configuration of the channel:
         CHECK( m_channel.readConfig(
                   element.childNodes().at( i ).toElement() ) );
         // Note that we managed to read the channel configuration:
         channelFound = true;
      }

      // Check whether the channel's configuration was found:
      CHECK( channelFound );

      return true;
   }

   bool Device::writeConfig( QDomElement& element ) const {

      REPORT_VERBOSE( tr( "Writing configuration to XML output" ) );

      // Write the properties of the device class:
      element.setAttribute( "Slot", m_slot );
      element.setAttribute( "GenerateLAM", m_generateLam );

      // Create a new node for the configuration of the channel:
      QDomElement ch_element =
         element.ownerDocument().createElement( "Channel" );
      CHECK( m_channel.writeConfig( ch_element ) );
      element.appendChild( ch_element );

      return true;
   }

   const QString& Device::deviceName() const {

      static const QString name( "AD1000" );
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
      m_channel.clear();

      return;
   }

} // namespace ad1000
