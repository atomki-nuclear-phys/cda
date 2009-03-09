// $Id$

// Qt include(s):
#include <QtCore/QIODevice>
#include <QtCore/QStringList>
#include <QtXml/QDomElement>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/BinaryStream.h"
#else
#   include "common/BinaryStream.h"
#endif

// Local include(s):
#include "CamacReader.h"

namespace daq {

   CamacReader::CamacReader()
      : Application(), m_clients(), m_logger( "daq::CamacReader" ) {

   }

   void CamacReader::clear() {

      Application::clear();
      m_clients.clear();

      m_logger << msg::VERBOSE << tr( "Cleared settings" ) << msg::endmsg;

      return;

   }

   const std::vector< Address >& CamacReader::getClients() const {

      return m_clients;

   }

   void CamacReader::addClient( const Address& address ) {

      m_clients.push_back( address );
      return;

   }

   bool CamacReader::readConfig( QIODevice* dev ) {

      m_logger << msg::VERBOSE << tr( "Reading configuration from binary input" )
               << msg::endmsg;

      clear();
      setName( "cda-camac-reader" );

      //
      // Unfortunately two separate stream objects are needed:
      //
      BinaryStream input( dev );
      QDataStream intinput( dev );
      intinput.setVersion( QDataStream::Qt_4_0 );

      //
      // Read the message server addresses:
      //
      quint32 address_number = 0;
      intinput >> address_number;
      for( quint32 i = 0; i < address_number; ++i ) {
         Address address;
         input >> address;
         addMsgServer( address );
      }

      //
      // Read the event client addresses:
      //
      address_number = 0;
      intinput >> address_number;
      for( quint32 i = 0; i < address_number; ++i ) {
         Address address;
         input >> address;
         m_clients.push_back( address );
      }

      return true;

   }

   bool CamacReader::writeConfig( QIODevice* dev ) const {

      m_logger << msg::VERBOSE << tr( "Writing configuration to binary output" )
               << msg::endmsg;

      //
      // Unfortunately two separate stream objects are needed:
      //
      BinaryStream output( dev );
      QDataStream intoutput( dev );
      intoutput.setVersion( QDataStream::Qt_4_0 );

      //
      // Write what message servers to send messages to:
      //
      quint32 address_number = getMsgServers().size();
      intoutput << address_number;
      for( std::vector< Address >::const_iterator server = getMsgServers().begin();
           server != getMsgServers().end(); ++server ) {
         output << *server;
      }

      //
      // Write which clients to send events to:
      //
      address_number = m_clients.size();
      intoutput << address_number;
      for( std::vector< Address >::const_iterator client = m_clients.begin();
           client != m_clients.end(); ++client ) {
         output << *client;
      }

      return true;

   }

   bool CamacReader::readConfig( const QDomElement& node ) {

      m_logger << msg::VERBOSE << tr( "Reading configuration from XML input" )
               << msg::endmsg;

      clear();
      setName( "cda-camac-reader" );

      //
      // Read what message servers to send messages to:
      //
      QString attribute = node.attribute( "MsgServers", "" );
      QStringList split = attribute.split( " ", QString::SkipEmptyParts );
      for( QStringList::const_iterator server = split.begin(); server != split.end();
           ++server ) {
         addMsgServer( Address( *server ) );
      }

      //
      // Read which clients to send events to:
      //
      attribute = node.attribute( "Clients", "" );
      split = attribute.split( " ", QString::SkipEmptyParts );
      for( QStringList::const_iterator client = split.begin(); client != split.end();
           ++client ) {
         m_clients.push_back( Address( *client ) );
      }

      return true;

   }

   bool CamacReader::writeConfig( QDomElement& node ) const {

      m_logger << msg::VERBOSE << tr( "Writing configuration to XML output" )
               << msg::endmsg;

      //
      // Write the message servers to send messages to:
      //
      QString attribute = "";
      for( std::vector< Address >::const_iterator server = getMsgServers().begin();
           server != getMsgServers().end(); ++server ) {
         if( attribute != "" ) attribute += " ";
         attribute += server->getHost().toString();
         attribute += ":";
         attribute += QString::number( server->getPort() );
      }
      node.setAttribute( "MsgServers", attribute );

      //
      // Write the clients to send events to:
      //
      attribute = "";
      for( std::vector< Address >::const_iterator client = m_clients.begin();
           client != m_clients.end(); ++client ) {
         if( attribute != "" ) attribute += " ";
         attribute += client->getHost().toString();
         attribute += ":";
         attribute += QString::number( client->getPort() );
      }
      node.setAttribute( "Clients", attribute );

      return true;

   }

} // namespace daq
