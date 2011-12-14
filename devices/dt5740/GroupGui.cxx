// $Id$

// Qt include(s):
#include <QtGui/QGroupBox>
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QSpinBox>

// Local include(s):
#include "GroupGui.h"
#include "ChannelGui.h"
#include "ChannelConfig.h"

namespace dt5740 {

   // Initialize the static constant(s):
   const int GroupGui::WIDTH  = 470;
   const int GroupGui::HEIGHT = 650;

   GroupGui::GroupGui( GroupConfig& group, QWidget* parent,
                       Qt::WindowFlags flags )
      : QWidget( parent, flags ), m_group( group ),
        m_syncing( false ) {

      //
      // Set the fixed size for the widget:
      //
      resize( WIDTH, HEIGHT );
      setMinimumSize( WIDTH, HEIGHT );
      setMaximumSize( WIDTH, HEIGHT );

      //
      // Create the group box for the widget:
      //
      m_groupBox =
         new QGroupBox( tr( "Channel group %1" ).arg( m_group.getGroupNumber() ),
                        this );
      m_groupBox->setGeometry( QRect( 5, 0, WIDTH - 10, HEIGHT ) );

      //
      // Create the trigger enabling checkboxes:
      //
      m_trigEnabled = new QCheckBox( tr( "Enable self trigger" ),
                                     m_groupBox );
      m_trigEnabled->setGeometry( QRect( 10, 25, 400, 25 ) );
      connect( m_trigEnabled, SIGNAL( toggled( bool ) ),
               this, SLOT( trigEnabledSlot( bool ) ) );

      m_trigOutEnabled = new QCheckBox( tr( "Enable front panel trigger output" ),
                                        m_groupBox );
      m_trigOutEnabled->setGeometry( QRect( 10, 55, 400, 25 ) );
      connect( m_trigOutEnabled, SIGNAL( toggled( bool ) ),
               this, SLOT( trigOutEnabledSlot( bool ) ) );

      m_trigOvlpEnabled = new QCheckBox( tr( "Enable trigger overlaps" ),
                                         m_groupBox );
      m_trigOvlpEnabled->setGeometry( QRect( 10, 85, 400, 25 ) );
      connect( m_trigOvlpEnabled, SIGNAL( toggled( bool ) ),
               this, SLOT( trigOvlpEnabledSlot( bool ) ) );

      //
      // Create the trigger mode selector:
      //
      m_trigModeLabel = new QLabel( tr( "Trigger mode:" ),
                                    m_groupBox );
      m_trigModeLabel->setGeometry( QRect( 10, 115, 150, 25 ) );

      m_trigMode = new QComboBox( m_groupBox );
      m_trigMode->setGeometry( QRect( 180, 115, 250, 25 ) );
      m_trigMode->addItem( tr( "Trigger on input over threshold" ) );
      m_trigMode->addItem( tr( "Trigger on input under threshold" ) );
      connect( m_trigMode, SIGNAL( currentIndexChanged( int ) ),
               this, SLOT( trigModeSlot( int ) ) );

      //
      // Create the trigger threshold selector:
      //
      m_trigThresholdLabel = new QLabel( tr( "Trigger threshold:" ),
                                         m_groupBox );
      m_trigThresholdLabel->setGeometry( QRect( 10, 145, 150, 25 ) );

      m_trigThreshold = new QSpinBox( m_groupBox );
      m_trigThreshold->setGeometry( QRect( 180, 145, 150, 25 ) );
      m_trigThreshold->setRange( 0, 4095 ); // We have 12 bits for this setting
      connect( m_trigThreshold, SIGNAL( valueChanged( int ) ),
               this, SLOT( trigThresholdSlot( int ) ) );

      //
      // Create the trigger mask setting:
      //
      m_trigMaskBox = new QGroupBox( tr( "Trigger mask (channels enabled)" ),
                                     m_groupBox );
      m_trigMaskBox->setGeometry( QRect( 10, 175, WIDTH - 30, 50 ) );
      m_trigMaskBox->setToolTip( "You can set here which channels should contribute "
                                 "to the self triggering of the device. Note that "
                                 "this setting is independent of which channels you "
                                 "actually choose to read out." );

      for( int i = 0; i < GroupConfig::CHANNELS_IN_GROUP; ++i ) {
         m_trigMask[ i ] =
            new QCheckBox( tr( "%1." )
                           .arg( m_group.getGroupNumber() *
                                 GroupConfig::CHANNELS_IN_GROUP + i ),
                           m_trigMaskBox );
         m_trigMask[ i ]->setGeometry( QRect( 5 + i * 55, 20, 50, 25 ) );
         connect( m_trigMask[ i ], SIGNAL( toggled( bool ) ),
                  this, SLOT( trigMaskSlot( bool ) ) );
      }

      //
      // Create the DC offset setting:
      //
      m_dcOffsetLabel = new QLabel( tr( "DC offset:" ),
                                    m_groupBox );
      m_dcOffsetLabel->setGeometry( QRect( 10, 245, 150, 25 ) );

      m_dcOffset = new QSpinBox( m_groupBox );
      m_dcOffset->setGeometry( QRect( 180, 245, 150, 25 ) );
      m_dcOffset->setRange( 0, 65535 ); // We have 16 bits for this setting
      connect( m_dcOffset, SIGNAL( valueChanged( int ) ),
               this, SLOT( dcOffsetSlot( int ) ) );

      //
      // Create the pattern generation setting:
      //
      m_patGenEnabled = new QCheckBox( tr( "Enable test pattern generation" ),
                                       m_groupBox );
      m_patGenEnabled->setGeometry( QRect( 10, 275, 400, 25 ) );
      connect( m_patGenEnabled, SIGNAL( toggled( bool ) ),
               this, SLOT( patGenEnabledSlot( bool ) ) );

      //
      // Create the gate mode setting:
      //
      m_gateModeLabel = new QLabel( tr( "Gate mode:" ),
                                    m_groupBox );
      m_gateModeLabel->setGeometry( QRect( 10, 305, 150, 25 ) );

      m_gateMode = new QComboBox( m_groupBox );
      m_gateMode->setGeometry( QRect( 180, 305, 250, 25 ) );
      m_gateMode->addItem( "Window" );
      m_gateMode->addItem( "Single shot" );
      connect( m_gateMode, SIGNAL( currentIndexChanged( int ) ),
               this, SLOT( gateModeSlot( int ) ) );

      //
      // Create the buffer mode setting:
      //
      m_bufferModeLabel = new QLabel( tr( "Buffer mode:" ),
                                      m_groupBox );
      m_bufferModeLabel->setGeometry( QRect( 10, 335, 150, 25 ) );

      m_bufferMode = new QComboBox( m_groupBox );
      m_bufferMode->setGeometry( QRect( 180, 335, 250, 25 ) );
      m_bufferMode->setToolTip( "You can choose how many samples should be "
                                "collected after each trigger, using this "
                                "property." );
      m_bufferMode->addItem( "1 buffer with 192k samples" );
      m_bufferMode->addItem( "2 buffers with 96k samples" );
      m_bufferMode->addItem( "4 buffers with 48k samples" );
      m_bufferMode->addItem( "8 buffers with 24k samples" );
      m_bufferMode->addItem( "16 buffers with 12k samples" );
      m_bufferMode->addItem( "32 buffers with 6k samples" );
      m_bufferMode->addItem( "64 buffers with 3k samples" );
      m_bufferMode->addItem( "128 buffers with 1536 samples" );
      m_bufferMode->addItem( "256 buffers with 768 samples" );
      m_bufferMode->addItem( "512 buffers with 384 samples" );
      m_bufferMode->addItem( "1024 buffers with 192 samples" );
      connect( m_bufferMode, SIGNAL( currentIndexChanged( int ) ),
               this, SLOT( bufferModeSlot( int ) ) );

      //
      // Create the post trigger percentage setting:
      //
      m_postTrigPercentageLabel = new QLabel( tr( "Post trigger percentage:" ),
                                              m_groupBox );
      m_postTrigPercentageLabel->setGeometry( QRect( 10, 365, 150, 25 ) );

      m_postTrigPercentage = new QSpinBox( m_groupBox );
      m_postTrigPercentage->setGeometry( QRect( 180, 365, 150, 25 ) );
      m_postTrigPercentage->setRange( 0, 100 ); // This is an integer percentage...
      m_postTrigPercentage->setValue( 100 );
      m_postTrigPercentage->setToolTip( "You can set here how many of the collected "
                                        "samples should be taken after the trigger "
                                        "signal." );
      connect( m_postTrigPercentage, SIGNAL( valueChanged( int ) ),
               this, SLOT( postTrigPercentageSlot( int ) ) );

      //
      // Create the channel labels:
      //
      m_nameLabel = new QLabel( tr( "Name" ), m_groupBox );
      m_nameLabel->setAlignment( Qt::AlignCenter );
      m_nameLabel->setGeometry( QRect( 125, 405, 75, 25 ) );

      m_channelsLabel = new QLabel( tr( "Channels" ), m_groupBox );
      m_channelsLabel->setAlignment( Qt::AlignCenter );
      m_channelsLabel->setGeometry( QRect( 205, 405, 75, 25 ) );

      m_lowerBoundLabel = new QLabel( tr( "Lower" ), m_groupBox );
      m_lowerBoundLabel->setAlignment( Qt::AlignCenter );
      m_lowerBoundLabel->setGeometry( QRect( 285, 405, 75, 25 ) );

      m_upperBoundLabel = new QLabel( tr( "Upper" ), m_groupBox );
      m_upperBoundLabel->setAlignment( Qt::AlignCenter );
      m_upperBoundLabel->setGeometry( QRect( 365, 405, 75, 25 ) );

      //
      // Create the widgets modifying the settings of the input channels:
      //
      for( int i = 0; i < GroupConfig::CHANNELS_IN_GROUP; ++i ) {

         m_channels[ i ] =
            new ChannelGui( m_group.getGroupNumber() *
                            GroupConfig::CHANNELS_IN_GROUP + i,
                            m_groupBox );
         m_channels[ i ]->setGeometry( QRect( 10, 440 + i * 25,
                                              ChannelGui::WIDTH,
                                              ChannelGui::HEIGHT ) );

         connect( m_channels[ i ], SIGNAL( enableChanged( int, bool ) ),
                  this, SLOT( channelEnabledSlot( int, bool ) ) );
         connect( m_channels[ i ], SIGNAL( nameChanged( int, const QString& ) ),
                  this, SLOT( nameChangedSlot( int, const QString& ) ) );
         connect( m_channels[ i ], SIGNAL( channelsChanged( int, int ) ),
                  this, SLOT( channelsChangedSlot( int, int ) ) );
         connect( m_channels[ i ], SIGNAL( lowerBoundChanged( int, double ) ),
                  this, SLOT( lowerBoundChangedSlot( int, double ) ) );
         connect( m_channels[ i ], SIGNAL( upperBoundChanged( int, double ) ),
                  this, SLOT( upperBoundChangedSlot( int, double ) ) );
      }

   }

