// $Id$

// Qt include(s):
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QDoubleSpinBox>

// Local include(s):
#include "ChannelGui.h"

namespace dt5740 {

   const int ChannelGui::WIDTH  = 430;
   const int ChannelGui::HEIGHT = 75;

   /**
    * The constructor sets up the look of the widget and connects up
    * all the signals and slots.
    *
    * @param channel The channel's number in the device
    * @param parent The Qt parent of the widget
    * @param flags The Qt flags given to the widget
    */
   ChannelGui::ChannelGui( int channel, QWidget* parent,
                           Qt::WindowFlags flags )
      : QWidget( parent, flags ), m_channelNumber( channel ) {

      //
      // Set the fixed size for the widget:
      //
      resize( WIDTH, HEIGHT );
      setMinimumSize( WIDTH, HEIGHT );
      setMaximumSize( WIDTH, HEIGHT );

      //
      // Create the widget enabling/disabling the channel:
      //
      m_enabledEdit = new QCheckBox( tr( "%1. channel" ).arg( m_channelNumber ),
                                     this );
      m_enabledEdit->setGeometry( QRect( 0, 0, 110, 25 ) );
      connect( m_enabledEdit, SIGNAL( toggled( bool ) ),
               this, SLOT( enableChangedSlot( bool ) ) );

      //
      // Create the widget editing the "raw name" of the channel:
      //
      m_rawNameEdit = new QLineEdit( this );
      m_rawNameEdit->setEnabled( false );
      m_rawNameEdit->setGeometry( QRect( 115, 2, 75, 22 ) );
      connect( m_rawNameEdit, SIGNAL( textChanged( const QString& ) ),
               this, SLOT( rawNameChangedSlot( const QString& ) ) );

      //
      // Create a description for the time channel:
      //
      m_timeLabel = new QLabel( tr( "Time" ),
                                this );
      m_timeLabel->setGeometry( QRect( 15, 25, 95, 25 ) );
      m_timeLabel->setEnabled( false );

      //
      // Create the widget changing the name of the time channel:
      //
      m_timeNameEdit = new QLineEdit( this );
      m_timeNameEdit->setEnabled( false );
      m_timeNameEdit->setGeometry( QRect( 115, 27, 75, 22 ) );
      connect( m_timeNameEdit, SIGNAL( textChanged( const QString& ) ),
               this, SLOT( timeNameChangedSlot( const QString& ) ) );

      //
      // Create the widget changing the number of channels in the
      // time monitoring histogram:
      //
      m_timeChannelsEdit = new QSpinBox( this );
      m_timeChannelsEdit->setEnabled( false );
      m_timeChannelsEdit->setGeometry( QRect( 195, 25, 75, 25 ) );
      m_timeChannelsEdit->setRange( 10, 4096 );
      m_timeChannelsEdit->setValue( 4096 );
      connect( m_timeChannelsEdit, SIGNAL( valueChanged( int ) ),
               this, SLOT( timeChannelsChangedSlot( int ) ) );

      //
      // Create the widget changing the lower bound of the
      // time monitoring histogram:
      //
      m_timeLowerBoundEdit = new QDoubleSpinBox( this );
      m_timeLowerBoundEdit->setEnabled( false );
      m_timeLowerBoundEdit->setGeometry( QRect( 275, 25, 75, 25 ) );
      m_timeLowerBoundEdit->setDecimals( 1 );
      m_timeLowerBoundEdit->setRange( -100000., 100000. );
      m_timeLowerBoundEdit->setValue( 0. );
      connect( m_timeLowerBoundEdit, SIGNAL( valueChanged( double ) ),
               this, SLOT( timeLowerBoundChangedSlot( double ) ) );

      //
      // Create the widget changing the upper bound of the
      // time monitoring histogram:
      //
      m_timeUpperBoundEdit = new QDoubleSpinBox( this );
      m_timeUpperBoundEdit->setEnabled( false );
      m_timeUpperBoundEdit->setGeometry( QRect( 355, 25, 75, 25 ) );
      m_timeUpperBoundEdit->setDecimals( 1 );
      m_timeUpperBoundEdit->setRange( -100000., 100000. );
      m_timeUpperBoundEdit->setValue( 4096. );
      connect( m_timeUpperBoundEdit, SIGNAL( valueChanged( double ) ),
               this, SLOT( timeUpperBoundChangedSlot( double ) ) );

      //
      // Create a description for the energy channel:
      //
      m_energyLabel = new QLabel( tr( "Energy" ),
                                  this );
      m_energyLabel->setGeometry( QRect( 15, 50, 95, 25 ) );
      m_energyLabel->setEnabled( false );

      //
      // Create the widget changing the name of the energy channel:
      //
      m_energyNameEdit = new QLineEdit( this );
      m_energyNameEdit->setEnabled( false );
      m_energyNameEdit->setGeometry( QRect( 115, 52, 75, 22 ) );
      connect( m_energyNameEdit, SIGNAL( textChanged( const QString& ) ),
               this, SLOT( energyNameChangedSlot( const QString& ) ) );

      //
      // Create the widget changing the number of channels in the
      // energy monitoring histogram:
      //
      m_energyChannelsEdit = new QSpinBox( this );
      m_energyChannelsEdit->setEnabled( false );
      m_energyChannelsEdit->setGeometry( QRect( 195, 50, 75, 25 ) );
      m_energyChannelsEdit->setRange( 10, 4096 );
      m_energyChannelsEdit->setValue( 4096 );
      connect( m_energyChannelsEdit, SIGNAL( valueChanged( int ) ),
               this, SLOT( energyChannelsChangedSlot( int ) ) );

      //
      // Create the widget changing the lower bound of the
      // energy monitoring histogram:
      //
      m_energyLowerBoundEdit = new QDoubleSpinBox( this );
      m_energyLowerBoundEdit->setEnabled( false );
      m_energyLowerBoundEdit->setGeometry( QRect( 275, 50, 75, 25 ) );
      m_energyLowerBoundEdit->setDecimals( 1 );
      m_energyLowerBoundEdit->setRange( -100000., 100000. );
      m_energyLowerBoundEdit->setValue( 0. );
      connect( m_energyLowerBoundEdit, SIGNAL( valueChanged( double ) ),
               this, SLOT( energyLowerBoundChangedSlot( double ) ) );

      //
      // Create the widget changing the upper bound of the
      // energy monitoring histogram:
      //
      m_energyUpperBoundEdit = new QDoubleSpinBox( this );
      m_energyUpperBoundEdit->setEnabled( false );
      m_energyUpperBoundEdit->setGeometry( QRect( 355, 50, 75, 25 ) );
      m_energyUpperBoundEdit->setDecimals( 1 );
      m_energyUpperBoundEdit->setRange( -100000., 100000. );
      m_energyUpperBoundEdit->setValue( 4096. );
      connect( m_energyUpperBoundEdit, SIGNAL( valueChanged( double ) ),
               this, SLOT( energyUpperBoundChangedSlot( double ) ) );
   }

