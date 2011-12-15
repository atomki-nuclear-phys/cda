// $Id$

// Qt include(s):
#include <QtCore/QIODevice>
#include <QtCore/QDataStream>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>

// Local include(s):
#include "GroupConfig.h"
#include "ChannelConfig.h"

namespace dt5740 {

   GroupConfig::GroupConfig()
      : m_groupNumber( -1 ),
        m_trigThreshold( 0 ), m_dcOffset( 0 ), m_trigMask( 0 ),
        m_trigEnabled( false ), m_trigOutEnabled( false ),
        m_logger( "dt5740::GroupConfig" ) {

      // Reset all the pointers in the array:
      for( int i = 0; i < CHANNELS_IN_GROUP; ++i ) {
         m_channels[ i ] = 0;
      }
   }

   GroupConfig::GroupConfig( const GroupConfig& parent )
      : dev::Config(),
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
            m_channels[ i ] = new ChannelConfig( *( parent.m_channels[ i ] ) );
         } else {
            m_channels[ i ] = 0;
         }
      }
   }

   GroupConfig::~GroupConfig() {

      clear();
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
            m_channels[ i ] = new ChannelConfig( *( rh.m_channels[ i ] ) );
         } else {
            m_channels[ i ] = 0;
         }
      }

      return *this;
   }

   bool GroupConfig::readConfig( QIODevice* dev ) {

      REPORT_VERBOSE( tr( "Reading configuration from binary input" ) );

      QDataStream input( dev );
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
         ChannelConfig* channel = new ChannelConfig();
         if( ! channel->readConfig( dev ) ) {
            REPORT_ERROR( tr( "The configuration of a channel couldn't be "
                              "read!" ) );
            delete channel;
            return false;
         }
         if( ( channel->getChannelNumber() >= ( m_groupNumber * CHANNELS_IN_GROUP ) ) &&
             ( channel->getChannelNumber() <
               ( ( m_groupNumber + 1 ) * CHANNELS_IN_GROUP ) ) ) {
            if( m_channels[ channel->getChannelNumber() -
                            ( m_groupNumber * CHANNELS_IN_GROUP ) ] ) {
               m_logger << msg::WARNING
                        << tr( "Redefining channel number: %1" )
                  .arg( channel->getChannelNumber() )
                        << msg::endmsg;
               delete m_channels[ channel->getChannelNumber() -
                                  ( m_groupNumber * CHANNELS_IN_GROUP ) ];
            }
            m_channels[ channel->getChannelNumber() -
                        ( m_groupNumber * CHANNELS_IN_GROUP ) ] = channel;
         } else {
            REPORT_ERROR( tr( "There was a problem reading the configuration "
                              "of one channel" ) );
            delete channel;
         }
      }

      return true;
   }

   bool GroupConfig::writeConfig( QIODevice* dev ) const {

      REPORT_VERBOSE( tr( "Writing configuration to binary output" ) );

      QDataStream output( dev );
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
            if( ! m_channels[ i ]->writeConfig( dev ) ) {
               REPORT_ERROR( tr( "A problem happened while writing out a "
                                 "channel configuration" ) );
               return false;
            }
         }
      }

      return true;
   }

   bool GroupConfig::readConfig( const QDomElement& node ) {

      REPORT_VERBOSE( tr( "Reading configuration from XML input" ) );

      bool ok;

      //
      // Read in the group-wide configurations:
      //
      m_groupNumber = node.attribute( "Number", "0" ).toInt( &ok );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading a "
                           "\"group number\" value" ) );
         return false;
      }

      m_trigThreshold = node.attribute( "TrigThreshold", "0" ).toInt( &ok );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading a "
                           "\"trigger threshold\" value" ) );
         return false;
      }

      m_dcOffset = node.attribute( "DCOffset", "0" ).toInt( &ok );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading a "
                           "\"DC offset\" value" ) );
         return false;
      }

      m_trigMask = node.attribute( "TrigMask", "0" ).toUInt( &ok );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading a "
                           "\"trigger mask\" value" ) );
         return false;
      }

      m_trigEnabled = node.attribute( "TrigEnabled", "0" ).toInt( &ok );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading a "
                           "\"trigger enabled\" value" ) );
         return false;
      }

      m_trigOutEnabled = node.attribute( "TrigOutEnabled",
                                         "0" ).toInt( &ok );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading a "
                           "\"trigger output enabled\" value" ) );
         return false;
      }

      //
      // Configure the channels:
      //
      for( int i = 0; i < node.childNodes().size(); ++i ) {

         // Only process "Channel" type child-nodes:
         if( node.childNodes().at( i ).nodeName() != "Channel" ) {
            continue;
         }

         ChannelConfig* channel = new ChannelConfig();
         if( ! channel->readConfig( node.childNodes().at( i ).toElement() ) ) {
            REPORT_ERROR( tr( "The configuration of a channel couldn't be "
                              "read!" ) );
            delete channel;
            return false;
         }
         if( ( channel->getChannelNumber() >= ( m_groupNumber * CHANNELS_IN_GROUP ) ) &&
             ( channel->getChannelNumber() <
               ( ( m_groupNumber + 1 ) * CHANNELS_IN_GROUP ) ) ) {
            if( m_channels[ channel->getChannelNumber() -
                            ( m_groupNumber * CHANNELS_IN_GROUP ) ] ) {
               m_logger << msg::WARNING
                        << tr( "Redefining channel number: %1" )
                  .arg( channel->getChannelNumber() )
                        << msg::endmsg;
               delete m_channels[ channel->getChannelNumber() -
                                  ( m_groupNumber * CHANNELS_IN_GROUP ) ];
            }
            m_channels[ channel->getChannelNumber() -
                        ( m_groupNumber * CHANNELS_IN_GROUP ) ] = channel;
         } else {
            REPORT_ERROR( tr( "There was a problem reading the configuration "
                              "of one channel" ) );
            delete channel;
            return false;
         }
      }

      return true;
   }

   bool GroupConfig::writeConfig( QDomElement& node ) const {

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
            if( ! m_channels[ i ]->writeConfig( ch_element ) ) {
               REPORT_ERROR( tr( "A problem happened while writing out a "
                                 "channel configuration" ) );
               return false;
            }
            node.appendChild( ch_element );

         }
      }

      return true;
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
         if( ! m_channels[ channel ] ) {
            m_channels[ channel ] = new ChannelConfig();
            m_channels[ channel ]->setChannelNumber( channel +
                                                     m_groupNumber *
                                                     CHANNELS_IN_GROUP );
         }
      } else {
         if( m_channels[ channel ] ) {
            delete m_channels[ channel ];
            m_channels[ channel ] = 0;
         }
      }

      return;
   }

   ChannelConfig* GroupConfig::getChannel( int channel ) {

      if( ( channel >= 0 ) &&
          ( channel < CHANNELS_IN_GROUP ) ) {
         return m_channels[ channel ];
      }

      REPORT_ERROR( tr( "Channel with invalid index (%1) requested" )
                    .arg( channel ) );
      return 0;
   }

   const ChannelConfig* GroupConfig::getChannel( int channel ) const {

      if( ( channel >= 0 ) &&
          ( channel < CHANNELS_IN_GROUP ) ) {
         return m_channels[ channel ];
      }

      REPORT_ERROR( tr( "Channel with invalid index (%1) requested" ).arg( channel ) );
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
         if( m_channels[ i ] ) delete m_channels[ i ];
         m_channels[ i ] = 0;
      }

      return;
   }

} // namespace dt5740
