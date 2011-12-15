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
      : m_connType( caen::Digitizer::USB ), m_linkNumber( 0 ),
        m_trigMode( TriggerOnInputOverThreshold ), 
        m_trigOvlpEnabled( false ), m_patGenEnabled( false ),
        m_gateMode( WindowGate ), m_bufferMode( NBuffers1 ),
        m_postTrigPercentage( 0 ), m_extTrigEnabled( false ),
        m_extTrigOutEnabled( false ),
        m_logger( "dt5740::Device" ) {

      // Set the ID of each group:
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {
         m_groups[ i ].setGroupNumber( i );
      }
   }

   Device::~Device() {

      clear();
   }

   bool Device::readConfig( QIODevice* dev ) {

      REPORT_VERBOSE( tr( "Reading configuration from binary input" ) );

      clear();

      unsigned int temp = 0;

      // Read the properties of this class:
      QDataStream input( dev );
      input.setVersion( QDataStream::Qt_4_0 );

      // Read the connection parameters of the device:
      int ctype;
      input >> ctype;
      m_connType = caen::Digitizer::convertConnType( ctype );
      input >> m_linkNumber;
      input >> temp;
      m_trigMode = toTriggerMode( temp );
      input >> m_trigOvlpEnabled;
      input >> m_patGenEnabled;
      input >> temp;
      m_gateMode = toGateMode( temp );
      input >> temp;
      m_bufferMode = toBufferMode( temp );
      input >> m_postTrigPercentage;
      input >> m_extTrigEnabled;
      input >> m_extTrigOutEnabled;

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
      output << toUInt( m_trigMode );
      output << m_trigOvlpEnabled;
      output << m_patGenEnabled;
      output << toUInt( m_gateMode );
      output << toUInt( m_bufferMode );
      output << m_postTrigPercentage; 
      output << m_extTrigEnabled;
      output << m_extTrigOutEnabled;

      // Write out the group configurations:
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {
         if( ! m_groups[ i ].writeConfig( dev ) ) {
            REPORT_ERROR( tr( "A problem happened while writing out a "
                              "group's configuration" ) );
            return false;
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

      m_trigMode = toTriggerMode( element.attribute( "TrigMode",
                                                     "0" ).toUInt( &ok ) );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading a "
                           "\"trigger mode\" value" ) );
         return false;
      }

      m_trigOvlpEnabled = element.attribute( "TrigOvlpEnabled", "0" ).toInt( &ok );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading a "
                           "\"trigger overlap enabled\" value" ) );
         return false;
      }

      m_patGenEnabled = element.attribute( "PatGenEnabled", "0" ).toInt( &ok );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading a "
                           "\"pattern generation enabled\" value" ) );
         return false;
      }

      m_gateMode = toGateMode( element.attribute( "GateMode",
                                                  "0" ).toUInt( &ok ) );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading a "
                           "\"gate mode\" value" ) );
         return false;
      }

      m_bufferMode = toBufferMode( element.attribute( "BufferMode",
                                                      "0" ).toUInt( &ok ) );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading a "
                           "\"buffer mode\" value" ) );
         return false;
      }

      m_postTrigPercentage = element.attribute( "PostTrigPercentage",
                                                "0" ).toInt( &ok );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading a "
                           "\"post trigger percentage\" value" ) );
         return false;
      }

      m_extTrigEnabled = element.attribute( "ExtTrigEnabled",
                                            "0" ).toInt( &ok );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading an "
                           "\"external trigger enabled\" value" ) );
         return false;
      }

      m_extTrigOutEnabled = element.attribute( "ExtTrigOutEnabled",
                                               "0" ).toInt( &ok );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading an "
                           "\"external trigger output enabled\" value" ) );
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
      element.setAttribute( "TrigMode", toUInt( m_trigMode ) );
      element.setAttribute( "TrigOvlpEnabled", m_trigOvlpEnabled );
      element.setAttribute( "PatGenEnabled", m_patGenEnabled );
      element.setAttribute( "GateMode", toUInt( m_gateMode ) );
      element.setAttribute( "BufferMode", toUInt( m_bufferMode ) );
      element.setAttribute( "PostTrigPercentage", m_postTrigPercentage );
      element.setAttribute( "ExtTrigEnabled", m_extTrigEnabled );
      element.setAttribute( "ExtTrigOutEnabled", m_extTrigOutEnabled ); 

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

      // Reset the simple properties:
      m_connType = caen::Digitizer::USB;
      m_linkNumber = 0;

      // Clear all the groups:
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {
         m_groups[ i ].clear();
      }

      return;
   }

   int Device::getSamples() const {

      switch( m_bufferMode ) {

      case NBuffers1:
         return 196608;
         break;
      case NBuffers2:
         return 98304;
         break;
      case NBuffers4:
         return 49152;
         break;
      case NBuffers8:
         return 24576;
         break;
      case NBuffers16:
         return 12288;
         break;
      case NBuffers32:
         return 6144;
         break;
      case NBuffers64:
         return 3072;
         break;
      case NBuffers128:
         return 1536;
         break;
      case NBuffers256:
         return 768;
         break;
      case NBuffers512:
         return 384;
         break;
      case NBuffers1024:
         return 192;
         break;
      default:
         REPORT_ERROR( tr( "Buffer mode (%1) not recognized" )
                       .arg( m_bufferMode ) );
         break;
      }

      return 0;
   } 

   /**
    * This function is used by the derived classes to decode the content
    * of the data words read from the device's memory during data
    * taking.
    *
    * @param fragment The raw data words coming from the device
    * @returns The data in an easier-to-digest format
    */
   Device::Data_t Device::decode( const ev::Fragment& fragment ) const {

      // The result object:
      Data_t result;
      result.resize( NUMBER_OF_GROUPS * GroupConfig::CHANNELS_IN_GROUP );

      // The first bit of the next data word. Remember that the first
      // 4 words belong to the header of the event:
      int bit_number = 128;

      // According to the documentation the readout format is as follows:
      //  - First we get all the samples for the first channel of the first
      //    group.
      //  - Next we get all the samples of the second channel of the first
      //    group.
      //  - etc.

      // Loop over the groups:
      for( int group = 0; group < NUMBER_OF_GROUPS; ++group ) {
         // Loop over the channels of the group:
         for( int channel = 0; channel < GroupConfig::CHANNELS_IN_GROUP;
              ++channel ) {
            // Skip the inactive channels:
            if( ! m_groups[ group ].getChannel( channel ) ) continue;
            // Reserve the needed space in the inner vector:
            result[ ( group * GroupConfig::CHANNELS_IN_GROUP ) +
                    channel ].resize( getSamples() );
            // Loop over all the samples:
            for( int sample = 0; sample < getSamples(); ++sample ) {
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
               result[ ( group * GroupConfig::CHANNELS_IN_GROUP ) +
                       channel ].push_back( ch_data );
               // Increment the bit position:
               bit_number += BITS_PER_CHANNEL;
            } // end of loop over samples
         } // end of loop over channels in group
      } // end of loop over groups

      return result;
   }

   unsigned int Device::toUInt( Device::TriggerMode mode ) const {

      switch( mode ) {

      case TriggerOnInputOverThreshold:
         return 0;
         break;
      case TriggerOnInputUnderThreshold:
         return 1;
         break;
      default:
         REPORT_ERROR( tr( "Trigger mode (%1) not recognized" ).arg( mode ) );
         break;
      }

      return 0;
   }

   unsigned int Device::toUInt( Device::GateMode mode ) const {

      switch( mode ) {

      case WindowGate:
         return 0;
         break;
      case SingleShotGate:
         return 1;
         break;
      default:
         REPORT_ERROR( tr( "Gate mode (%1) not recognized" ).arg( mode ) );
         break;
      }

      return 0;
   }

   unsigned int Device::toUInt( Device::BufferMode mode ) const {

      switch( mode ) {

      case NBuffers1:
         return 0;
         break;
      case NBuffers2:
         return 1;
         break;
      case NBuffers4:
         return 2;
         break;
      case NBuffers8:
         return 3;
         break;
      case NBuffers16:
         return 4;
         break;
      case NBuffers32:
         return 5;
         break;
      case NBuffers64:
         return 6;
         break;
      case NBuffers128:
         return 7;
         break;
      case NBuffers256:
         return 8;
         break;
      case NBuffers512:
         return 9;
         break;
      case NBuffers1024:
         return 10;
         break;
      default:
         REPORT_ERROR( tr( "Buffer mode (%1) not recognized" ).arg( mode ) );
         break;
      }

      return 0;
   }

   Device::TriggerMode
   Device::toTriggerMode( unsigned int value ) const {

      switch( value ) {

      case 0:
         return TriggerOnInputOverThreshold;
         break;
      case 1:
         return TriggerOnInputUnderThreshold;
         break;
      default:
         REPORT_ERROR( tr( "Trigger mode (%1) not recognized" ).arg( value ) );
         break;
      }

      return TriggerOnInputOverThreshold;
   }

   Device::GateMode
   Device::toGateMode( unsigned int value ) const {

      switch( value ) {

      case 0:
         return WindowGate;
         break;
      case 1:
         return SingleShotGate;
         break;
      default:
         REPORT_ERROR( tr( "Gate mode (%1) not recognized" ).arg( value ) );
         break;
      }

      return WindowGate;
   }

   Device::BufferMode
   Device::toBufferMode( unsigned int value ) const {

      switch( value ) {

      case 0:
         return NBuffers1;
         break;
      case 1:
         return NBuffers2;
         break;
      case 2:
         return NBuffers4;
         break;
      case 3:
         return NBuffers8;
         break;
      case 4:
         return NBuffers16;
         break;
      case 5:
         return NBuffers32;
         break;
      case 6:
         return NBuffers64;
         break;
      case 7:
         return NBuffers128;
         break;
      case 8:
         return NBuffers256;
         break;
      case 9:
         return NBuffers512;
         break;
      case 10:
         return NBuffers1024;
         break;
      default:
         REPORT_ERROR( tr( "Buffer mode (%1) not recognized" ).arg( value ) );
         break;
      }

      return NBuffers1;
   } 

} // namespace dt5740