   /**
    * The destructor deletes all the constituent widgets.
    */
   ChannelGui::~ChannelGui() {

      delete m_enabledEdit;
      delete m_rawNameEdit;

      delete m_timeLabel;
      delete m_timeNameEdit;
      delete m_timeChannelsEdit;
      delete m_timeLowerBoundEdit;
      delete m_timeUpperBoundEdit;

      delete m_energyLabel;
      delete m_energyNameEdit;
      delete m_energyChannelsEdit;
      delete m_energyLowerBoundEdit;
      delete m_energyUpperBoundEdit;
   }

   int ChannelGui::getChannelNumber() const {

      return m_channelNumber;

   }

   void ChannelGui::setChannelNumber( int value ) {

      m_channelNumber = value;
      return;

   }

   /**
    * This function is normally called from the outside to change the
    * state of the channel. Normally when reading the device configuration
    * form file.
    */
   void ChannelGui::setEnabled( bool on ) {

      m_enabledEdit->setChecked( on );
      m_rawNameEdit->setEnabled( on );

      m_timeLabel->setEnabled( on );
      m_timeNameEdit->setEnabled( on );
      m_timeChannelsEdit->setEnabled( on );
      m_timeLowerBoundEdit->setEnabled( on );
      m_timeUpperBoundEdit->setEnabled( on );

      m_energyLabel->setEnabled( on );
      m_energyNameEdit->setEnabled( on );
      m_energyChannelsEdit->setEnabled( on );
      m_energyLowerBoundEdit->setEnabled( on );
      m_energyUpperBoundEdit->setEnabled( on );

      return;
   }

   void ChannelGui::setRawName( const QString& text ) {

      m_rawNameEdit->setText( text );
      return;
   }

   void ChannelGui::setTimeName( const QString& text ) {

      m_timeNameEdit->setText( text );
      return;
   }

