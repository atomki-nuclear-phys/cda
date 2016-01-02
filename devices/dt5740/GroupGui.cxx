// $Id$

// Qt include(s):
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>

// Local include(s):
#include "GroupGui.h"
#include "ChannelGui.h"
#include "ChannelConfig.h"

namespace dt5740 {

   // Initialize the static constant(s):
   const int GroupGui::WIDTH  = 480;
   const int GroupGui::HEIGHT = 850;

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
      m_groupBox.reset( new QGroupBox( tr( "Channel group %1" )
                                       .arg( m_group.getGroupNumber() ),
                                       this ) );
      m_groupBox->setGeometry( QRect( 5, 0, WIDTH - 10, HEIGHT ) );

      //
      // Create the trigger enabling checkboxes:
      //
      m_trigEnabled.reset( new QCheckBox( tr( "Enable self trigger" ),
                                          m_groupBox.get() ) );
      m_trigEnabled->setGeometry( QRect( 10, 25, 400, 25 ) );
      connect( m_trigEnabled.get(), SIGNAL( toggled( bool ) ),
               this, SLOT( trigEnabledSlot( bool ) ) );

      m_trigOutEnabled.reset( new QCheckBox( tr( "Forward self trigger to "
                                                 "front panel" ),
                                             m_groupBox.get() ) );
      m_trigOutEnabled->setGeometry( QRect( 10, 55, 400, 25 ) );
      connect( m_trigOutEnabled.get(), SIGNAL( toggled( bool ) ),
               this, SLOT( trigOutEnabledSlot( bool ) ) );

      //
      // Create the trigger threshold selector:
      //
      m_trigThresholdLabel.reset( new QLabel( tr( "Trigger threshold:" ),
                                              m_groupBox.get() ) );
      m_trigThresholdLabel->setGeometry( QRect( 10, 85, 150, 25 ) );

      m_trigThreshold.reset( new QSpinBox( m_groupBox.get() ) );
      m_trigThreshold->setGeometry( QRect( 180, 85, 150, 25 ) );
      m_trigThreshold->setRange( 0, 4095 ); // We have 12 bits for this setting
      connect( m_trigThreshold.get(), SIGNAL( valueChanged( int ) ),
               this, SLOT( trigThresholdSlot( int ) ) );

      //
      // Create the DC offset setting:
      //
      m_dcOffsetLabel.reset( new QLabel( tr( "DC offset:" ),
                                         m_groupBox.get() ) );
      m_dcOffsetLabel->setGeometry( QRect( 10, 115, 150, 25 ) );

      m_dcOffset.reset( new QSpinBox( m_groupBox.get() ) );
      m_dcOffset->setGeometry( QRect( 180, 115, 150, 25 ) );
      m_dcOffset->setRange( 0, 65535 ); // We have 16 bits for this setting
      connect( m_dcOffset.get(), SIGNAL( valueChanged( int ) ),
               this, SLOT( dcOffsetSlot( int ) ) );

      //
      // Create the trigger mask setting:
      //
      m_trigMaskBox.reset( new QGroupBox( tr( "Trigger mask (channels "
                                              "enabled)" ),
                                          m_groupBox.get() ) );
      m_trigMaskBox->setGeometry( QRect( 10, 145, WIDTH - 30, 50 ) );
      m_trigMaskBox->setToolTip( "You can set here which channels should "
                                 "contribute to the self triggering of the "
                                 "device. Note that this setting is "
                                 "independent of which channels you "
                                 "actually choose to read out." );

      for( int i = 0; i < GroupConfig::CHANNELS_IN_GROUP; ++i ) {
         m_trigMask[ i ].reset( new QCheckBox( tr( "%1." )
                                     .arg( m_group.getGroupNumber() *
                                           GroupConfig::CHANNELS_IN_GROUP + i ),
                                               m_trigMaskBox.get() ) );
         m_trigMask[ i ]->setGeometry( QRect( 5 + i * 55, 25, 50, 25 ) );
         connect( m_trigMask[ i ].get(), SIGNAL( toggled( bool ) ),
                  this, SLOT( trigMaskSlot( bool ) ) );
      }

      //
      // Create the channel labels:
      //
      m_nameLabel.reset( new QLabel( tr( "Name" ), m_groupBox.get() ) );
      m_nameLabel->setAlignment( Qt::AlignCenter );
      m_nameLabel->setGeometry( QRect( 125, 205, 75, 25 ) );

