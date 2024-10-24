
// Local include(s).
#include "ChannelConfig.h"

// CDA include(s).
#include "common/errorcheck.h"

// Qt include(s).
#include <QtCore/QIODevice>
#include <QtCore/QDataStream>
#include <QtXml/QDomElement>

namespace v812 {

   //
   // Make sure that the following Qt classes are available in the
   // current namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QIODevice );
   using QT_PREPEND_NAMESPACE( QDataStream );
   using QT_PREPEND_NAMESPACE( QDomElement );

   ChannelConfig::ChannelConfig()
   : m_channel( -1 ), m_threshold( 0 ), m_logger( "v812::ChannelConfig" ) {

   }

   StatusCode ChannelConfig::readConfig( QIODevice& dev ) {

      // Tell the user what's happening.
      REPORT_VERBOSE( tr( "Reading configuration from binary input" ) );

      // Clear any previous configuration.
      clear();

      // Read the channel's configuration.
      QDataStream input( &dev );
      input.setVersion( QDataStream::Qt_4_0 );
      input >> m_channel;
      input >> m_threshold;

      // Print the configuration for debugging.
      printConfig( msg::VERBOSE );

      // Return gracefully.
      return StatusCode::SUCCESS;
   }

   StatusCode ChannelConfig::writeConfig( QIODevice& dev ) const {

      // Tell the user what's happening.
      REPORT_VERBOSE( tr( "Writing configuration to binary output" ) );

      // Write the channel's configuration.
      QDataStream output( &dev );
      output.setVersion( QDataStream::Qt_4_0 );
      output << m_channel;
      output << m_threshold;

      // Return gracefully.
      return StatusCode::SUCCESS;
   }

   StatusCode ChannelConfig::readConfig( const QDomElement& element ) {

      // Tell the user what's happening.
      REPORT_VERBOSE( tr( "Reading configuration from XML input" ) );

      // Clear any previous configuration.
      clear();

      // Read the channel's configuration.
      bool ok;
      m_channel = element.attribute( "ChannelNumber", "" ).toInt( &ok );
      CHECK( ok );
      m_threshold = element.attribute( "Threshold", "1" ).toInt( &ok );
      CHECK( ok );

      // Print the configuration for debugging.
      printConfig( msg::VERBOSE );

      // Return gracefully.
      return StatusCode::SUCCESS;
   }

   StatusCode ChannelConfig::writeConfig( QDomElement& element ) const {

      // Tell the user what's happening.
      REPORT_VERBOSE( tr( "Writing configuration to XML output" ) );

      // Write the channel's configuration.
      element.setAttribute( "ChannelNumber", m_channel );
      element.setAttribute( "Threshold", m_threshold );

      // Return gracefully.
      return StatusCode::SUCCESS;
   }

   int ChannelConfig::getChannel() const {

      return m_channel;
   }

   void ChannelConfig::setChannel( int value ) {

      m_channel = value;
      return;
   }

   uint16_t ChannelConfig::getThreshold() const {

      return m_threshold;
   }

   void ChannelConfig::setThreshold( uint16_t value ) {

      m_threshold = value;
      return;
   }

   /// @param level The message level in which the channel configuration should
   ///              be printed
   ///
   void ChannelConfig::printConfig( msg::Level level ) const {

      m_logger << level
               << tr( " - Threshold      : %1" ).arg( m_threshold )
               << msg::endmsg;
      return;
   }

   void ChannelConfig::clear() {

      m_channel = -1;
      m_threshold = 1;
      return;
   }

} // namespace v812
