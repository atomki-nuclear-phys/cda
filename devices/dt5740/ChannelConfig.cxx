// $Id$

// Qt include(s):
#include <QtCore/QIODevice>
#include <QtCore/QDataStream>
#include <QtXml/QDomElement>

// CDA include(s):
#include "common/errorcheck.h"

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
      : m_rawName( "raw" ),
        m_timeNumberOfChannels( 100 ), m_timeLowerBound( 0. ),
        m_timeUpperBound( 100. ), m_timeName( "time" ),
        m_energyNumberOfChannels( 100 ), m_energyLowerBound( 0. ),
        m_energyUpperBound( 100. ), m_energyName( "energy" ),
        m_channelNumber( -1 ),
        m_logger( "dt5740::ChannelConfig" ) {

   }

   StatusCode ChannelConfig::readConfig( QIODevice& dev ) {

      REPORT_VERBOSE( tr( "Reading configuration from binary input" ) );

      clear();

      QDataStream input( &dev );
      input.setVersion( QDataStream::Qt_4_0 );
      input >> m_rawName;
      input >> m_timeNumberOfChannels;
      input >> m_timeLowerBound;
      input >> m_timeUpperBound;
      input >> m_timeName;
      input >> m_energyNumberOfChannels;
      input >> m_energyLowerBound;
      input >> m_energyUpperBound;
      input >> m_energyName;
      input >> m_channelNumber;

      printConfig( msg::VERBOSE );

      return StatusCode::SUCCESS;
   }

   StatusCode ChannelConfig::writeConfig( QIODevice& dev ) const {

      REPORT_VERBOSE( tr( "Writing configuration to binary output" ) );

      QDataStream output( &dev );
      output.setVersion( QDataStream::Qt_4_0 );
      output << m_rawName;
      output << m_timeNumberOfChannels;
      output << m_timeLowerBound;
      output << m_timeUpperBound;
      output << m_timeName;
      output << m_energyNumberOfChannels;
      output << m_energyLowerBound;
      output << m_energyUpperBound;
      output << m_energyName;
      output << m_channelNumber;

      return StatusCode::SUCCESS;
   }

   StatusCode ChannelConfig::readConfig( const QDomElement& element ) {

      REPORT_VERBOSE( tr( "Reading configuration from XML input" ) );

      clear();

      bool ok;

      m_rawName = element.attribute( "RawName", "" );

      m_timeNumberOfChannels = element.attribute( "NumberOfTimeChannels",
                                                  "100" ).toInt( &ok );
      CHECK( ok );

      m_timeLowerBound = element.attribute( "TimeLowerBound",
                                            "0." ).toDouble( &ok );
      CHECK( ok );

      m_timeUpperBound = element.attribute( "TimeUpperBound",
                                            "100." ).toDouble( &ok );
      CHECK( ok );

      m_timeName = element.attribute( "TimeName", "" );

      m_energyNumberOfChannels = element.attribute( "NumberOfEnergyChannels",
                                                    "100" ).toInt( &ok );
      CHECK( ok );

      m_energyLowerBound = element.attribute( "EnergyLowerBound",
                                              "0." ).toDouble( &ok );
      CHECK( ok );

      m_energyUpperBound = element.attribute( "EnergyUpperBound",
                                              "100." ).toDouble( &ok );
      CHECK( ok );

      m_energyName = element.attribute( "EnergyName", "" );

      m_channelNumber = element.attribute( "ChannelNumber",
                                           "0" ).toInt( &ok );
      CHECK( ok );

      printConfig( msg::VERBOSE );

      return StatusCode::SUCCESS;
   }

   StatusCode ChannelConfig::writeConfig( QDomElement& element ) const {

      REPORT_VERBOSE( tr( "Writing configuration to XML output" ) );

      element.setAttribute( "RawName", m_rawName );
      element.setAttribute( "NumberOfTimeChannels", m_timeNumberOfChannels );
      element.setAttribute( "TimeLowerBound", m_timeLowerBound );
      element.setAttribute( "TimeUpperBound", m_timeUpperBound );
      element.setAttribute( "TimeName", m_timeName );
      element.setAttribute( "NumberOfEnergyChannels",
                            m_energyNumberOfChannels );
      element.setAttribute( "EnergyLowerBound", m_energyLowerBound );
      element.setAttribute( "EnergyUpperBound", m_energyUpperBound );
      element.setAttribute( "EnergyName", m_energyName );
      element.setAttribute( "ChannelNumber", m_channelNumber );

      return StatusCode::SUCCESS;
   }

   int ChannelConfig::getTimeNumberOfChannels() const {

      return m_timeNumberOfChannels;
   }

   double ChannelConfig::getTimeLowerBound() const {

      return m_timeLowerBound;
   }

   double ChannelConfig::getTimeUpperBound() const {

      return m_timeUpperBound;
   }

   const QString& ChannelConfig::getTimeName() const {

      return m_timeName;
   }

   void ChannelConfig::setTimeNumberOfChannels( int value ) {

      m_timeNumberOfChannels = value;
      return;
   }

   void ChannelConfig::setTimeLowerBound( double value ) {

      m_timeLowerBound = value;
      return;
   }

   void ChannelConfig::setTimeUpperBound( double value ) {

      m_timeUpperBound = value;
      return;
   }

   void ChannelConfig::setTimeName( const QString& value ) {

      m_timeName = value;
      return;
   }

   int ChannelConfig::getEnergyNumberOfChannels() const {

      return m_energyNumberOfChannels;
   }

   double ChannelConfig::getEnergyLowerBound() const {

      return m_energyLowerBound;
   }

   double ChannelConfig::getEnergyUpperBound() const {

      return m_energyUpperBound;
   }

   const QString& ChannelConfig::getEnergyName() const {

      return m_energyName;
   }

   void ChannelConfig::setEnergyNumberOfChannels( int value ) {

      m_energyNumberOfChannels = value;
      return;
   }

   void ChannelConfig::setEnergyLowerBound( double value ) {

      m_energyLowerBound = value;
      return;
   }

   void ChannelConfig::setEnergyUpperBound( double value ) {

      m_energyUpperBound = value;
      return;
   }

   void ChannelConfig::setEnergyName( const QString& value ) {

      m_energyName = value;
      return;
   }

   int ChannelConfig::getChannelNumber() const {

      return m_channelNumber;
   }

   void ChannelConfig::setChannelNumber( int number ) {

      m_channelNumber = number;
      return;
   }

   const QString& ChannelConfig::getRawName() const {

      return m_rawName;
   }

   void ChannelConfig::setRawName( const QString& value ) {

      m_rawName = value;
      return;
   }

   /**
    * @param level The message level in which the channel configuration should
    *              be printed
    */
   void ChannelConfig::printConfig( msg::Level level ) const {

      m_logger << level
               << tr( " - Channel number    : %1\n"
                      " - Raw channel name  : %2\n"
                      " - Number of time channels: %3\n"
                      " - Time lower bound       : %4\n"
                      " - Time upper bound       : %5\n"
                      " - Time name              : %6\n"
                      " - Number of energy channels: %7\n"
                      " - Energy lower bound       : %8\n"
                      " - Energy upper bound       : %9\n"
                      " - Energy name              : %10" )
         .arg( m_channelNumber ).arg( m_rawName )
         .arg( m_timeNumberOfChannels )
         .arg( m_timeLowerBound ).arg( m_timeUpperBound )
         .arg( m_timeName ).arg( m_energyNumberOfChannels )
         .arg( m_energyLowerBound ).arg( m_energyUpperBound )
         .arg( m_energyName )
               << msg::endmsg;

      return;
   }

   void ChannelConfig::clear() {

      m_rawName = "";

      m_timeNumberOfChannels = 100;
      m_timeLowerBound = 0.;
      m_timeUpperBound = 100.;
      m_timeName = "";

      m_energyNumberOfChannels = 100;
      m_energyLowerBound = 0.;
      m_energyUpperBound = 100.;
      m_energyName = "";

      m_channelNumber = -1;

      return;
   }

} // namespace dt5740