      m_channelsLabel.reset( new QLabel( tr( "Channels" ), m_groupBox.get() ) );
      m_channelsLabel->setAlignment( Qt::AlignCenter );
      m_channelsLabel->setGeometry( QRect( 205, 205, 75, 25 ) );

      m_lowerBoundLabel.reset( new QLabel( tr( "Lower" ), m_groupBox.get() ) );
      m_lowerBoundLabel->setAlignment( Qt::AlignCenter );
      m_lowerBoundLabel->setGeometry( QRect( 285, 205, 75, 25 ) );

      m_upperBoundLabel.reset( new QLabel( tr( "Upper" ), m_groupBox.get() ) );
      m_upperBoundLabel->setAlignment( Qt::AlignCenter );
      m_upperBoundLabel->setGeometry( QRect( 365, 205, 75, 25 ) );

      //
      // Create the widgets modifying the settings of the input channels:
      //
      for( int i = 0; i < GroupConfig::CHANNELS_IN_GROUP; ++i ) {

         m_channels[ i ].reset( new ChannelGui( m_group.getGroupNumber() *
                                                GroupConfig::CHANNELS_IN_GROUP +
                                                i, m_groupBox.get() ) );
         m_channels[ i ]->setGeometry( QRect( 10, 240 + i * 75,
                                              ChannelGui::WIDTH,
                                              ChannelGui::HEIGHT ) );

         connect( m_channels[ i ].get(), SIGNAL( enableChanged( int, bool ) ),
                  this, SLOT( channelEnabledSlot( int, bool ) ) );
         connect( m_channels[ i ].get(),
                  SIGNAL( rawNameChanged( int, const QString& ) ),
                  this, SLOT( rawNameChangedSlot( int, const QString& ) ) );

         connect( m_channels[ i ].get(),
                  SIGNAL( timeNameChanged( int, const QString& ) ),
                  this, SLOT( timeNameChangedSlot( int, const QString& ) ) );
         connect( m_channels[ i ].get(),
                  SIGNAL( timeChannelsChanged( int, int ) ),
                  this, SLOT( timeChannelsChangedSlot( int, int ) ) );
         connect( m_channels[ i ].get(),
                  SIGNAL( timeLowerBoundChanged( int, double ) ),
                  this, SLOT( timeLowerBoundChangedSlot( int, double ) ) );
         connect( m_channels[ i ].get(),
                  SIGNAL( timeUpperBoundChanged( int, double ) ),
                  this, SLOT( timeUpperBoundChangedSlot( int, double ) ) );

         connect( m_channels[ i ].get(),
                  SIGNAL( energyNameChanged( int, const QString& ) ),
                  this, SLOT( energyNameChangedSlot( int, const QString& ) ) );
         connect( m_channels[ i ].get(),
                  SIGNAL( energyChannelsChanged( int, int ) ),
                  this, SLOT( energyChannelsChangedSlot( int, int ) ) );
         connect( m_channels[ i ].get(),
                  SIGNAL( energyLowerBoundChanged( int, double ) ),
                  this, SLOT( energyLowerBoundChangedSlot( int, double ) ) );
         connect( m_channels[ i ].get(),
                  SIGNAL( energyUpperBoundChanged( int, double ) ),
                  this, SLOT( energyUpperBoundChangedSlot( int, double ) ) );
      }

   }

   ChannelGui* GroupGui::getChannel( int index ) const {

      if( ( index >= 0 ) &&
          ( index < GroupConfig::CHANNELS_IN_GROUP ) ) {
         return m_channels[ index ].get();
      }

      REPORT_ERROR( tr( "Channel with invalid index (%1) requested" )
                    .arg( index ) );
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

      m_trigThreshold->setValue( m_group.getTrigThreshold() );

      for( int i = 0; i < GroupConfig::CHANNELS_IN_GROUP; ++i ) {
         if( ( m_group.getTrigMask() >> i ) & 0x1 ) {
            m_trigMask[ i ]->setChecked( true );
         } else {
            m_trigMask[ i ]->setChecked( false );
         }
      }

      m_dcOffset->setValue( m_group.getDCOffset() );

      //
      // Synchronize all the channels of the group:
      //
      for( int i = 0; i < GroupConfig::CHANNELS_IN_GROUP; ++i ) {
         if( m_group.getChannel( i ) ) {
            m_channels[ i ]->setEnabled( false );

            m_channels[ i ]->setRawName(
                     m_group.getChannel( i )->getRawName() );

            m_channels[ i ]->setTimeName(
                     m_group.getChannel( i )->getTimeName() );
            m_channels[ i ]->setTimeChannels(
                     m_group.getChannel( i )->getTimeNumberOfChannels() );
            m_channels[ i ]->setTimeLowerBound(
                     m_group.getChannel( i )->getTimeLowerBound() );
            m_channels[ i ]->setTimeUpperBound(
                     m_group.getChannel( i )->getTimeUpperBound() );

            m_channels[ i ]->setEnergyName(
                     m_group.getChannel( i )->getEnergyName() );
            m_channels[ i ]->setEnergyChannels(
                     m_group.getChannel( i )->getEnergyNumberOfChannels() );
            m_channels[ i ]->setEnergyLowerBound(
                     m_group.getChannel( i )->getEnergyLowerBound() );
            m_channels[ i ]->setEnergyUpperBound(
                     m_group.getChannel( i )->getEnergyUpperBound() );

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

   void GroupGui::dcOffsetSlot( int value ) {

      // Don't emit anything if the object is syncing itself to the
      // configuration:
      if( m_syncing ) return;

      m_group.setDCOffset( value );
      return;
   }

   void GroupGui::channelEnabledSlot( int channel, bool on ) {

      m_group.enableChannel( channel -
                             ( m_group.getGroupNumber() *
                               GroupConfig::CHANNELS_IN_GROUP ), on );

      return;
   }

   void GroupGui::rawNameChangedSlot( int channel, const QString& text ) {

      ChannelConfig* c =
            m_group.getChannel( channel -
                                ( m_group.getGroupNumber() *
                                  GroupConfig::CHANNELS_IN_GROUP ) );
      c->setRawName( text );
      return;
   }

   void GroupGui::timeNameChangedSlot( int channel, const QString& text ) {

      ChannelConfig* c =
            m_group.getChannel( channel -
                                ( m_group.getGroupNumber() *
                                  GroupConfig::CHANNELS_IN_GROUP ) );
      c->setTimeName( text );
      return;
   }

   void GroupGui::timeChannelsChangedSlot( int channel, int channels ) {

      ChannelConfig* c =
            m_group.getChannel( channel -
                                ( m_group.getGroupNumber() *
                                  GroupConfig::CHANNELS_IN_GROUP ) );
      c->setTimeNumberOfChannels( channels );
      return;
   }

   void GroupGui::timeLowerBoundChangedSlot( int channel, double value ) {

      ChannelConfig* c =
            m_group.getChannel( channel -
                                ( m_group.getGroupNumber() *
                                  GroupConfig::CHANNELS_IN_GROUP ) );
      c->setTimeLowerBound( value );
      return;
   }

   void GroupGui::timeUpperBoundChangedSlot( int channel, double value ) {

      ChannelConfig* c =
            m_group.getChannel( channel -
                                ( m_group.getGroupNumber() *
                                  GroupConfig::CHANNELS_IN_GROUP ) );
      c->setTimeUpperBound( value );
      return;
   }

   void GroupGui::energyNameChangedSlot( int channel, const QString& text ) {

      ChannelConfig* c =
            m_group.getChannel( channel -
                                ( m_group.getGroupNumber() *
                                  GroupConfig::CHANNELS_IN_GROUP ) );
      c->setEnergyName( text );
      return;
   }

   void GroupGui::energyChannelsChangedSlot( int channel, int channels ) {

      ChannelConfig* c =
            m_group.getChannel( channel -
                                ( m_group.getGroupNumber() *
                                  GroupConfig::CHANNELS_IN_GROUP ) );
      c->setEnergyNumberOfChannels( channels );
      return;
   }

   void GroupGui::energyLowerBoundChangedSlot( int channel, double value ) {

      ChannelConfig* c =
            m_group.getChannel( channel -
                                ( m_group.getGroupNumber() *
                                  GroupConfig::CHANNELS_IN_GROUP ) );
      c->setEnergyLowerBound( value );
      return;
   }

   void GroupGui::energyUpperBoundChangedSlot( int channel, double value ) {

      ChannelConfig* c =
            m_group.getChannel( channel -
                                ( m_group.getGroupNumber() *
                                  GroupConfig::CHANNELS_IN_GROUP ) );
      c->setEnergyUpperBound( value );
      return;
   }

} // namespace dt5740
