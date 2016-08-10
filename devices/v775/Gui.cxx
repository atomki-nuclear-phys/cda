// $Id$

// CDA include(s):
#include "common/errorcheck.h"

// Local include(s):
#include "Gui.h"

namespace v775 {

   /// Format for the FSR register feedback label
   static const char* FSR_LABEL_VALUE = "Full range: %1 ns  Step value: %2 ps";

   Gui::Gui( QWidget* parent, Qt::WindowFlags flags )
   : dev::CaenVmeGui( parent, flags ),
   m_logger( "v775::Gui" ) {

      //
      // Create the widget that will hold all the configuration widgets:
      //
      m_scrollWidget.reset( new QWidget( 0, flags ) );
      m_scrollWidget->setGeometry( QRect( 0, 0, WIDGET_WIDTH - 20, 2010 ) );

      //
      // Embed the previous widget into a scroll area:
      //
      m_scrollArea.reset( new QScrollArea( this ) );
      m_scrollArea->setGeometry( QRect( 0, 0, WIDGET_WIDTH, WIDGET_HEIGHT ) );
      m_scrollArea->setWidget( m_scrollWidget.get() );

      //
      // Put a picture of the device at the top-left:
      //
      m_image.reset( new QLabel( m_scrollWidget.get() ) );
      m_image->setGeometry( QRect( 10, 10, 100, 325 ) );
      QPixmap pixmap( ":/img/v775.png" );
#if QT_VERSION >= QT_VERSION_CHECK( 5, 1, 0 )
      pixmap.setDevicePixelRatio( m_image->devicePixelRatio() );
#endif // QT_VERSION
      m_image->setPixmap( pixmap );
      m_image->setScaledContents( true );

      //
      // Create a label telling us what kind of device this is:
      //
      m_topLabel.reset( new QLabel( tr( "CAEN V775 32 Channel Multievent TDC" ),
                                    m_scrollWidget.get() ) );
      m_topLabel->setAlignment( Qt::AlignCenter );
      m_topLabel->setGeometry( QRect( 120, 20, WIDGET_WIDTH - 150, 35 ) );
      QFont titleFont( m_topLabel->font() );
      titleFont.setPointSize( titleFont.pointSize() + 2 );
      titleFont.setBold( true );
      m_topLabel->setFont( titleFont );
      m_topLabel->setWordWrap( true );

      //
      // Create the device level settings:
      //
      m_deviceSettingsBox.reset( new QGroupBox( tr( "Device properties" ),
                                                m_scrollWidget.get() ) );
      m_deviceSettingsBox->setGeometry( QRect( 130, 80,
                                               WIDGET_WIDTH - 150, 260 ) );

      m_vmeAddressLabel.reset( new QLabel( tr( "VME Address (hex)" ),
                                           m_deviceSettingsBox.get() ) );
      m_vmeAddressLabel->setGeometry( QRect( 10, 35, 150, 25 ) );

      m_vmeAddressEdit.reset( new QLineEdit( "0", m_deviceSettingsBox.get() ) );
      m_vmeAddressEdit->setGeometry( QRect( 170, 35, 100, 25 ) );
      connect( m_vmeAddressEdit.get(), SIGNAL( textChanged( QString ) ),
               this, SLOT( vmeAddressSlot( QString ) ) );

      m_zeroSuppressionEdit.reset(
               new QCheckBox( tr( "Zero suppression enabled" ),
                             m_deviceSettingsBox.get() ) );
      m_zeroSuppressionEdit->setGeometry( QRect( 10, 60, 300, 25 ) );
      connect( m_zeroSuppressionEdit.get(), SIGNAL( toggled( bool ) ),
               this, SLOT( zeroSuppressionSlot( bool ) ) );

      m_overflowSuppressionEdit.reset(
               new QCheckBox( tr( "Overflow suppression enabled" ),
                              m_deviceSettingsBox.get() ) );
      m_overflowSuppressionEdit->setGeometry( QRect( 10, 85, 300, 25 ) );
      connect( m_overflowSuppressionEdit.get(), SIGNAL( toggled( bool ) ),
               this, SLOT( overflowSuppressionSlot( bool ) ) );

      m_validSuppressionEdit.reset(
               new QCheckBox( tr( "Valid suppression enabled" ),
                              m_deviceSettingsBox.get() ) );
      m_validSuppressionEdit->setGeometry( QRect( 10, 110, 300, 25 ) );
      connect( m_validSuppressionEdit.get(), SIGNAL( toggled( bool ) ),
               this, SLOT( validSuppressionSlot( bool ) ) );

      m_commonStopLabel.reset(
               new QLabel( tr( "Common gate" ), m_deviceSettingsBox.get() ) );
      m_commonStopLabel->setGeometry( QRect( 10, 135, 140, 25 ) );

      m_commonStopEdit.reset( new QComboBox( m_deviceSettingsBox.get() ) );
      m_commonStopEdit->setGeometry( QRect( 150, 135, 150, 25 ) );
      m_commonStopEdit->addItem( tr( "Common Start" ) );
      m_commonStopEdit->addItem( tr( "Common Stop" ) );
      connect( m_commonStopEdit.get(), SIGNAL( currentIndexChanged( int ) ),
               this, SLOT( commonStopSlot( int ) ) );

      m_fullScaleRangeBox.reset( new QGroupBox( tr( "Full Scale Range" ),
                                                m_deviceSettingsBox.get() ) );
      m_fullScaleRangeBox->setGeometry( QRect( 10, 165, WIDGET_WIDTH - 170,
                                               85 ) );

      m_fullScaleRangeLabel.reset( new QLabel( tr( "Register value" ),
                                               m_fullScaleRangeBox.get() ) );
      m_fullScaleRangeLabel->setGeometry( QRect( 10, 30, 140, 25 ) );

      m_fullScaleRangeEdit.reset( new QSpinBox( m_fullScaleRangeBox.get() ) );
      m_fullScaleRangeEdit->setGeometry( QRect( 150, 30, 100, 25 ) );
      m_fullScaleRangeEdit->setRange( 0x1e, 0xff );
      m_fullScaleRangeEdit->setValue( m_fullScaleRangeValue );
#if QT_VERSION >= QT_VERSION_CHECK( 5, 2, 0 )
      m_fullScaleRangeEdit->setDisplayIntegerBase( 16 );
#endif
      connect( m_fullScaleRangeEdit.get(), SIGNAL( valueChanged( int ) ),
               this, SLOT( fullScaleRangeSlot( int ) ) );

      const double stepValue = 8900.0 / m_fullScaleRangeValue;
      const double fullRange = stepValue * 4.096;
      m_fullScaleRangeRealValue.reset(
               new QLabel( tr( FSR_LABEL_VALUE )
                           .arg( fullRange, 0, 'f', 0 )
                           .arg( stepValue, 0, 'f', 0 ),
                           m_fullScaleRangeBox.get() ) );
      m_fullScaleRangeRealValue->setGeometry( QRect( 10, 55, 300, 25 ) );

      //
      // Create the input channel settings:
      //
      m_channelSettingsBox.reset( new QGroupBox( tr( "Input channels" ),
                                                 m_scrollWidget.get() ) );
      m_channelSettingsBox->setGeometry( QRect( 10, 350, WIDGET_WIDTH - 30,
                                                1650 ) );
      m_channelNameLabel.reset( new QLabel( tr( "Name" ),
                                            m_channelSettingsBox.get() ) );
      m_channelNameLabel->setGeometry( QRect( 135, 25, 75, 25 ) );
      m_channelNameLabel->setAlignment( Qt::AlignCenter );
      m_channelNBinsLabel.reset( new QLabel( tr( "NBins" ),
                                             m_channelSettingsBox.get() ) );
      m_channelNBinsLabel->setGeometry( QRect( 215, 25, 75, 25 ) );
      m_channelNBinsLabel->setAlignment( Qt::AlignCenter );
      m_channelLowerBoundLabel.reset(
               new QLabel( tr( "Lower" ), m_channelSettingsBox.get() ) );
      m_channelLowerBoundLabel->setGeometry( QRect( 295, 25, 75, 25 ) );
      m_channelLowerBoundLabel->setAlignment( Qt::AlignCenter );
      m_channelUpperBoundLabel.reset(
               new QLabel( tr( "Upper" ), m_channelSettingsBox.get() ) );
      m_channelUpperBoundLabel->setGeometry( QRect( 375, 25, 75, 25 ) );
      m_channelUpperBoundLabel->setAlignment( Qt::AlignCenter );

      for( size_t i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
         m_gchannels[ i ].reset( new ChannelGui( i,
                                                 m_channelSettingsBox.get() ) );
         m_gchannels[ i ]->setGeometry( QRect( 20, 50 + i * ChannelGui::HEIGHT,
                                               ChannelGui::WIDTH - 40,
                                               ChannelGui::HEIGHT ) );
         connect( m_gchannels[ i ].get(), SIGNAL( enableChanged( int, bool ) ),
                  this, SLOT( channelEnabledSlot( int, bool ) ) );
         connect( m_gchannels[ i ].get(), SIGNAL( nameChanged( int, QString ) ),
                  this, SLOT( nameChangedSlot( int, QString ) ) );
         connect( m_gchannels[ i ].get(), SIGNAL( nbinsChanged( int, int ) ),
                  this, SLOT( nbinsChangedSlot( int, int ) ) );
         connect( m_gchannels[ i ].get(),
                  SIGNAL( lowerBoundChanged( int, double ) ),
                  this, SLOT( lowerBoundChangedSlot( int, double ) ) );
         connect( m_gchannels[ i ].get(),
                  SIGNAL( upperBoundChanged( int, double ) ),
                  this, SLOT( upperBoundChangedSlot( int, double ) ) );
         connect( m_gchannels[ i ].get(),
                  SIGNAL( thresholdChanged( int, int ) ),
                  this, SLOT( thresholdChangedSlot( int, int ) ) );
      }
   }

