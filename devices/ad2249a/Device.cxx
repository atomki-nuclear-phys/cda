// $Id$

// Qt include(s):
#include <QtCore/QIODevice>
#include <QtCore/QDataStream>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>

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

      // Reset all the pointers in the array:
      for( int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i ) {
         m_channels[ i ] = 0;
      }

   }

   Device::~Device() {

      clear();

   }

   bool Device::readConfig( QIODevice* dev ) {

      m_logger << msg::VERBOSE << "Reading configuration from binary input"
               << msg::endmsg;

      clear();

      QDataStream input( dev );
      input.setVersion( QDataStream::Qt_4_0 );
      input >> m_slot;
      input >> m_generateLam;
      quint32 number_of_channels;
      input >> number_of_channels;

      m_logger << msg::VERBOSE << " - Slot        : " << m_slot << std::endl
               << " - GenerateLAM : " << m_generateLam << std::endl
               << " - Subaddresses: " << number_of_channels
               << msg::endmsg;

      for( quint32 i = 0; i < number_of_channels; ++i ) {
         ChannelConfig* channel = new ChannelConfig();
         if( ! channel->readConfig( dev ) ) {
            m_logger << msg::ERROR << "The configuration of a channel couldn't be "
                     << "read!" << msg::endmsg;
            delete channel;
            return false;
         }
         if( ( channel->getSubaddress() >= 0 ) &&
             ( channel->getSubaddress() < NUMBER_OF_SUBADDRESSES ) ) {
            if( m_channels[ channel->getSubaddress() ] ) {
               m_logger << msg::WARNING << "Redefining channel number: "
                        << channel->getSubaddress() << msg::endmsg;
               delete m_channels[ channel->getSubaddress() ];
            }
            m_channels[ channel->getSubaddress() ] = channel;
         } else {
            m_logger << msg::ERROR << "There was a problem reading the configuration "
                     << "of one channel" << msg::endmsg;
            delete channel;
         }
      }

      return true;

   }

   bool Device::writeConfig( QIODevice* dev ) const {

      m_logger << msg::VERBOSE << "Writing configuration to binary output"
               << msg::endmsg;

      QDataStream output( dev );
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
            if( ! m_channels[ i ]->writeConfig( dev ) ) {
               m_logger << msg::ERROR << "A problem happened while writing out a "
                        << "channel configuration" << msg::endmsg;
               return false;
            }
         }
      }

      return true;

   }

   bool Device::readConfig( const QDomElement& element ) {

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

      m_slot = element.attribute( "Slot", "0" ).toInt( &ok );
      if( ! ok ) {
         m_logger << msg::ERROR << "There was a problem reading the "
                  << "\"Slot\" property!" << msg::endmsg;
         return false;
      }

      m_generateLam = element.attribute( "GenerateLAM", "0" ).toShort( &ok );
      if( ! ok ) {
         m_logger << msg::ERROR << "There was a problem reading the "
                  << "\"generate LAM\" property!" << msg::endmsg;
         return false;
      }

      m_logger << msg::VERBOSE << " - Slot       : " << m_slot << std::endl
               << " - GenerateLAM: " << m_generateLam << msg::endmsg;

      for( int i = 0; i < element.childNodes().size(); ++i ) {

         // Only process "Channel" type child-nodes:
         if( element.childNodes().at( i ).nodeName() != "Channel" ) {
            continue;
         }

         ChannelConfig* channel = new ChannelConfig();
         if( ! channel->readConfig( element.childNodes().at( i ).toElement() ) ) {
            m_logger << msg::ERROR << "The configuration of a channel couldn't be "
                     << "read!" << msg::endmsg;
            delete channel;
            return false;
         }
         if( ( channel->getSubaddress() >= 0 ) &&
             ( channel->getSubaddress() < NUMBER_OF_SUBADDRESSES ) ) {
            if( m_channels[ channel->getSubaddress() ] ) {
               m_logger << msg::WARNING << "Redefining channel number: "
                        << channel->getSubaddress() << msg::endmsg;
               delete m_channels[ channel->getSubaddress() ];
            }
            m_channels[ channel->getSubaddress() ] = channel;
         } else {
            m_logger << msg::ERROR << "There was a problem reading the configuration "
                     << "of one channel" << msg::endmsg;
            delete channel;
         }
      }

      return true;

   }

   bool Device::writeConfig( QDomElement& element ) const {

      m_logger << msg::VERBOSE << "Writing configuration to XML output"
               << msg::endmsg;

      // The element has to be an element:
      if( ! element.isElement() ) {
         m_logger << msg::ERROR << "Node received is not a DomElement"
                  << msg::endmsg;
         return false;
      }

      element.setAttribute( "Slot", m_slot );
      element.setAttribute( "GenerateLAM", m_generateLam );

      //
      // Create a new node for the configuration of each channel:
      //
      for( int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i ) {
         if( m_channels[ i ] ) {

            QDomElement ch_element =
               element.ownerDocument().createElement( "Channel" );
            if( ! m_channels[ i ]->writeConfig( ch_element ) ) {
               m_logger << msg::ERROR << "A problem happened while writing out a "
                        << "channel configuration" << msg::endmsg;
               return false;
            }
            element.appendChild( ch_element );

         }
      }

      return true;

   }

   int Device::getSlot() const {

      return m_slot;

   }

   void Device::setSlot( int value ) {

      m_slot = value;
      return;

   }

   void Device::clear() {

      m_slot = -1;
      m_generateLam = false;
      for( int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i ) {
         if( m_channels[ i ] ) delete m_channels[ i ];
         m_channels[ i ] = 0;
      }

      return;

   }

} // namespace ad2249a
