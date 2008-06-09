// $Id$

// STL include(s):
#include <iostream>

// Qt include(s):
#include <QtCore/QIODevice>
#include <QtCore/QDataStream>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>

// Local include(s):
#include "Config.h"

namespace camac {

   //
   // Make sure that the following Qt classes are available in the
   // current namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QIODevice );
   using QT_PREPEND_NAMESPACE( QDataStream );
   using QT_PREPEND_NAMESPACE( QDomNode );
   using QT_PREPEND_NAMESPACE( QDomElement );

   /**
    * The constructor initialises m_logger and sets all properties
    * to sensible defaults.
    */
   Config::Config()
      : m_logger( "Example::Config" ) {

      m_booleanProperty = true;
      m_integerProperty = 666;
      m_doubleProperty  = 3.141592;
      m_stringProperty  = "Example_test";

      m_logger << msg::VERBOSE << "Example::Config created" << msg::endmsg;

   }

   /**
    * The function reads all property member from the QIODevice
    * using a QDataStream in a specified order. In the end it prints
    * the values of the properties just read.
    */
   bool Config::readConfig( QIODevice* dev ) {

      m_logger << msg::VERBOSE << "Reading configuration from binary input"
               << msg::endmsg;

      QDataStream input( dev );
      input.setVersion( QDataStream::Qt_4_0 );
      input >> m_booleanProperty;
      input >> m_integerProperty;
      input >> m_doubleProperty;
      input >> m_stringProperty;

      m_logger << msg::VERBOSE << " - m_booleanProperty : "
               << m_booleanProperty << std::endl;
      m_logger << msg::VERBOSE << " - m_integerProperty : "
               << m_integerProperty << std::endl;
      m_logger << msg::VERBOSE << " - m_doubleProperty  : "
               << m_doubleProperty << std::endl;
      m_logger << msg::VERBOSE << " - m_stringProperty  : "
               << m_stringProperty << std::endl;

      return true;

   }

   /**
    * The function writes all property member to the QIODevice
    * using QDataStream in a specified order.
    */
   bool Config::writeConfig( QIODevice* dev ) const {

      m_logger << msg::VERBOSE << "Writing configuration to binary input"
               << msg::endmsg;

      QDataStream input( dev );
      input.setVersion( QDataStream::Qt_4_0 );
      input << m_booleanProperty;
      input << m_integerProperty;
      input << m_doubleProperty;
      input << m_stringProperty;

      return true;

   }

   /**
    * The function tries to read all property member from the XML
    * node using standard Qt XML access methods.
    */
   bool Config::readConfig( const QDomNode* node ) {

      m_logger << msg::VERBOSE << "Reading configuration from XML input"
               << msg::endmsg;

      // The node has to be an element:
      if( ! node->isElement() ) {
         m_logger << msg::ERROR << "Node received is not a DomElement"
                  << msg::endmsg;
         return false;
      }
      const QDomElement element = node->toElement();

      //
      // Read the attributes of the object. Since the default values can
      // always be translated, the "ok == false" states don't have to be
      // explicitly handled.
      //
      bool ok;
      m_booleanProperty = element.attribute( "boolean",
                                             "0" ).toShort( &ok );
      m_integerProperty = element.attribute( "integer",
                                             "0" ).toInt( &ok );
      m_doubleProperty = element.attribute( "double",
                                            "0.0" ).toDouble( &ok );
      m_stringProperty = element.attribute( "string", "" );

      m_logger << msg::VERBOSE << " - m_booleanProperty : "
               << m_booleanProperty << std::endl;
      m_logger << msg::VERBOSE << " - m_integerProperty : "
               << m_integerProperty << std::endl;
      m_logger << msg::VERBOSE << " - m_doubleProperty  : "
               << m_doubleProperty << std::endl;
      m_logger << msg::VERBOSE << " - m_stringProperty  : "
               << m_stringProperty << std::endl;

      return true;

   }

   /**
    * The function writes to all property member from the XML
    * node using standard Qt XML access methods.
    */
   bool Config::writeConfig( QDomNode* node ) const {

      m_logger << msg::VERBOSE << "Writing configuration to XML output"
               << msg::endmsg;

      // The node has to be an element:
      if( ! node->isElement() ) {
         m_logger << msg::ERROR << "Node received is not a DomElement"
                  << msg::endmsg;
         return false;
      }
      QDomElement element = node->toElement();

      element.setAttribute( "boolean", m_booleanProperty );
      element.setAttribute( "integer", m_integerProperty );
      element.setAttribute( "double", m_doubleProperty );
      element.setAttribute( "string", m_stringProperty );

      return true;

   }

} // namespace camac