   GroupGui::~GroupGui() {

      delete m_trigEnabled;
      delete m_trigOutEnabled;
      delete m_trigOvlpEnabled;

      delete m_trigModeLabel;
      delete m_trigMode;

      delete m_trigThresholdLabel;
      delete m_trigThreshold;

      for( int i = 0; i < GroupConfig::CHANNELS_IN_GROUP; ++i ) {
         delete m_trigMask[ i ];
      }
      delete m_trigMaskBox;

      delete m_postTrigPercentageLabel;
      delete m_postTrigPercentage;

      delete m_dcOffsetLabel;
      delete m_dcOffset;

      delete m_patGenEnabled;

      delete m_gateModeLabel;
      delete m_gateMode;

      delete m_bufferModeLabel;
      delete m_bufferMode;

      delete m_nameLabel;
      delete m_channelsLabel;
      delete m_lowerBoundLabel;
      delete m_upperBoundLabel;

      for( int i = 0; i < GroupConfig::CHANNELS_IN_GROUP; ++i ) {
         delete m_channels[ i ];
      }
   }

   ChannelGui* GroupGui::getChannel( int index ) const {

      if( ( index >= 0 ) &&
          ( index < GroupConfig::CHANNELS_IN_GROUP ) ) {
         return m_channels[ index ];
      }

      REPORT_ERROR( tr( "Channel with invalid index (%1) requested" ).arg( index ) );
      return 0;
   }