   void ChannelGui::setTimeChannels( int channels ) {

      m_timeChannelsEdit->setValue( channels );
      return;
   }

   void ChannelGui::setTimeLowerBound( double value ) {

      m_timeLowerBoundEdit->setValue( value );
      return;
   }

   void ChannelGui::setTimeUpperBound( double value ) {

      m_timeUpperBoundEdit->setValue( value );
      return;
   }

   void ChannelGui::setEnergyName( const QString& text ) {

      m_energyNameEdit->setText( text );
      return;
   }

   void ChannelGui::setEnergyChannels( int channels ) {

      m_energyChannelsEdit->setValue( channels );
      return;
   }

   void ChannelGui::setEnergyLowerBound( double value ) {

      m_energyLowerBoundEdit->setValue( value );
      return;
   }

   void ChannelGui::setEnergyUpperBound( double value ) {

      m_energyUpperBoundEdit->setValue( value );
      return;
   }

   /**
    * This slot has to be a bit tricky. When you enable a channel,
    * you have to first enable the widgets changing the channel's
    * properties. Then, you have to make sure that the configuration
    * object has the same values as the ones shown, so the signals
    * from all the widgets have to be sent as if the values of the
    * widgets would've changed. But this can only be done when the
    * channel is turned on! (Otherwise you would try to change the
    * settings of a non-existent channel.)
    *
    * @param on State of the channel
    */
   void ChannelGui::enableChangedSlot( bool on ) {

      m_rawNameEdit->setEnabled( on );

      m_timeLabel->setEnabled( on );
      m_timeNameEdit->setEnabled( on );
      m_timeChannelsEdit->setEnabled( on );
      m_timeLowerBoundEdit->setEnabled( on );
      m_timeUpperBoundEdit->setEnabled( on );

      m_energyLabel->setEnabled( on );
      m_energyNameEdit->setEnabled( on );
      m_energyChannelsEdit->setEnabled( on );
      m_energyLowerBoundEdit->setEnabled( on );
      m_energyUpperBoundEdit->setEnabled( on );

      emit enableChanged( m_channelNumber, on );
      if( on ) {
         emit rawNameChanged( m_channelNumber, m_rawNameEdit->text() );

         emit timeNameChanged( m_channelNumber, m_timeNameEdit->text() );
         emit timeChannelsChanged( m_channelNumber, m_timeChannelsEdit->value() );
         emit timeLowerBoundChanged( m_channelNumber, m_timeLowerBoundEdit->value() );
         emit timeUpperBoundChanged( m_channelNumber, m_timeUpperBoundEdit->value() );

         emit energyNameChanged( m_channelNumber, m_energyNameEdit->text() );
         emit energyChannelsChanged( m_channelNumber, m_energyChannelsEdit->value() );
         emit energyLowerBoundChanged( m_channelNumber, m_energyLowerBoundEdit->value() );
         emit energyUpperBoundChanged( m_channelNumber, m_energyUpperBoundEdit->value() );
      }

      return;
   }

   void ChannelGui::rawNameChangedSlot( const QString& text ) {

      emit rawNameChanged( m_channelNumber, text );
   }

   void ChannelGui::timeNameChangedSlot( const QString& text ) {

      emit timeNameChanged( m_channelNumber, text );
      return;
   }

   void ChannelGui::timeChannelsChangedSlot( int channels ) {

      emit timeChannelsChanged( m_channelNumber, channels );
      return;
   }

   void ChannelGui::timeLowerBoundChangedSlot( double value ) {

      emit timeLowerBoundChanged( m_channelNumber, value );
      return;
   }

   void ChannelGui::timeUpperBoundChangedSlot( double value ) {

      emit timeUpperBoundChanged( m_channelNumber, value );
      return;
   }

   void ChannelGui::energyNameChangedSlot( const QString& text ) {

      emit energyNameChanged( m_channelNumber, text );
      return;
   }

   void ChannelGui::energyChannelsChangedSlot( int channels ) {

      emit energyChannelsChanged( m_channelNumber, channels );
      return;
   }

   void ChannelGui::energyLowerBoundChangedSlot( double value ) {

      emit energyLowerBoundChanged( m_channelNumber, value );
      return;
   }

   void ChannelGui::energyUpperBoundChangedSlot( double value ) {

      emit energyUpperBoundChanged( m_channelNumber, value );
      return;
   }

} // namespace dt5740