   StatusCode Gui::readConfig( QIODevice& dev ) {

      CHECK( Device::readConfig( dev ) );
      sync();
      return StatusCode::SUCCESS;
   }

   StatusCode Gui::readConfig( const QDomElement& node ) {

      CHECK( Device::readConfig( node ) );
      sync();
      return StatusCode::SUCCESS;
   }

   void Gui::vmeAddressSlot( const QString& value ) {

      // The content should be a hexadecimal number:
      bool ok = true;
      m_vmeAddress = value.toInt( &ok, 16 );
      if( ! ok ) {
         REPORT_ERROR( tr( "Couldn't parse \"%1\" into hex value" )
                       .arg( value ) );
      } else {
         emit addressChanged();
      }

      return;
   }

   void Gui::zeroSuppressionSlot( bool value ) {

      m_zeroSuppressionEnabled = value;
      return;
   }

   void Gui::overflowSuppressionSlot( bool value ) {

      m_overflowSuppressionEnabled = value;
      return;
   }

   void Gui::validSuppressionSlot( bool value ) {

      m_validSuppressionEnabled = value;
      return;
   }

   void Gui::commonStopSlot( int index ) {

      m_commonStopEnabled = ( index == 1 );
      return;
   }

   void Gui::fullScaleRangeSlot( int value ) {

      // Set the value:
      m_fullScaleRangeValue = value;

      // Update the label:
      const double stepValue = 8900.0 / m_fullScaleRangeValue;
      const double fullRange = stepValue * 4.096;
      m_fullScaleRangeRealValue->setText( tr( FSR_LABEL_VALUE )
                                          .arg( fullRange, 0, 'f', 0 )
                                          .arg( stepValue, 0, 'f', 0 ) );
      return;
   }

