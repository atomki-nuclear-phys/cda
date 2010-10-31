// $Id$

// Qt include(s):
#include <QtCore/QIODevice>
#include <QtCore/QDataStream>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>

// Local include(s):
#include "ChannelConfig.h"

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

   ChannelConfig::ChannelConfig()
      : m_numberOfChannels( 100 ),
        m_lowerBound( 0. ), m_upperBound( 100. ), m_name( "time" ),
        m_logger( "ad1000::ChannelConfig" ) {

      m_logger << msg::VERBOSE << "Object created" << msg::endmsg;
   }

   bool ChannelConfig::readConfig( QIODevice* dev ) {

      m_logger << msg::VERBOSE << "Reading configuration from binary input"
               << msg::endmsg;

      clear();

      QDataStream input( dev );
      input.setVersion( QDataStream::Qt_4_0 );
      input >> m_numberOfChannels;
      input >> m_lowerBound;
      input >> m_upperBound;
      input >> m_name;

      printConfig( msg::VERBOSE );

      return true;
   }

   bool ChannelConfig::writeConfig( QIODevice* dev ) const {

      m_logger << msg::VERBOSE << "Writing configuration to binary output"
               << msg::endmsg;

      QDataStream output( dev );
      output.setVersion( QDataStream::Qt_4_0 );
      output << m_numberOfChannels;
      output << m_lowerBound;
      output << m_upperBound;
      output << m_name;

      return true;
   }

   bool ChannelConfig::readConfig( const QDomElement& element ) {

      m_logger << msg::VERBOSE << "Reading configuration from XML input"
               << msg::endmsg;

      clear();

      // The element has to be an element:
      if( ! element.isElement() ) {
         m_logger << msg::ERROR << "Node received is not a DomElement"
                  << msg::endmsg;
         return false;
      }

      bool ok;

      m_numberOfChannels = element.attribute( "NumberOfChannels",
                                              "100" ).toInt( &ok );
      if( ! ok ) {
         m_logger << msg::ERROR << "There was a problem reading a "
                  << "\"number of channels\" value" << msg::endmsg;
         return false;
      }

      m_lowerBound = element.attribute( "LowerBound", "0." ).toDouble( &ok );
      if( ! ok ) {
         m_logger << msg::ERROR << "There was a problem reading a "
                  << "\"lower bound\" value" << msg::endmsg;
         return false;
      }

      m_upperBound = element.attribute( "UpperBound", "100." ).toDouble( &ok );
      if( ! ok ) {
         m_logger << msg::ERROR << "There was a problem reading an "
                  << "\"upper bound\" value" << msg::endmsg;
         return false;
      }

      m_name = element.attribute( "Name", "" );

      printConfig( msg::VERBOSE );

      return true;
   }

   bool ChannelConfig::writeConfig( QDomElement& element ) const {

      m_logger << msg::VERBOSE << "Writing configuration to XML output"
               << msg::endmsg;

      // The element has to be an element:
      if( ! element.isElement() ) {
         m_logger << msg::ERROR << "Node received is not a DomElement"
                  << msg::endmsg;
         return false;
      }

      element.setAttribute( "NumberOfChannels", m_numberOfChannels );
      element.setAttribute( "LowerBound", m_lowerBound );
      element.setAttribute( "UpperBound", m_upperBound );
      element.setAttribute( "Name", m_name );

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

   /**
    * @param level The message level in which the channel configuration should
    *              be printed
    */
   void ChannelConfig::printConfig( msg::Level level ) const {

      m_logger << level << " - Number of channels: "
               << m_numberOfChannels << std::endl;
      m_logger << " - Lower bound       : "
               << m_lowerBound << std::endl;
      m_logger << " - Upper bound       : "
               << m_upperBound << std::endl;
      m_logger << " - Name              : "
               << m_name << msg::endmsg;

      return;
   }

   void ChannelConfig::clear() {

      m_numberOfChannels = 100;
      m_lowerBound = 0.;
      m_upperBound = 100.;
      m_name = "";

      return;
   }

} // namespace ad1000
