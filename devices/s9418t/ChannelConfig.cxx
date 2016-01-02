// $Id$

// Qt include(s):
#include <QtCore/QIODevice>
#include <QtCore/QDataStream>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>

// Local include(s):
#include "ChannelConfig.h"

namespace s9418t {

   //
   // Make sure that the following Qt classes are available in the
   // current namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QIODevice );
   using QT_PREPEND_NAMESPACE( QDataStream );
   using QT_PREPEND_NAMESPACE( QDomNode );
   using QT_PREPEND_NAMESPACE( QDomElement );

   ChannelConfig::ChannelConfig()
      : m_channel( -1 ), m_numberOfChannels( 100 ),
        m_lowerBound( 0. ), m_upperBound( 100. ), m_name( "time" ),
        m_threshold( 0 ),
        m_logger( "s9418t::ChannelConfig" ) {

      m_logger << msg::VERBOSE << tr( "Object created" ) << msg::endmsg;
   }

   bool ChannelConfig::readConfig( QIODevice& dev ) {

      m_logger << msg::VERBOSE
               << tr( "Reading configuration from binary input" )
               << msg::endmsg;

      clear();

      QDataStream input( &dev );
      input.setVersion( QDataStream::Qt_4_0 );
      input >> m_channel;
      input >> m_numberOfChannels;
      input >> m_lowerBound;
      input >> m_upperBound;
      input >> m_name;
      input >> m_threshold;

      printConfig( msg::VERBOSE );

      return true;
   }

   bool ChannelConfig::writeConfig( QIODevice& dev ) const {

      m_logger << msg::VERBOSE
               << tr( "Writing configuration to binary output" )
               << msg::endmsg;

      QDataStream output( &dev );
      output.setVersion( QDataStream::Qt_4_0 );
      output << m_channel;
      output << m_numberOfChannels;
      output << m_lowerBound;
      output << m_upperBound;
      output << m_name;
      output << m_threshold;

      return true;
   }

   bool ChannelConfig::readConfig( const QDomElement& element ) {

      m_logger << msg::VERBOSE
               << tr( "Reading configuration from XML input" )
               << msg::endmsg;

      clear();

      bool ok;

      m_channel = element.attribute( "Number", "" ).toInt( &ok );
      if( ! ok ) {
         m_logger << msg::ERROR << tr( "There was a problem reading a "
                                       "\"channel number\" value" )
                  << msg::endmsg;
         return false;
      }

      m_numberOfChannels = element.attribute( "NumberOfChannels",
                                              "100" ).toInt( &ok );
      if( ! ok ) {
         m_logger << msg::ERROR << tr( "There was a problem reading a "
                                       "\"number of channels\" value" )
                  << msg::endmsg;
         return false;
      }

      m_lowerBound = element.attribute( "LowerBound", "0." ).toDouble( &ok );
      if( ! ok ) {
         m_logger << msg::ERROR << tr( "There was a problem reading a "
                                       "\"lower bound\" value" )
                  << msg::endmsg;
         return false;
      }

      m_upperBound = element.attribute( "UpperBound", "100." ).toDouble( &ok );
      if( ! ok ) {
         m_logger << msg::ERROR << tr( "There was a problem reading an "
                                       "\"upper bound\" value" )
                  << msg::endmsg;
         return false;
      }

      m_name = element.attribute( "Name", "" );

      m_threshold = element.attribute( "Threshold", "" ).toShort( &ok );
      if( ! ok ) {
         m_logger << msg::ERROR << tr( "There was a problem reading a "
                                       "\"threshold\" value" )
                  << msg::endmsg;
         return false;
      }

      printConfig( msg::VERBOSE );

      return true;
   }

   bool ChannelConfig::writeConfig( QDomElement& element ) const {

      m_logger << msg::VERBOSE
               << tr( "Writing configuration to XML output" )
               << msg::endmsg;

      element.setAttribute( "Number", m_channel );
      element.setAttribute( "NumberOfChannels", m_numberOfChannels );
      element.setAttribute( "LowerBound", m_lowerBound );
      element.setAttribute( "UpperBound", m_upperBound );
      element.setAttribute( "Name", m_name );
      element.setAttribute( "Threshold", m_threshold );

      return true;
   }

   int ChannelConfig::getChannel() const {

      return m_channel;
   }

   int ChannelConfig::getNumberOfChannels() const {

      return m_numberOfChannels;
   }

   double ChannelConfig::getLowerBound() const {

      return m_lowerBound;
   }

   double ChannelConfig::getUpperBound() const {

      return m_upperBound;
   }

   const QString& ChannelConfig::getName() const {

      return m_name;
   }

   short ChannelConfig::getThreshold() const {

      return m_threshold;
   }

   void ChannelConfig::setChannel( int value ) {

      m_channel = value;
      return;
   }

   void ChannelConfig::setNumberOfChannels( int value ) {

      m_numberOfChannels = value;
      return;
   }

   void ChannelConfig::setLowerBound( double value ) {

      m_lowerBound = value;
      return;
   }

   void ChannelConfig::setUpperBound( double value ) {

      m_upperBound = value;
      return;
   }

   void ChannelConfig::setName( const QString& value ) {

      m_name = value;
      return;
   }

   void ChannelConfig::setThreshold( short value ) {

      m_threshold = value;
      return;
   }

   /**
    * @param level The message level in which the channel configuration should
    *              be printed
    */
   void ChannelConfig::printConfig( msg::Level level ) const {

      m_logger << level
               << tr( " - Channel number    : %1\n"
                      " - Number of channels: %2\n"
                      " - Lower bound       : %3\n"
                      " - Upper bound       : %4\n"
                      " - Name              : %5\n"
                      " - Threshold         : %6" )
         .arg( m_channel ).arg( m_numberOfChannels )
         .arg( m_lowerBound ).arg( m_upperBound ).arg( m_name )
         .arg( m_threshold )
               << msg::endmsg;

      return;
   }

   void ChannelConfig::clear() {

      m_channel = -1;
      m_numberOfChannels = 100;
      m_lowerBound = 0.;
      m_upperBound = 100.;
      m_name = "";
      m_threshold = 0;

      return;
   }

} // namespace s9418t
