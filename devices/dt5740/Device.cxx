// $Id$

// System include(s):
#include <cmath>

// Qt include(s):
#include <QtCore/QIODevice>
#include <QtCore/QDataStream>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/event/Fragment.h"
#else
#   include "event/Fragment.h"
#endif

// Local include(s):
#include "Device.h"

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

   Device::Device()
      : m_logger( "dt5740::Device" ) {

      // Set the ID of each group:
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {
         m_groups[ i ].setGroupNumber( i );
      }

      // Reset all the pointers in the array:
      for( int i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
         m_channels[ i ] = 0;
      }
   }

   Device::~Device() {

      clear();
   }

   bool Device::readConfig( QIODevice* dev ) {

      REPORT_VERBOSE( tr( "Reading configuration from binary input" ) );

      clear();

      // Read the properties of this class:
      QDataStream input( dev );
      input.setVersion( QDataStream::Qt_4_0 );

      // Read the connection parameters of the device:
      int ctype;
      input >> ctype;
      m_connType = caen::Digitizer::convertConnType( ctype );
      input >> m_linkNumber;

      // Read the number of active channels:
      quint32 number_of_channels;
      input >> number_of_channels;

      REPORT_VERBOSE( tr( "Number of configured channels: %1" )
                      .arg( number_of_channels ) );

      // Read in the configuration of the groups:
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {
         if( ! m_groups[ i ].readConfig( dev ) ) {
            REPORT_ERROR( tr( "The configuration of a group couldn't be "
                              "read!" ) );
            return false;
         }
         if( m_groups[ i ].getGroupNumber() != i ) {
            REPORT_ERROR( tr( "There was an error reading the configuration "
                              "of a group" ) );
            return false;
         }
      }

      // Read in the configuration of all the channels:
      for( quint32 i = 0; i < number_of_channels; ++i ) {
         ChannelConfig* channel = new ChannelConfig();
         if( ! channel->readConfig( dev ) ) {
            REPORT_ERROR( tr( "The configuration of a channel couldn't be "
                              "read!" ) );
            delete channel;
            return false;
         }
         if( ( channel->getChannelNumber() >= 0 ) &&
             ( channel->getChannelNumber() < NUMBER_OF_CHANNELS ) ) {
            if( m_channels[ channel->getChannelNumber() ] ) {
               m_logger << msg::WARNING
                        << tr( "Redefining channel number: %1" )
                  .arg( channel->getChannelNumber() )
                        << msg::endmsg;
               delete m_channels[ channel->getChannelNumber() ];
            }
            m_channels[ channel->getChannelNumber() ] = channel;
         } else {
            REPORT_ERROR( tr( "There was a problem reading the configuration "
                              "of one channel" ) );
            delete channel;
         }
      }

      return true;
   }

   bool Device::writeConfig( QIODevice* dev ) const {

      REPORT_VERBOSE( tr( "Writing configuration to binary output" ) );

      // Write the properties of this class:
      QDataStream output( dev );
      output.setVersion( QDataStream::Qt_4_0 );

      // Write out the connection parameters of the device:
      output << caen::Digitizer::convertConnType( m_connType );
      output << m_linkNumber;

      // Count the number of configured channels:
      quint32 number_of_channels = 0;
      for( int i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
         if( m_channels[ i ] ) ++number_of_channels;
      }

      // Write the number of channels to follow:
      output << number_of_channels;

      // Write out the group configurations:
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {
         if( ! m_groups[ i ].writeConfig( dev ) ) {
            REPORT_ERROR( tr( "A problem happened while writing out a "
                              "group's configuration" ) );
            return false;
         }
      }

      // Write the channel configurations:
      for( int i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
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

   bool Device::readConfig( const QDomElement& element ) {

      REPORT_VERBOSE( tr( "Reading configuration from XML input" ) );

      clear();

      //
      // Read the device wide configuration:
      //
      bool ok;

      const int ctype = element.attribute( "ConnType", "0" ).toInt( &ok );
      if( ! ok ) {
         REPORT_ERROR( tr( "Couldn't read connection type" ) );
         return false;
      }
      m_connType = caen::Digitizer::convertConnType( ctype );

      m_linkNumber = element.attribute( "LinkNumber", "0" ).toInt( &ok );
      if( ! ok ) {
         REPORT_ERROR( tr( "Couldn't read link number" ) );
         return false;
      }

      //
      // Configure the groups:
      //
      for( int i = 0; i < element.childNodes().size(); ++i ) {

         // Only process "Group" type child-nodes:
         if( element.childNodes().at( i ).nodeName() != "Group" ) {
            continue;
         }

         GroupConfig group;
         if( ! group.readConfig( element.childNodes().at( i ).toElement() ) ) {
            REPORT_ERROR( tr( "The configuration of a group couldn't be "
                              "read" ) );
            return false;
         }
         if( ( group.getGroupNumber() >= 0 ) &&
             ( group.getGroupNumber() < NUMBER_OF_GROUPS ) ) {
            m_groups[ group.getGroupNumber() ] = group;
         } else {
            REPORT_ERROR( tr( "There was an error reading the configuration "
                              "of a group" ) );
            return false;
         }
      }

      //
      // Configure the channels:
      //
      for( int i = 0; i < element.childNodes().size(); ++i ) {

         // Only process "Channel" type child-nodes:
         if( element.childNodes().at( i ).nodeName() != "Channel" ) {
            continue;
         }

         ChannelConfig* channel = new ChannelConfig();
         if( ! channel->readConfig( element.childNodes().at( i ).toElement() ) ) {
            REPORT_ERROR( tr( "The configuration of a channel couldn't be "
                              "read!" ) );
            delete channel;
            return false;
         }
         if( ( channel->getChannelNumber() >= 0 ) &&
             ( channel->getChannelNumber() < NUMBER_OF_CHANNELS ) ) {
            if( m_channels[ channel->getChannelNumber() ] ) {
               m_logger << msg::WARNING
                        << tr( "Redefining channel number: %1" )
                  .arg( channel->getChannelNumber() )
                        << msg::endmsg;
               delete m_channels[ channel->getChannelNumber() ];
            }
            m_channels[ channel->getChannelNumber() ] = channel;
         } else {
            REPORT_ERROR( tr( "There was a problem reading the configuration "
                              "of one channel" ) );
            delete channel;
            return false;
         }
      }

      return true;
   }

   bool Device::writeConfig( QDomElement& element ) const {

      REPORT_VERBOSE( tr( "Writing configuration to XML output" ) );

      //
      // Write the device wide configuration:
      //
      element.setAttribute( "ConnType",
                            caen::Digitizer::convertConnType( m_connType ) );
      element.setAttribute( "LinkNumber", m_linkNumber );

      //
      // Create a new node for the configuration of each group:
      //
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {

         QDomElement gr_element =
            element.ownerDocument().createElement( "Group" );
         if( ! m_groups[ i ].writeConfig( gr_element ) ) {
            REPORT_ERROR( tr( "A problem happened while writing out a "
                              "group's configuration" ) );
            return false;
         }
         element.appendChild( gr_element );
      }

      //
      // Create a new node for the configuration of each channel:
      //
      for( int i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
         if( m_channels[ i ] ) {

            QDomElement ch_element =
               element.ownerDocument().createElement( "Channel" );
            if( ! m_channels[ i ]->writeConfig( ch_element ) ) {
               REPORT_ERROR( tr( "A problem happened while writing out a "
                                 "channel configuration" ) );
               return false;
            }
            element.appendChild( ch_element );

         }
      }

      return true;
   }

   unsigned int Device::getID() const {

      return m_linkNumber;
   }

   void Device::setID( unsigned int id ) {

      m_linkNumber = id;
      return;
   }

   void Device::clear() {

      for( int i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
         if( m_channels[ i ] ) delete m_channels[ i ];
         m_channels[ i ] = 0;
      }

      return;
   }

   Device::Data_t Device::decode( const ev::Fragment& fragment ) const {

      // The result object:
      Data_t result;
      result.resize( NUMBER_OF_CHANNELS );

      // The maximum number of samples requested by any of the groups:
      const int max_samples = std::max( std::max( m_groups[ 0 ].getSamples(),
                                                  m_groups[ 1 ].getSamples() ),
                                        std::max( m_groups[ 2 ].getSamples(),
                                                  m_groups[ 3 ].getSamples() ) );

      // The first bit of the next data word:
      int bit_number = 0;

      // Loop over the samples:
      for( int sample = 0; sample < max_samples; ++sample ) {
         // Loop over the groups:
         for( int group = 0; group < NUMBER_OF_GROUPS; ++group ) {
            // Check if this group is still active for this sample:
            if( sample >= m_groups[ group ].getSamples() ) {
               continue;
            }
            // Loop over the channels of the group:
            for( int channel = 0; channel < GroupConfig::CHANNELS_IN_GROUP;
                 ++channel ) {
               // The absolute channel number of this channel:
               const int ch_number = group * GroupConfig::CHANNELS_IN_GROUP + channel;
               // Check if the channel is configured:
               if( ! m_channels[ ch_number ] ) {
                  continue;
               }
               // The data of this channel in this sample:
               unsigned int ch_data = 0;
               // Fill the data bit-by-bit:
               for( int abit = bit_number, i = 0; abit < bit_number + BITS_PER_CHANNEL;
                    ++abit, ++i ) {
                  const int word = abit / 32;
                  const int bit  = abit % 32;
                  // A security check:
                  if( word >= static_cast< int >( fragment.getDataWords().size() ) ) {
                     REPORT_ERROR( tr( "The received data fragment is too small!" ) );
                     return result;
                  }
                  if( ( fragment.getDataWords()[ word ] >> bit ) & 0x1 ) {
                     ch_data |= ( 0x1 << i );
                  }
               } // end of loop over bits
               // Add this data word to the result:
               result[ ch_number ].push_back( ch_data );
            } // end of loop over channels in group
         } // end of loop over groups
      } // end of loop over samples

      return result;
   }

} // namespace dt5740