   void GroupGui::sync() {

      m_syncing = true;

      if( m_group.getTrigEnabled() ) {
         m_trigEnabled->setChecked( true );
      } else {
         m_trigEnabled->setChecked( false );
      }

      if( m_group.getTrigOutEnabled() ) {
         m_trigOutEnabled->setChecked( true );
      } else {
         m_trigOutEnabled->setChecked( false );
      }

      if( m_group.getTrigOvlpEnabled() ) {
         m_trigOvlpEnabled->setChecked( true );
      } else {
         m_trigOvlpEnabled->setChecked( false );
      }

      switch( m_group.getTrigMode() ) {

      case GroupConfig::TriggerOnInputOverThreshold:
         m_trigMode->setCurrentIndex( 0 );
         break;
      case GroupConfig::TriggerOnInputUnderThreshold:
         m_trigMode->setCurrentIndex( 1 );
         break;
      default:
         REPORT_ERROR( tr( "Trigger mode not recognized" ) );
         break;
      }

      m_trigThreshold->setValue( m_group.getTrigThreshold() );

      for( int i = 0; i < GroupConfig::CHANNELS_IN_GROUP; ++i ) {
         if( ( m_group.getTrigMask() >> i ) & 0x1 ) {
            m_trigMask[ i ]->setChecked( true );
         } else {
            m_trigMask[ i ]->setChecked( false );
         }
      }

      m_postTrigPercentage->setValue( m_group.getPostTrigPercentage() );
      m_dcOffset->setValue( m_group.getDCOffset() );

      if( m_group.getPatGenEnabled() ) {
         m_patGenEnabled->setChecked( true );
      } else {
         m_patGenEnabled->setChecked( false );
      }

      switch( m_group.getGateMode() ) {

      case GroupConfig::WindowGate:
         m_gateMode->setCurrentIndex( 0 );
         break;
      case GroupConfig::SingleShotGate:
         m_gateMode->setCurrentIndex( 1 );
         break;
      default:
         REPORT_ERROR( tr( "Gate mode not recognized" ) );
         break;
      }

      switch( m_group.getBufferMode() ) {

      case GroupConfig::NBuffers1:
         m_bufferMode->setCurrentIndex( 0 );
         break;
      case GroupConfig::NBuffers2:
         m_bufferMode->setCurrentIndex( 1 );
         break;
      case GroupConfig::NBuffers4:
         m_bufferMode->setCurrentIndex( 2 );
         break;
      case GroupConfig::NBuffers8:
         m_bufferMode->setCurrentIndex( 3 );
         break;
      case GroupConfig::NBuffers16:
         m_bufferMode->setCurrentIndex( 4 );
         break;
      case GroupConfig::NBuffers32:
         m_bufferMode->setCurrentIndex( 5 );
         break;
      case GroupConfig::NBuffers64:
         m_bufferMode->setCurrentIndex( 6 );
         break;
      case GroupConfig::NBuffers128:
         m_bufferMode->setCurrentIndex( 7 );
         break;
      case GroupConfig::NBuffers256:
         m_bufferMode->setCurrentIndex( 8 );
         break;
      case GroupConfig::NBuffers512:
         m_bufferMode->setCurrentIndex( 9 );
         break;
      case GroupConfig::NBuffers1024:
         m_bufferMode->setCurrentIndex( 10 );
         break;
      default:
         REPORT_ERROR( tr( "Buffer mode not recognized" ) );
         break;
      }

      //
      // Synchronize all the channels of the group:
      //
      for( int i = 0; i < GroupConfig::CHANNELS_IN_GROUP; ++i ) {
         if( m_group.getChannel( i ) ) {
            m_channels[ i ]->setEnabled( false );
            m_channels[ i ]->setName( m_group.getChannel( i )->getName() );
            m_channels[ i ]->setChannels( m_group.getChannel( i )->getNumberOfChannels() );
            m_channels[ i ]->setLowerBound( m_group.getChannel( i )->getLowerBound() );
            m_channels[ i ]->setUpperBound( m_group.getChannel( i )->getUpperBound() );
            m_channels[ i ]->setEnabled( true );
         } else {
            m_channels[ i ]->setEnabled( false );
         }
      }

      m_syncing = false;

      return;
   }

