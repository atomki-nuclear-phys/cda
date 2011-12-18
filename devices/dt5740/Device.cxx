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
#   include "cdacore/common/errorcheck.h"
#else
#   include "event/Fragment.h"
#   include "common/errorcheck.h"
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
        m_acqMode( caen::Digitizer::ACQ_SW_Controlled ),
        m_saveRawNtuple( false ),
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
      input >> ctype;
      m_acqMode = caen::Digitizer::convertAcqMode( ctype );
      input >> m_saveRawNtuple;

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
      output << caen::Digitizer::convertAcqMode( m_acqMode );
      output << m_saveRawNtuple;

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

      m_acqMode = caen::Digitizer::convertAcqMode( element.attribute( "AcqMode",
                                                                      "0" ).toInt( &ok ) );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading an "
                           "\"acquisition mode\" value" ) );
         return false;
      }

      m_saveRawNtuple = element.attribute( "SaveRawNtuple",
                                           "0" ).toInt( &ok );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading a "
                           "\"save RAW ntuple\" value" ) );
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
      element.setAttribute( "AcqMode",
                            caen::Digitizer::convertAcqMode( m_acqMode ) );
      element.setAttribute( "SaveRawNtuple", m_saveRawNtuple );

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

      unsigned int result = 0;
      switch( m_connType ) {

      case caen::Digitizer::USB:
         result = 0;
         break;
      case caen::Digitizer::PCI_OpticalLink:
         result = 1000;
         break;
      case caen::Digitizer::PCIE_OpticalLink:
         result = 2000;
         break;
      case caen::Digitizer::PCIE_EmbeddedDigitizer:
         result = 3000;
         break;
      default:
         result = 0;
         break;
      }

      result += m_linkNumber;

      return result;
   }

   void Device::setID( unsigned int ) {

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
    * an event fragment into easier-to-use structures.
    *
    * @param fragment The encoded information
    * @param ei The decoded event information
    * @param ed The decoded event data
    * @returns <code>true</code> if the decoding was successful,
    *          <code>false</code> otherwise
    */
   bool Device::decode( const ev::Fragment& fragment,
                        caen::Digitizer::EventInfo& ei,
                        caen::Digitizer::EventData16Bit& ed ) const {

      // Calculate the number of active channels:
      size_t channels = 0;
      for( int group = 0; group < NUMBER_OF_GROUPS; ++group ) {
         for( int channel = 0; channel < GroupConfig::CHANNELS_IN_GROUP;
              ++channel ) {
            if( m_groups[ group ].getChannel( channel ) ) {
               ++channels;
            }
         }
      }

      // The required size of the fragment:
      const size_t size =
         6 +                              // Event info words
         ( channels * getSamples() / 2 ); // Samples for each channel

      // Check if the fragment is if the expected size:
      CHECK( fragment.getDataWords().size() == size );

      // Easy access to the data words:
      const std::vector< uint32_t >& data = fragment.getDataWords();

      // Set the event information:
      ei.eventSize      = data[ 0 ];
      ei.boardId        = data[ 1 ];
      ei.pattern        = data[ 2 ];
      ei.channelMask    = data[ 3 ];
      ei.eventCounter   = data[ 4 ];
      ei.triggerTimeTag = data[ 5 ];

      // Set the sample information for each active channel:
      int index = 6;
      bool low_bits = true;
      for( int group = 0; group < NUMBER_OF_GROUPS; ++group ) {
         for( int channel = 0; channel < GroupConfig::CHANNELS_IN_GROUP;
              ++channel ) {

            // Index of this channel:
            const int chIndex = group * GroupConfig::CHANNELS_IN_GROUP + channel;

            // Clear the data in the inactive channels:
            if( ! m_groups[ group ].getChannel( channel ) ) {
               ed.chData[ chIndex ].clear();
               continue;
            }

            // Fill the event data for this channel:
            ed.chData[ chIndex ].resize( getSamples(), 0 );
            for( int sample = 0; sample < getSamples();
                 ++sample ) {
               if( low_bits ) {
                  ed.chData[ chIndex ][ sample ] = data[ index ] & 0xffff;
                  low_bits = false;
               } else {
                  ed.chData[ chIndex ][ sample ] = ( data[ index ] >> 16 ) & 0xffff;
                  low_bits = true;
                  ++index;
               }
            }
         }
      }

      return true;
   }

   /**
    * This function is used by the Readout class to place the device's data
    * into an event fragment.
    *
    * @param ei The event information
    * @param ed The event data
    * @param fragment The encoded event information
    * @returns <code>true</code> if the decoding was successful,
    *          <code>false</code> otherwise
    */
   bool Device::encode( const caen::Digitizer::EventInfo& ei,
                        const caen::Digitizer::EventData16Bit& ed,
                        ev::Fragment& fragment ) const {

      // Clear the data words, but leave the module ID alone:
      fragment.clear( false );

      // Put the event information into the fragment:
      fragment.addDataWord( ei.eventSize );
      fragment.addDataWord( ei.boardId );
      fragment.addDataWord( ei.pattern );
      fragment.addDataWord( ei.channelMask );
      fragment.addDataWord( ei.eventCounter );
      fragment.addDataWord( ei.triggerTimeTag );

      // Now place all the channel data into the fragment:
      bool low_bits = true;
      ev::Fragment::Payload_t::value_type dataWord = 0;
      for( int group = 0; group < NUMBER_OF_GROUPS; ++group ) {
         for( int channel = 0; channel < GroupConfig::CHANNELS_IN_GROUP;
              ++channel ) {

            // Skip inactive channels:
            if( ! m_groups[ group ].getChannel( channel ) ) {
               continue;
            }

            // Index of this channel:
            const int chIndex = group * GroupConfig::CHANNELS_IN_GROUP + channel;

            // A security check:
            CHECK( static_cast< int >( ed.chData[ chIndex ].size() ) ==
                   getSamples() );

            // Add the data from this channel to the fragment:
            for( int sample = 0; sample < getSamples(); ++sample ) {
               if( low_bits ) {
                  dataWord = 0;
                  dataWord |= ed.chData[ chIndex ][ sample ] & 0xffff;
                  low_bits = false;
               } else {
                  dataWord |= ( ed.chData[ chIndex ][ sample ] << 16 ) & 0xffff0000;
                  fragment.addDataWord( dataWord );
                  low_bits = true;
               }
            }
         }
      }

      // If a sample was set, but not added to the payload, let's do that here:
      if( ! low_bits ) {
         fragment.addDataWord( dataWord );
      }

      return true;
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
