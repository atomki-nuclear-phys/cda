// $Id$

// Qt include(s):
#include <QtCore/QIODevice>
#include <QtCore/QDataStream>
#include <QtXml/QDomElement>

// CDA include(s):
#include "common/errorcheck.h"

// Local include(s):
#include "ChannelConfig.h"

namespace v862 {

   //
   // Make sure that the following Qt classes are available in the
   // current namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QIODevice );
   using QT_PREPEND_NAMESPACE( QDataStream );
   using QT_PREPEND_NAMESPACE( QDomElement );

   ChannelConfig::ChannelConfig()
      : m_channel( 0 ), m_nBins( 100 ),
        m_lowerBound( 0. ), m_upperBound( 100. ), m_name( "" ),
        m_threshold( 0 ),
        m_logger( "v862::ChannelConfig" ) {

      REPORT_VERBOSE( tr( "Object created" ) );
   }

   bool ChannelConfig::readConfig( QIODevice& dev ) {

      REPORT_VERBOSE( tr( "Reading configuration from binary input" ) );

      clear();

      QDataStream input( &dev );
      input.setVersion( QDataStream::Qt_4_0 );
      input >> m_channel;
      input >> m_nBins;
      input >> m_lowerBound;
      input >> m_upperBound;
      input >> m_name;
      input >> m_threshold;

      printConfig( msg::VERBOSE );

      return true;
   }

   bool ChannelConfig::writeConfig( QIODevice& dev ) const {

      REPORT_VERBOSE( tr( "Writing configuration to binary output" ) );

      QDataStream output( &dev );
      output.setVersion( QDataStream::Qt_4_0 );
      output << m_channel;
      output << m_nBins;
      output << m_lowerBound;
      output << m_upperBound;
      output << m_name;
      output << m_threshold;

      return true;
   }

   bool ChannelConfig::readConfig( const QDomElement& element ) {

      REPORT_VERBOSE( tr( "Reading configuration from XML input" ) );

      clear();

      bool ok;

      m_channel = element.attribute( "ChannelNumber", "" ).toInt( &ok );
      CHECK( ok );

      m_nBins = element.attribute( "NBins", "100" ).toInt( &ok );
      CHECK( ok );

      m_lowerBound = element.attribute( "LowerBound", "0." ).toDouble( &ok );
      CHECK( ok );

      m_upperBound = element.attribute( "UpperBound", "100." ).toDouble( &ok );
      CHECK( ok );

      m_name = element.attribute( "Name", "" );

      m_threshold = element.attribute( "Threshold", "" ).toInt( &ok );
      CHECK( ok );

      printConfig( msg::VERBOSE );

      return true;
   }

   bool ChannelConfig::writeConfig( QDomElement& element ) const {

      REPORT_VERBOSE( tr( "Writing configuration to XML output" ) );

      element.setAttribute( "ChannelNumber", m_channel );
      element.setAttribute( "NBins", m_nBins );
      element.setAttribute( "LowerBound", m_lowerBound );
      element.setAttribute( "UpperBound", m_upperBound );
      element.setAttribute( "Name", m_name );
      element.setAttribute( "Threshold", m_threshold );

      return true;
   }

   int ChannelConfig::getChannel() const {

      return m_channel;
   }

   int ChannelConfig::getNBins() const {

      return m_nBins;
   }

   double ChannelConfig::getLowerBound() const {

      return m_lowerBound;
   }

   double ChannelConfig::getUpperBound() const {

      return m_upperBound;
   }

   int ChannelConfig::getThreshold() const {

      return m_threshold;
   }

   const QString& ChannelConfig::getName() const {

      return m_name;
   }

   void ChannelConfig::setChannel( int value ) {

      m_channel = value;
      return;
   }

   void ChannelConfig::setNBins( int value ) {

      m_nBins = value;
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

   void ChannelConfig::setThreshold( int value ) {

      m_threshold = value;
      return;
   }

   /**
    * @param level The message level in which the channel configuration should
    *              be printed
    */
   void ChannelConfig::printConfig( msg::Level level ) const {

      m_logger << level
               << tr( " - ChannelNumber  : %1\n"
                      " - Number of bins : %2\n"
                      " - Lower bound    : %3\n"
                      " - Upper bound    : %4\n"
                      " - Name           : %5\n"
                      " - Threshold      : %6" )
                  .arg( m_channel ).arg( m_nBins )
                  .arg( m_lowerBound ).arg( m_upperBound ).arg( m_name )
                  .arg( m_threshold )
               << msg::endmsg;

      return;
   }

   void ChannelConfig::clear() {

      m_channel = -1;
      m_nBins = 100;
      m_lowerBound = 0.;
      m_upperBound = 100.;
      m_name = "";
      m_threshold = 0;

      return;
   }

} // namespace v862