   void GroupGui::trigEnabledSlot( bool checked ) {

      // Don't emit anything if the object is syncing itself to the
      // configuration:
      if( m_syncing ) return;

      m_group.setTrigEnabled( checked );
      return;
   }

   void GroupGui::trigOutEnabledSlot( bool checked ) {

      // Don't emit anything if the object is syncing itself to the
      // configuration:
      if( m_syncing ) return;

      m_group.setTrigOutEnabled( checked );
      return;
   }

   void GroupGui::trigOvlpEnabledSlot( bool checked ) {

      // Don't emit anything if the object is syncing itself to the
      // configuration:
      if( m_syncing ) return;

      m_group.setTrigOvlpEnabled( checked );
      return;
   }

   void GroupGui::trigModeSlot( int index ) {

      // Don't emit anything if the object is syncing itself to the
      // configuration:
      if( m_syncing ) return;

      // Translate the state of the combo box into an enumeration value:
      switch( index ) {

      case 0:
         m_group.setTrigMode( GroupConfig::TriggerOnInputOverThreshold );
         break;
      case 1:
         m_group.setTrigMode( GroupConfig::TriggerOnInputUnderThreshold );
         break;
      default:
         REPORT_ERROR( tr( "Trigger mode not recognized" ) );
         break;
      }

      return;
   }

   void GroupGui::trigThresholdSlot( int value ) {

      // Don't emit anything if the object is syncing itself to the
      // configuration:
      if( m_syncing ) return;

      m_group.setTrigThreshold( value );
      return;
   }

