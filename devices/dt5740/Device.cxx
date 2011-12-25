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
        m_trigMode( TRG_InputOverThreshold ), 
        m_trigOvlpEnabled( false ), m_patGenEnabled( false ),
        m_gateMode( GATE_Window ), m_bufferMode( BUFF_NBuffers1024 ),
        m_postTrigPercentage( 0 ), m_extTrigEnabled( false ),
        m_extTrigOutEnabled( false ),
        m_clockSource( CLK_Internal ),
        m_evCountMode( EV_CountAcceptedTriggers ),
        m_signalType( SGNL_NIM ),
        m_highImpedanceGPO( false ),
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

      QString temp;

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
      input >> temp;
      m_clockSource = toClockSource( temp );
      input >> temp;
      m_evCountMode = toEvCountMode( temp );
      input >> temp;
      m_signalType = toSignalType( temp );
      input >> m_highImpedanceGPO;

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
      output << toString( m_trigMode );
      output << m_trigOvlpEnabled;
      output << m_patGenEnabled;
      output << toString( m_gateMode );
      output << toString( m_bufferMode );
      output << m_postTrigPercentage; 
      output << m_extTrigEnabled;
      output << m_extTrigOutEnabled;
      output << caen::Digitizer::convertAcqMode( m_acqMode );
      output << m_saveRawNtuple;
      output << toString( m_clockSource );
      output << toString( m_evCountMode );
      output << toString( m_signalType );
      output << m_highImpedanceGPO;

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
      CHECK( ok );
      m_connType = caen::Digitizer::convertConnType( ctype );

      m_linkNumber = element.attribute( "LinkNumber", "0" ).toInt( &ok );
      CHECK( ok );

      m_trigMode = toTriggerMode( element.attribute( "TrigMode",
                                                     "InputOverThreshold" ) );

      m_trigOvlpEnabled = element.attribute( "TrigOvlpEnabled", "0" ).toInt( &ok );
      CHECK( ok );

      m_patGenEnabled = element.attribute( "PatGenEnabled", "0" ).toInt( &ok );
      CHECK( ok );

      m_gateMode = toGateMode( element.attribute( "GateMode",
                                                  "Window" ) );

      m_bufferMode = toBufferMode( element.attribute( "BufferMode",
                                                      "NBuffers1" ) );

      m_postTrigPercentage = element.attribute( "PostTrigPercentage",
                                                "0" ).toInt( &ok );
      CHECK( ok );

      m_extTrigEnabled = element.attribute( "ExtTrigEnabled",
                                            "0" ).toInt( &ok );
      CHECK( ok );

      m_extTrigOutEnabled = element.attribute( "ExtTrigOutEnabled",
                                               "0" ).toInt( &ok );
      CHECK( ok );

      m_acqMode =
         caen::Digitizer::convertAcqMode( element.attribute( "AcqMode",
                                                             "0" ).toInt( &ok ) );
      CHECK( ok );

      m_saveRawNtuple = element.attribute( "SaveRawNtuple",
                                           "0" ).toInt( &ok );
      CHECK( ok );

      m_clockSource = toClockSource( element.attribute( "ClockSource",
                                                        "Internal" ) );

      m_evCountMode = toEvCountMode( element.attribute( "EvCountMode",
                                                        "AcceptedTriggers" ) );

      m_signalType = toSignalType( element.attribute( "SignalType",
                                                      "NIM" ) );

      m_highImpedanceGPO = element.attribute( "HighImpedanceGPO",
                                              "0" ).toInt( &ok );
      CHECK( ok );

      //
      // Configure the groups:
      //
      for( int i = 0; i < element.childNodes().size(); ++i ) {

         // Only process "Group" type child-nodes:
         if( element.childNodes().at( i ).nodeName() != "Group" ) {
            continue;
         }

         GroupConfig group;
         CHECK( group.readConfig( element.childNodes().at( i ).toElement() ) );
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
      element.setAttribute( "TrigMode", toString( m_trigMode ) );
      element.setAttribute( "TrigOvlpEnabled", m_trigOvlpEnabled );
      element.setAttribute( "PatGenEnabled", m_patGenEnabled );
      element.setAttribute( "GateMode", toString( m_gateMode ) );
      element.setAttribute( "BufferMode", toString( m_bufferMode ) );
      element.setAttribute( "PostTrigPercentage", m_postTrigPercentage );
      element.setAttribute( "ExtTrigEnabled", m_extTrigEnabled );
      element.setAttribute( "ExtTrigOutEnabled", m_extTrigOutEnabled );
      element.setAttribute( "AcqMode",
                            caen::Digitizer::convertAcqMode( m_acqMode ) );
      element.setAttribute( "SaveRawNtuple", m_saveRawNtuple );
      element.setAttribute( "ClockSource", toString( m_clockSource ) );
      element.setAttribute( "EvCountMode", toString( m_evCountMode ) );
      element.setAttribute( "SignalType", toString( m_signalType ) );
      element.setAttribute( "HighImpedanceGPO", m_highImpedanceGPO );

      //
      // Create a new node for the configuration of each group:
      //
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {

         QDomElement gr_element =
            element.ownerDocument().createElement( "Group" );
         CHECK( m_groups[ i ].writeConfig( gr_element ) );
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

      // Reset the device wide properties:
      m_trigMode = TRG_InputOverThreshold;
      m_trigOvlpEnabled = false;
      m_patGenEnabled = false;
      m_gateMode = GATE_Window;
      m_bufferMode = BUFF_NBuffers1024;
      m_postTrigPercentage = 50;
      m_extTrigEnabled = false;
      m_extTrigOutEnabled = false;
      m_clockSource = CLK_Internal;
      m_evCountMode = EV_CountAcceptedTriggers;
      m_signalType = SGNL_NIM;
      m_highImpedanceGPO = false;

      // Clear all the groups:
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {
         m_groups[ i ].clear();
      }

      return;
   }

   int Device::getSamples() const {

      switch( m_bufferMode ) {

      case BUFF_NBuffers1:
         return 196608;
         break;
      case BUFF_NBuffers2:
         return 98304;
         break;
      case BUFF_NBuffers4:
         return 49152;
         break;
      case BUFF_NBuffers8:
         return 24576;
         break;
      case BUFF_NBuffers16:
         return 12288;
         break;
      case BUFF_NBuffers32:
         return 6144;
         break;
      case BUFF_NBuffers64:
         return 3072;
         break;
      case BUFF_NBuffers128:
         return 1536;
         break;
      case BUFF_NBuffers256:
         return 768;
         break;
      case BUFF_NBuffers512:
         return 384;
         break;
      case BUFF_NBuffers1024:
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

   QString Device::toString( Device::TriggerMode mode ) const {

      switch( mode ) {

      case TRG_InputOverThreshold:
         return "InputOverThreshold";
         break;
      case TRG_InputUnderThreshold:
         return "InputUnderThreshold";
         break;
      default:
         REPORT_ERROR( tr( "Trigger mode (%1) not recognized" ).arg( mode ) );
         break;
      }

      return "UnknownTriggerMode";
   }

   QString Device::toString( Device::GateMode mode ) const {

      switch( mode ) {

      case GATE_Window:
         return "Window";
         break;
      case GATE_SingleShot:
         return "SingleShot";
         break;
      default:
         REPORT_ERROR( tr( "Gate mode (%1) not recognized" ).arg( mode ) );
         break;
      }

      return "UnknownGateMode";
   }

   QString Device::toString( Device::BufferMode mode ) const {

      switch( mode ) {

      case BUFF_NBuffers1:
         return "NBuffers1";
         break;
      case BUFF_NBuffers2:
         return "NBuffers2";
         break;
      case BUFF_NBuffers4:
         return "NBuffers4";
         break;
      case BUFF_NBuffers8:
         return "NBuffers8";
         break;
      case BUFF_NBuffers16:
         return "NBuffers16";
         break;
      case BUFF_NBuffers32:
         return "NBuffers32";
         break;
      case BUFF_NBuffers64:
         return "NBuffers64";
         break;
      case BUFF_NBuffers128:
         return "NBuffers128";
         break;
      case BUFF_NBuffers256:
         return "NBuffers256";
         break;
      case BUFF_NBuffers512:
         return "NBuffers512";
         break;
      case BUFF_NBuffers1024:
         return "NBuffers1024";
         break;
      default:
         REPORT_ERROR( tr( "Buffer mode (%1) not recognized" ).arg( mode ) );
         break;
      }

      return "UnknownBufferMode";
   }

   QString Device::toString( Device::ClockSource source ) const {

      switch( source ) {

      case CLK_Internal:
         return "Internal";
         break;
      case CLK_External:
         return "External";
         break;
      default:
         REPORT_ERROR( tr( "Clock source (%1) not recognized" ).arg( source ) );
         break;
      }

      return "UnknownClockSource";
   }

   QString Device::toString( Device::EvCountMode mode ) const {

      switch( mode ) {

      case EV_CountAcceptedTriggers:
         return "AcceptedTriggers";
         break;
      case EV_CountAllTriggers:
         return "AllTriggers";
         break;
      default:
         REPORT_ERROR( tr( "Event counting mode (%1) not recognized" ).arg( mode ) );
         break;
      }

      return "UnknownEvCountMode";
   }

   QString Device::toString( SignalType type ) const {

      switch( type ) {

      case SGNL_NIM:
         return "NIM";
         break;
      case SGNL_TTL:
         return "TTL";
         break;
      default:
         REPORT_ERROR( tr( "Signal type (%1) not recognized" ).arg( type ) );
         break;
      }

      return "UnknownSignalType";
   }

   Device::TriggerMode
   Device::toTriggerMode( const QString& value ) const {

      if( value == "InputOverThreshold" ) {
         return TRG_InputOverThreshold;
      } else if( value == "InputUnderThreshold" ) {
         return TRG_InputUnderThreshold;
      }

      REPORT_ERROR( tr( "Trigger mode (%1) not recognized" ).arg( value ) );
      return TRG_InputOverThreshold;
   }

   Device::GateMode
   Device::toGateMode( const QString& value ) const {

      if( value == "Window" ) {
         return GATE_Window;
      } else if( value == "SingleShot" ) {
         return GATE_SingleShot;
      }

      REPORT_ERROR( tr( "Gate mode (%1) not recognized" ).arg( value ) );
      return GATE_Window;
   }

   Device::BufferMode
   Device::toBufferMode( const QString& value ) const {

      if( value == "NBuffers1" ) {
         return BUFF_NBuffers1;
      } else if( value == "NBuffers2" ) {
         return BUFF_NBuffers2;
      } else if( value == "NBuffers4" ) {
         return BUFF_NBuffers4;
      } else if( value == "NBuffers8" ) {
         return BUFF_NBuffers8;
      } else if( value == "NBuffers16" ) {
         return BUFF_NBuffers16;
      } else if( value == "NBuffers32" ) {
         return BUFF_NBuffers32;
      } else if( value == "NBuffers64" ) {
         return BUFF_NBuffers64;
      } else if( value == "NBuffers128" ) {
         return BUFF_NBuffers128;
      } else if( value == "NBuffers256" ) {
         return BUFF_NBuffers256;
      } else if( value == "NBuffers512" ) {
         return BUFF_NBuffers512;
      } else if( value == "NBuffers1024" ) {
         return BUFF_NBuffers1024;
      }

      REPORT_ERROR( tr( "Buffer mode (%1) not recognized" ).arg( value ) );
      return BUFF_NBuffers1;
   }

   Device::ClockSource
   Device::toClockSource( const QString& value ) const {

      if( value == "Internal" ) {
         return CLK_Internal;
      } else if( value == "External" ) {
         return CLK_External;
      }

      REPORT_ERROR( tr( "Clock source (%1) not recognized" ).arg( value ) );
      return CLK_Internal;
   }

   Device::EvCountMode
   Device::toEvCountMode( const QString& value ) const {

      if( value == "AcceptedTriggers" ) {
         return EV_CountAcceptedTriggers;
      } else if( value == "AllTriggers" ) {
         return EV_CountAllTriggers;
      }

      REPORT_ERROR( tr( "Event count mode (%1) not recognized" ).arg( value ) );
      return EV_CountAcceptedTriggers;
   }

   Device::SignalType
   Device::toSignalType( const QString& value ) const {

      if( value == "NIM" ) {
         return SGNL_NIM;
      } else if( value == "TTL" ) {
         return SGNL_TTL;
      }

      REPORT_ERROR( tr( "Signal type (%1) not recognized" ).arg( value ) );
      return SGNL_NIM;
   }

} // namespace dt5740
