// $Id$

// Qt include(s):
#include <QtCore/QIODevice>
#include <QtCore/QDataStream>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>

// Local include(s):
#include "GroupConfig.h"

namespace dt5740 {

   GroupConfig::GroupConfig()
      : m_groupNumber( -1 ),
        m_trigThreshold( 0 ), m_dcOffset( 0 ), m_trigMask( 0 ),
        m_trigMode( TriggerOnInputOverThreshold ), 
        m_trigOvlpEnabled( false ), m_patGenEnabled( false ),
        m_gateMode( WindowGate ), m_bufferMode( NBuffers1 ),
        m_trigEnabled( false ), m_trigOutEnabled( false ),
        m_postTrigSamples( 0 ),
        m_logger( "dt5740::GroupConfig" ) {

   }

   GroupConfig::GroupConfig( const GroupConfig& parent )
      : dev::Config(),
        m_groupNumber( parent.m_groupNumber ),
        m_trigThreshold( parent.m_trigThreshold ),
        m_dcOffset( parent.m_dcOffset ),
        m_trigMask( parent.m_trigMask ),
        m_trigMode( parent.m_trigMode ), 
        m_trigOvlpEnabled( parent.m_trigOvlpEnabled ),
        m_patGenEnabled( parent.m_patGenEnabled ),
        m_gateMode( parent.m_gateMode ),
        m_bufferMode( parent.m_bufferMode ),
        m_trigEnabled( parent.m_trigEnabled ),
        m_trigOutEnabled( parent.m_trigOutEnabled ),
        m_postTrigSamples( parent.m_postTrigSamples ),
        m_logger( "dt5740::GroupConfig" ) {

   }

   GroupConfig& GroupConfig::operator= ( const GroupConfig& rh ) {

      m_groupNumber = rh.m_groupNumber;
      m_trigThreshold = rh.m_trigThreshold;
      m_dcOffset = rh.m_dcOffset;
      m_trigMask = rh.m_trigMask;
      m_trigMode = rh.m_trigMode;
      m_trigOvlpEnabled = rh.m_trigOvlpEnabled;
      m_patGenEnabled = rh.m_patGenEnabled;
      m_gateMode = rh.m_gateMode;
      m_bufferMode = rh.m_bufferMode;
      m_trigEnabled = rh.m_trigEnabled;
      m_trigOutEnabled = rh.m_trigOutEnabled;
      m_postTrigSamples = rh.m_postTrigSamples;

      return *this;
   }

   bool GroupConfig::readConfig( QIODevice* dev ) {

      REPORT_VERBOSE( tr( "Reading configuration from binary input" ) );

      unsigned int temp = 0;

      QDataStream input( dev );
      input.setVersion( QDataStream::Qt_4_0 );

      input >> m_groupNumber;
      input >> m_trigThreshold;
      input >> m_dcOffset;
      input >> m_trigMask;
      input >> temp;
      m_trigMode = toTriggerMode( temp );
      input >> m_trigOvlpEnabled;
      input >> m_patGenEnabled;
      input >> temp;
      m_gateMode = toGateMode( temp );
      input >> temp;
      m_bufferMode = toBufferMode( temp );
      input >> m_trigEnabled;
      input >> m_trigOutEnabled;
      input >> m_postTrigSamples;

      return true;
   }

   bool GroupConfig::writeConfig( QIODevice* dev ) const {

      REPORT_VERBOSE( tr( "Writing configuration to binary output" ) );

      QDataStream output( dev );
      output.setVersion( QDataStream::Qt_4_0 );

      output << m_groupNumber;
      output << m_trigThreshold;
      output << m_dcOffset;
      output << m_trigMask;
      output << toUInt( m_trigMode );
      output << m_trigOvlpEnabled;
      output << m_patGenEnabled;
      output << toUInt( m_gateMode );
      output << toUInt( m_bufferMode );
      output << m_trigEnabled;
      output << m_trigOutEnabled;
      output << m_postTrigSamples;

      return true;
   }

