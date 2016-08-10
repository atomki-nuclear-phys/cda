// $Id$

// Qt include(s):
#include <QtCore/QIODevice>
#include <QtCore/QDataStream>
#include <QtXml/QDomElement>

// CDA include(s):
#include "common/errorcheck.h"

// Local include(s):
#include "GroupConfig.h"
#include "ChannelConfig.h"

namespace dt5740 {

   GroupConfig::GroupConfig()
      : m_groupNumber( -1 ),
        m_trigThreshold( 0 ), m_dcOffset( 0 ), m_trigMask( 0 ),
        m_trigEnabled( false ), m_trigOutEnabled( false ),
        m_logger( "dt5740::GroupConfig" ) {

   }

   GroupConfig::GroupConfig( const GroupConfig& parent )
      : dev::IConfig(),
        m_groupNumber( parent.m_groupNumber ),
        m_trigThreshold( parent.m_trigThreshold ),
        m_dcOffset( parent.m_dcOffset ),
        m_trigMask( parent.m_trigMask ),
        m_trigEnabled( parent.m_trigEnabled ),
        m_trigOutEnabled( parent.m_trigOutEnabled ),
        m_logger( "dt5740::GroupConfig" ) {

      // Copy the channel configuration:
      for( int i = 0; i < CHANNELS_IN_GROUP; ++i ) {
         if( parent.m_channels[ i ] ) {
            m_channels[ i ].reset(
                     new ChannelConfig( *( parent.m_channels[ i ] ) ) );
         } else {
            m_channels[ i ].reset();
         }
      }
   }

   GroupConfig& GroupConfig::operator= ( const GroupConfig& rh ) {

      m_groupNumber = rh.m_groupNumber;
      m_trigThreshold = rh.m_trigThreshold;
      m_dcOffset = rh.m_dcOffset;
      m_trigMask = rh.m_trigMask;
      m_trigEnabled = rh.m_trigEnabled;
      m_trigOutEnabled = rh.m_trigOutEnabled;

      // Copy the channel configuration:
      for( int i = 0; i < CHANNELS_IN_GROUP; ++i ) {
         if( rh.m_channels[ i ] ) {
            m_channels[ i ].reset(
                     new ChannelConfig( *( rh.m_channels[ i ] ) ) );
         } else {
            m_channels[ i ].reset();
         }
      }

      return *this;
   }

   StatusCode GroupConfig::readConfig( QIODevice& dev ) {

      REPORT_VERBOSE( tr( "Reading configuration from binary input" ) );

      QDataStream input( &dev );
      input.setVersion( QDataStream::Qt_4_0 );

      // Read in the group-wide configurations:
      input >> m_groupNumber;
      input >> m_trigThreshold;
      input >> m_dcOffset;
      input >> m_trigMask;
      input >> m_trigEnabled;
      input >> m_trigOutEnabled;

      // Read in the number of active channels in the group:
      quint32 number_of_channels;
      input >> number_of_channels;

      // Read in the configuration of all the channels:
      for( quint32 i = 0; i < number_of_channels; ++i ) {

         std::unique_ptr< ChannelConfig > channel( new ChannelConfig() );
         CHECK( channel->readConfig( dev ) );

         if( ( channel->getChannelNumber() >=
               ( m_groupNumber * CHANNELS_IN_GROUP ) ) &&
             ( channel->getChannelNumber() <
               ( ( m_groupNumber + 1 ) * CHANNELS_IN_GROUP ) ) ) {
            if( m_channels[ channel->getChannelNumber() -
                            ( m_groupNumber * CHANNELS_IN_GROUP ) ] ) {
               m_logger << msg::WARNING
                        << tr( "Redefining channel number: %1" )
                  .arg( channel->getChannelNumber() )
                        << msg::endmsg;
            }
            m_channels[ channel->getChannelNumber() -
                  ( m_groupNumber * CHANNELS_IN_GROUP ) ].swap( channel );
         } else {
            REPORT_ERROR( tr( "There was a problem reading the configuration "
                              "of one channel" ) );
            return StatusCode::FAILURE;
         }
      }

      return StatusCode::SUCCESS;
   }

   StatusCode GroupConfig::writeConfig( QIODevice& dev ) const {

      REPORT_VERBOSE( tr( "Writing configuration to binary output" ) );

      QDataStream output( &dev );
      output.setVersion( QDataStream::Qt_4_0 );

      // Write out the group-wide configurations:
      output << m_groupNumber;
      output << m_trigThreshold;
      output << m_dcOffset;
      output << m_trigMask;
      output << m_trigEnabled;
      output << m_trigOutEnabled;

      // Count the number of configured channels:
      quint32 number_of_channels = 0;
      for( int i = 0; i < CHANNELS_IN_GROUP; ++i ) {
         if( m_channels[ i ] ) ++number_of_channels;
      }

      // Write the number of channels to follow:
      output << number_of_channels;

      // Write the channel configurations:
      for( int i = 0; i < CHANNELS_IN_GROUP; ++i ) {
         if( m_channels[ i ] ) {
            CHECK( m_channels[ i ]->writeConfig( dev ) );
         }
      }

      return StatusCode::SUCCESS;
   }