   void Gui::channelEnabledSlot( int channel, bool on ) {

      if( on ) {
         m_channels[ channel ].reset( new ChannelConfig() );
         m_channels[ channel ]->setChannel( channel );
      } else {
         m_channels[ channel ].reset();
      }

      return;
   }

   void Gui::nameChangedSlot( int channel, const QString& text ) {

      m_channels[ channel ]->setName( text );
      return;
   }

   void Gui::nbinsChangedSlot( int channel, int channels ) {

      m_channels[ channel ]->setNBins( channels );
      return;
   }

   void Gui::lowerBoundChangedSlot( int channel, double value ) {

      m_channels[ channel ]->setLowerBound( value );
      return;
   }

   void Gui::upperBoundChangedSlot( int channel, double value ) {

      m_channels[ channel ]->setUpperBound( value );
      return;
   }

   void Gui::thresholdChangedSlot( int channel, int value ) {

      m_channels[ channel ]->setThreshold( value );
      return;
   }

   void Gui::sync() {

      // Set the device properties:
      m_vmeAddressEdit->setText( QString( "%1" ).arg( m_vmeAddress, 4, 16,
                                                      QChar( '0' ) ) );
      m_zeroSuppressionEdit->setChecked( m_zeroSuppressionEnabled );
      m_overflowSuppressionEdit->setChecked( m_overflowSuppressionEnabled );
      m_validSuppressionEdit->setChecked( m_validSuppressionEnabled );
      m_commonStopEdit->setCurrentIndex( m_commonStopEnabled ? 1 : 0 );
      m_fullScaleRangeEdit->setValue( m_fullScaleRangeValue );

      // Set the channel properties:
      for( size_t i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
         if( m_channels[ i ] ) {
            m_gchannels[ i ]->setEnabled( false );
            m_gchannels[ i ]->setName( m_channels[ i ]->getName() );
            m_gchannels[ i ]->setNBins( m_channels[ i ]->getNBins() );
            m_gchannels[ i ]->setLowerBound( m_channels[ i ]->getLowerBound() );
            m_gchannels[ i ]->setUpperBound( m_channels[ i ]->getUpperBound() );
            m_gchannels[ i ]->setThreshold( m_channels[ i ]->getThreshold() );
            m_gchannels[ i ]->setEnabled( true );
         } else {
            m_gchannels[ i ]->setEnabled( false );
         }
      }

      return;
   }

} // namespace v775