   bool GroupConfig::readConfig( const QDomElement& node ) {

      REPORT_VERBOSE( tr( "Reading configuration from XML input" ) );

      bool ok;

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

      m_trigMode = toTriggerMode( node.attribute( "TrigMode",
                                                  "0" ).toUInt( &ok ) );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading a "
                           "\"trigger mode\" value" ) );
         return false;
      }

      m_trigOvlpEnabled = node.attribute( "TrigOvlpEnabled", "0" ).toInt( &ok );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading a "
                           "\"trigger overlap enabled\" value" ) );
         return false;
      }

      m_patGenEnabled = node.attribute( "PatGenEnabled", "0" ).toInt( &ok );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading a "
                           "\"pattern generation enabled\" value" ) );
         return false;
      }

      m_gateMode = toGateMode( node.attribute( "GateMode",
                                               "0" ).toUInt( &ok ) );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading a "
                           "\"gate mode\" value" ) );
         return false;
      }

      m_bufferMode = toBufferMode( node.attribute( "BufferMode",
                                                   "0" ).toUInt( &ok ) );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading a "
                           "\"buffer mode\" value" ) );
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

      m_postTrigSamples = node.attribute( "PostTrigSamples",
                                          "0" ).toInt( &ok );
      if( ! ok ) {
         REPORT_ERROR( tr( "There was a problem reading a "
                           "\"post trigger samples\" value" ) );
         return false;
      }

      return true;
   }

   bool GroupConfig::writeConfig( QDomElement& node ) const {

      REPORT_VERBOSE( tr( "Writing configuration to XML output" ) );

      node.setAttribute( "Number", m_groupNumber );
      node.setAttribute( "TrigThreshold", m_trigThreshold );
      node.setAttribute( "DCOffset", m_dcOffset );
      node.setAttribute( "TrigMask", m_trigMask );
      node.setAttribute( "TrigMode", toUInt( m_trigMode ) );
      node.setAttribute( "TrigOvlpEnabled", m_trigOvlpEnabled );
      node.setAttribute( "PatGenEnabled", m_patGenEnabled );
      node.setAttribute( "GateMode", toUInt( m_gateMode ) );
      node.setAttribute( "BufferMode", toUInt( m_bufferMode ) );
      node.setAttribute( "TrigEnabled", m_trigEnabled );
      node.setAttribute( "TrigOutEnabled", m_trigOutEnabled );
      node.setAttribute( "PostTrigSamples", m_postTrigSamples );

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

   bool GroupConfig::getTrigOvlpEnabled() const {

      return m_trigOvlpEnabled;
   }

   void GroupConfig::setTrigOvlpEnabled( bool on ) {

      m_trigOvlpEnabled = on;
      return;
   }

   GroupConfig::TriggerMode GroupConfig::getTrigMode() const {

      return m_trigMode;
   }

   void GroupConfig::setTrigMode( GroupConfig::TriggerMode mode ) {

      m_trigMode = mode;
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

   int GroupConfig::getPostTrigSamples() const {

      return m_postTrigSamples;
   }

   void GroupConfig::setPostTrigSamples( int value ) {

      m_postTrigSamples = value;
      return;
   }

   int GroupConfig::getDCOffset() const {

      return m_dcOffset;
   }

   void GroupConfig::setDCOffset( int value ) {

      m_dcOffset = value;
      return;
   }

   bool GroupConfig::getPatGenEnabled() const {

      return m_patGenEnabled;
   }

   void GroupConfig::setPatGenEnabled( bool on ) {

      m_patGenEnabled = on;
      return;
   }

   GroupConfig::GateMode GroupConfig::getGateMode() const {

      return m_gateMode;
   }

   void GroupConfig::setGateMode( GroupConfig::GateMode mode ) {

      m_gateMode = mode;
      return;
   }

   GroupConfig::BufferMode GroupConfig::getBufferMode() const {

      return m_bufferMode;
   }

   void GroupConfig::setBufferMode( GroupConfig::BufferMode mode ) {

      m_bufferMode = mode;
      return;
   }

   unsigned int GroupConfig::toUInt( GroupConfig::TriggerMode mode ) const {

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

   unsigned int GroupConfig::toUInt( GroupConfig::GateMode mode ) const {

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

   unsigned int GroupConfig::toUInt( GroupConfig::BufferMode mode ) const {

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

   GroupConfig::TriggerMode
   GroupConfig::toTriggerMode( unsigned int value ) const {

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

   GroupConfig::GateMode
   GroupConfig::toGateMode( unsigned int value ) const {

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

   GroupConfig::BufferMode
   GroupConfig::toBufferMode( unsigned int value ) const {

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