   StatusCode GroupConfig::readConfig( const QDomElement& node ) {

      REPORT_VERBOSE( tr( "Reading configuration from XML input" ) );

      bool ok;

      //
      // Read in the group-wide configurations:
      //
      m_groupNumber = node.attribute( "Number", "0" ).toInt( &ok );
      CHECK( ok );

      m_trigThreshold = node.attribute( "TrigThreshold", "0" ).toInt( &ok );
      CHECK( ok );

      m_dcOffset = node.attribute( "DCOffset", "0" ).toInt( &ok );
      CHECK( ok );

      m_trigMask = node.attribute( "TrigMask", "0" ).toUInt( &ok );
      CHECK( ok );

      m_trigEnabled = node.attribute( "TrigEnabled", "0" ).toInt( &ok );
      CHECK( ok );

      m_trigOutEnabled = node.attribute( "TrigOutEnabled",
                                         "0" ).toInt( &ok );
      CHECK( ok );

      //
      // Configure the channels:
      //
      for( int i = 0; i < node.childNodes().size(); ++i ) {

         // Only process "Channel" type child-nodes:
         if( node.childNodes().at( i ).nodeName() != "Channel" ) {
            continue;
         }

         std::unique_ptr< ChannelConfig > channel( new ChannelConfig() );
         CHECK( channel->readConfig( node.childNodes().at( i ).toElement() ) );

         if( ( channel->getChannelNumber() >=
               ( m_groupNumber * CHANNELS_IN_GROUP ) ) &&
             ( channel->getChannelNumber() <
               ( ( m_groupNumber + 1 ) * CHANNELS_IN_GROUP ) ) ) {
            if( m_channels[ channel->getChannelNumber() -
                            ( m_groupNumber * CHANNELS_IN_GROUP ) ] ) {
               m_logger << msg::WARNING
                        << tr( "Redefining channel number: %1" )
                  .arg( channel->getChannelNumber() )
                        << msg::endmsg;
            }
            m_channels[ channel->getChannelNumber() -
                  ( m_groupNumber * CHANNELS_IN_GROUP ) ].swap( channel );
         } else {
            REPORT_ERROR( tr( "There was a problem reading the configuration "
                              "of one channel" ) );
            return StatusCode::FAILURE;
         }
      }

      return StatusCode::SUCCESS;
   }

   StatusCode GroupConfig::writeConfig( QDomElement& node ) const {

      REPORT_VERBOSE( tr( "Writing configuration to XML output" ) );

      //
      // Write out the group-wide configurations:
      //
      node.setAttribute( "Number", m_groupNumber );
      node.setAttribute( "TrigThreshold", m_trigThreshold );
      node.setAttribute( "DCOffset", m_dcOffset );
      node.setAttribute( "TrigMask", m_trigMask );
      node.setAttribute( "TrigEnabled", m_trigEnabled );
      node.setAttribute( "TrigOutEnabled", m_trigOutEnabled );

      //
      // Create a new node for the configuration of each channel:
      //
      for( int i = 0; i < CHANNELS_IN_GROUP; ++i ) {
         if( m_channels[ i ] ) {
            QDomElement ch_element =
               node.ownerDocument().createElement( "Channel" );
            CHECK( m_channels[ i ]->writeConfig( ch_element ) );
            node.appendChild( ch_element );
         }
      }

      return StatusCode::SUCCESS;
   }

   int GroupConfig::getGroupNumber() const {

      return m_groupNumber;
   }

   void GroupConfig::setGroupNumber( int value ) {

      m_groupNumber = value;
      return;
   }

   bool GroupConfig::getTrigEnabled() const {

      return m_trigEnabled;
   }

   void GroupConfig::setTrigEnabled( bool on ) {

      m_trigEnabled = on;
      return;
   }

   bool GroupConfig::getTrigOutEnabled() const {

      return m_trigOutEnabled;
   }

   void GroupConfig::setTrigOutEnabled( bool on ) {

      m_trigOutEnabled = on;
      return;
   }

   int GroupConfig::getTrigThreshold() const {

      return m_trigThreshold;
   }

   void GroupConfig::setTrigThreshold( int value ) {

      m_trigThreshold = value;
      return;
   }

   unsigned int GroupConfig::getTrigMask() const {

      return m_trigMask;
   }

   void GroupConfig::setTrigMask( unsigned int mask ) {

      m_trigMask = mask;
      return;
   }

   int GroupConfig::getDCOffset() const {

      return m_dcOffset;
   }

   void GroupConfig::setDCOffset( int value ) {

      m_dcOffset = value;
      return;
   }

   void GroupConfig::enableChannel( int channel, bool enable ) {

      if( enable ) {
         m_channels[ channel ].reset( new ChannelConfig() );
         m_channels[ channel ]->setChannelNumber( channel +
                                                  m_groupNumber *
                                                  CHANNELS_IN_GROUP );
      } else {
         m_channels[ channel ].reset();
      }

      return;
   }

   ChannelConfig* GroupConfig::getChannel( int channel ) {

      if( ( channel >= 0 ) &&
          ( channel < CHANNELS_IN_GROUP ) ) {
         return m_channels[ channel ].get();
      }

      REPORT_ERROR( tr( "Channel with invalid index (%1) requested" )
                    .arg( channel ) );
      return 0;
   }

   const ChannelConfig* GroupConfig::getChannel( int channel ) const {

      if( ( channel >= 0 ) &&
          ( channel < CHANNELS_IN_GROUP ) ) {
         return m_channels[ channel ].get();
      }

      REPORT_ERROR( tr( "Channel with invalid index (%1) requested" )
                    .arg( channel ) );
      return 0;
   }

   void GroupConfig::clear() {

      // Reset all regulat parameters:
      m_trigThreshold = 0;
      m_dcOffset = 0;
      m_trigMask = 0;
      m_trigEnabled = false;
      m_trigOutEnabled = false;

      // Delete all the channels:
      for( int i = 0; i < CHANNELS_IN_GROUP; ++i ) {
         m_channels[ i ].reset();
      }

      return;
   }

} // namespace dt5740
