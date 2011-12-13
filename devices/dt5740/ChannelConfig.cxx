// $Id$

// Qt include(s):
#include <QtCore/QIODevice>
#include <QtCore/QDataStream>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>

// Local include(s):
#include "ChannelConfig.h"

namespace dt5740 {

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
      : m_numberOfChannels( 100 ),
        m_lowerBound( 0. ), m_upperBound( 100. ), m_name( "time" ),
        m_channelNumber( -1 ),
        m_logger( "dt5740::ChannelConfig" ) {

   }

   ChannelConfig::ChannelConfig( const ChannelConfig& parent )
      : dev::Config(),
        m_numberOfChannels( parent.m_numberOfChannels ),
        m_lowerBound( parent.m_lowerBound ),
        m_upperBound( parent.m_upperBound ),
        m_name( parent.m_name ),
        m_channelNumber( parent.m_channelNumber ),
        m_logger( "dt5740::ChannelConfig" ) {

   }

   bool ChannelConfig::readConfig( QIODevice* dev ) {

      REPORT_VERBOSE( tr( "Reading configuration from binary input" ) );

      clear();

      QDataStream input( dev );
      input.setVersion( QDataStream::Qt_4_0 );
      input >> m_numberOfChannels;
      input >> m_lowerBound;
      input >> m_upperBound;
      input >> m_name;
      input >> m_channelNumber;

      printConfig( msg::VERBOSE );

      return true;
   }

   bool ChannelConfig::writeConfig( QIODevice* dev ) const {

      REPORT_VERBOSE( tr( "Writing configuration to binary output" ) );

      QDataStream output( dev );
      output.setVersion( QDataStream::Qt_4_0 );
      output << m_numberOfChannels;
      output << m_lowerBound;
      output << m_upperBound;
      output << m_name;
      output << m_channelNumber;

      return true;
   }

   bool ChannelConfig::readConfig( const QDomElement& element ) {

      REPORT_VERBOSE( tr( "Reading configuration from XML input" ) );

      clear();

      bool ok;

      m_numberOfChannels = element.attribute( "NumberOfChannels",
                                              "100" ).toInt( &ok );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading a "
                           "\"number of channels\" value" ) );
         return false;
      }

      m_lowerBound = element.attribute( "LowerBound", "0." ).toDouble( &ok );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading a "
                           "\"lower bound\" value" ) );
         return false;
      }

      m_upperBound = element.attribute( "UpperBound", "100." ).toDouble( &ok );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading an "
                           "\"upper bound\" value" ) );
         return false;
      }

      m_name = element.attribute( "Name", "" );

      m_channelNumber = element.attribute( "ChannelNumber",
                                           "0" ).toInt( &ok );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading a "
                           "\"channel number\" value" ) );
         return false;
      }

      printConfig( msg::VERBOSE );

      return true;
   }

   bool ChannelConfig::writeConfig( QDomElement& element ) const {

      REPORT_VERBOSE( tr( "Writing configuration to XML output" ) );

      element.setAttribute( "NumberOfChannels", m_numberOfChannels );
      element.setAttribute( "LowerBound", m_lowerBound );
      element.setAttribute( "UpperBound", m_upperBound );
      element.setAttribute( "Name", m_name );
      element.setAttribute( "ChannelNumber", m_channelNumber );

      return true;
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

   int ChannelConfig::getChannelNumber() const {

      return m_channelNumber;
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

   void ChannelConfig::setChannelNumber( int number ) {

      m_channelNumber = number;
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
                      " - Name              : %5" )
         .arg( m_channelNumber ).arg( m_numberOfChannels )
         .arg( m_lowerBound ).arg( m_upperBound )
         .arg( m_name )
               << msg::endmsg;

      return;
   }

   void ChannelConfig::clear() {

      m_numberOfChannels = 100;
      m_lowerBound = 0.;
      m_upperBound = 100.;
      m_name = "";
      m_channelNumber = -1;

      return;
   }

} // namespace dt5740
