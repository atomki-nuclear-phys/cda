// $Id$

// Qt include(s):
#include <QtCore/QIODevice>
#include <QtCore/QDataStream>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>

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

   bool Device::readConfig( QIODevice* dev ) {

      m_logger << msg::VERBOSE << "Reading configuration from binary input"
               << msg::endmsg;

      clear();

      // Read the properties of this class:
      QDataStream input( dev );
      input.setVersion( QDataStream::Qt_4_0 );
      input >> m_slot;
      input >> m_generateLam;

      m_logger << msg::VERBOSE << " - Slot        : " << m_slot << std::endl
               << " - GenerateLAM : " << m_generateLam << msg::endmsg;

      // Now read the properties of the channel class:
      if( ! m_channel.readConfig( dev ) ) {
         m_logger << msg::ERROR << "The configuration of the channel couldn't be "
                  << "read!" << msg::endmsg;
         return false;
      }

      return true;
   }

   bool Device::writeConfig( QIODevice* dev ) const {

      m_logger << msg::VERBOSE << "Writing configuration to binary output"
               << msg::endmsg;

      // Write the properties of this class:
      QDataStream output( dev );
      output.setVersion( QDataStream::Qt_4_0 );
      output << m_slot;
      output << m_generateLam;

      // Write the properties of the channel:
      if( ! m_channel.writeConfig( dev ) ) {
         m_logger << msg::ERROR << "A problem happened while writing out the "
                  << "channel configuration" << msg::endmsg;
         return false;
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

      bool channelFound = false;
      for( int i = 0; i < element.childNodes().size(); ++i ) {

         // Only process "Channel" type child-nodes:
         if( element.childNodes().at( i ).nodeName() != "Channel" ) {
            continue;
         }

         // Sanity check:
         if( channelFound ) {
            m_logger << msg::WARNING << "Multiple channel configurations detected. "
                     << "Will use the first one!" << msg::endmsg;
            continue;
         }

         // Now read the configuration of the channel:
         if( ! m_channel.readConfig( element.childNodes().at( i ).toElement() ) ) {
            m_logger << msg::ERROR << "The configuration of the channel couldn't be "
                     << "read!" << msg::endmsg;
            return false;
         }
         channelFound = true; // Note that we managed to read the channel configuration
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

      // Write the properties of the device class:
      element.setAttribute( "Slot", m_slot );
      element.setAttribute( "GenerateLAM", m_generateLam );

      // Create a new node for the configuration of the channel:
      QDomElement ch_element =
         element.ownerDocument().createElement( "Channel" );
      if( ! m_channel.writeConfig( ch_element ) ) {
         m_logger << msg::ERROR << "A problem happened while writing out the "
                  << "channel configuration" << msg::endmsg;
         return false;
      }
      element.appendChild( ch_element );

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
      m_channel.clear();

      return;
   }

} // namespace ad1000