   void GroupGui::trigMaskSlot( bool ) {

      // Don't emit anything if the object is syncing itself to the
      // configuration:
      if( m_syncing ) return;

      // Construct the trigger mask from the state of the
      // check buttons:
      unsigned int mask = 0;
      for( int i = 0; i < GroupConfig::CHANNELS_IN_GROUP; ++i ) {
         if( m_trigMask[ i ]->isChecked() ) {
            mask |= ( 0x1 << i );
         }
      }

      m_group.setTrigMask( mask );
      return;
   }

   void GroupGui::postTrigPercentageSlot( int value ) {

      // Don't emit anything if the object is syncing itself to the
      // configuration:
      if( m_syncing ) return;

      m_group.setPostTrigPercentage( value );
      return;
   }

   void GroupGui::dcOffsetSlot( int value ) {

      // Don't emit anything if the object is syncing itself to the
      // configuration:
      if( m_syncing ) return;

      m_group.setDCOffset( value );
      return;
   }

   void GroupGui::patGenEnabledSlot( bool checked ) {

      // Don't emit anything if the object is syncing itself to the
      // configuration:
      if( m_syncing ) return;

      m_group.setPatGenEnabled( checked );
      return;
   }

   void GroupGui::gateModeSlot( int index ) {

      // Don't emit anything if the object is syncing itself to the
      // configuration:
      if( m_syncing ) return;

      // Translate the state of the combo box into an enumeration value:
      switch( index ) {

      case 0:
         m_group.setGateMode( GroupConfig::WindowGate );
         break;
      case 1:
         m_group.setGateMode( GroupConfig::SingleShotGate );
         break;
      default:
         REPORT_ERROR( tr( "Gate mode not recognized" ) );
         break;
      }

      return;
   }

   void GroupGui::bufferModeSlot( int index ) {

      // Don't emit anything if the object is syncing itself to the
      // configuration:
      if( m_syncing ) return;

      // Translate the state of the combo box into an enumeration value:
      switch( index ) {

      case 0:
         m_group.setBufferMode( GroupConfig::NBuffers1 );
         break;
      case 1:
         m_group.setBufferMode( GroupConfig::NBuffers2 );
         break;
      case 2:
         m_group.setBufferMode( GroupConfig::NBuffers4 );
         break;
      case 3:
         m_group.setBufferMode( GroupConfig::NBuffers8 );
         break;
      case 4:
         m_group.setBufferMode( GroupConfig::NBuffers16 );
         break;
      case 5:
         m_group.setBufferMode( GroupConfig::NBuffers32 );
         break;
      case 6:
         m_group.setBufferMode( GroupConfig::NBuffers64 );
         break;
      case 7:
         m_group.setBufferMode( GroupConfig::NBuffers128 );
         break;
      case 8:
         m_group.setBufferMode( GroupConfig::NBuffers256 );
         break;
      case 9:
         m_group.setBufferMode( GroupConfig::NBuffers512 );
         break;
      case 10:
         m_group.setBufferMode( GroupConfig::NBuffers1024 );
         break;
      default:
         REPORT_ERROR( tr( "Buffer mode not recognized" ) );
         break;
      }

      return;
   }

   void GroupGui::channelEnabledSlot( int channel, bool on ) {

      m_group.enableChannel( channel -
                             ( m_group.getGroupNumber() *
                               GroupConfig::CHANNELS_IN_GROUP ), on );

      return;
   }

   void GroupGui::nameChangedSlot( int channel, const QString& text ) {

      m_group.getChannel( channel -
                          ( m_group.getGroupNumber() *
                            GroupConfig::CHANNELS_IN_GROUP ) )->setName( text );
      return;
   }

   void GroupGui::channelsChangedSlot( int channel, int channels ) {

      m_group.getChannel( channel -
                          ( m_group.getGroupNumber() *
                            GroupConfig::CHANNELS_IN_GROUP ) )->setNumberOfChannels( channels );
      return;
   }

   void GroupGui::lowerBoundChangedSlot( int channel, double value ) {

      m_group.getChannel( channel -
                          ( m_group.getGroupNumber() *
                            GroupConfig::CHANNELS_IN_GROUP ) )->setLowerBound( value );
      return;
   }

   void GroupGui::upperBoundChangedSlot( int channel, double value ) {

      m_group.getChannel( channel -
                          ( m_group.getGroupNumber() *
                            GroupConfig::CHANNELS_IN_GROUP ) )->setUpperBound( value );
      return;
   }

} // namespace dt5740
