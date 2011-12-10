// $Id$

// Qt include(s):
#include <QtCore/QRect>
#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QDoubleSpinBox>

// Local include(s):
#include "ChannelGui.h"

namespace dt5740 {

   const int ChannelGui::WIDTH  = 430;
   const int ChannelGui::HEIGHT = 25;

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
      m_enabledEdit = new QCheckBox( QString::number( m_channelNumber ) +
                                     tr( ". channel" ), this );
      m_enabledEdit->setGeometry( QRect( 0, 0, 110, 25 ) );
      connect( m_enabledEdit, SIGNAL( toggled( bool ) ),
               this, SLOT( enableChangedSlot( bool ) ) );

      //
      // Create the widget changing the name of the channel:
      //
      m_nameEdit = new QLineEdit( this );
      m_nameEdit->setEnabled( false );
      m_nameEdit->setGeometry( QRect( 115, 2, 75, 21 ) );
      connect( m_nameEdit, SIGNAL( textChanged( const QString& ) ),
               this, SLOT( nameChangedSlot( const QString& ) ) );

      //
      // Create the widget changing the number of channels in the
      // monitoring histogram:
      //
      m_channelsEdit = new QSpinBox( this );
      m_channelsEdit->setEnabled( false );
      m_channelsEdit->setGeometry( QRect( 195, 0, 75, 25 ) );
      m_channelsEdit->setRange( 10, 4096 );
      m_channelsEdit->setValue( 4096 );
      connect( m_channelsEdit, SIGNAL( valueChanged( int ) ),
               this, SLOT( channelsChangedSlot( int ) ) );

      //
      // Create the widget changing the lower bound of the monitoring
      // histogram:
      //
      m_lowerBoundEdit = new QDoubleSpinBox( this );
      m_lowerBoundEdit->setEnabled( false );
      m_lowerBoundEdit->setGeometry( QRect( 275, 0, 75, 25 ) );
      m_lowerBoundEdit->setDecimals( 1 );
      m_lowerBoundEdit->setRange( -100000., 100000. );
      m_lowerBoundEdit->setValue( 0. );
      connect( m_lowerBoundEdit, SIGNAL( valueChanged( double ) ),
               this, SLOT( lowerBoundChangedSlot( double ) ) );

      //
      // Create the widget changing the upper bound of the monitoring
      // histogram:
      //
      m_upperBoundEdit = new QDoubleSpinBox( this );
      m_upperBoundEdit->setEnabled( false );
      m_upperBoundEdit->setGeometry( QRect( 355, 0, 75, 25 ) );
      m_upperBoundEdit->setDecimals( 1 );
      m_upperBoundEdit->setRange( -100000., 100000. );
      m_upperBoundEdit->setValue( 1024. );
      connect( m_upperBoundEdit, SIGNAL( valueChanged( double ) ),
               this, SLOT( upperBoundChangedSlot( double ) ) );

   }

   /**
    * The destructor deletes all the constituent widgets.
    */
   ChannelGui::~ChannelGui() {

      delete m_enabledEdit;
      delete m_nameEdit;
      delete m_channelsEdit;
      delete m_lowerBoundEdit;
      delete m_upperBoundEdit;

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
      m_nameEdit->setEnabled( on );
      m_channelsEdit->setEnabled( on );
      m_lowerBoundEdit->setEnabled( on );
      m_upperBoundEdit->setEnabled( on );
      return;

   }

   void ChannelGui::setName( const QString& text ) {

      m_nameEdit->setText( text );
      return;

   }

   void ChannelGui::setChannels( int channels ) {

      m_channelsEdit->setValue( channels );
      return;

   }

   void ChannelGui::setLowerBound( double value ) {

      m_lowerBoundEdit->setValue( value );
      return;

   }

   void ChannelGui::setUpperBound( double value ) {

      m_upperBoundEdit->setValue( value );
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

      m_nameEdit->setEnabled( on );
      m_channelsEdit->setEnabled( on );
      m_lowerBoundEdit->setEnabled( on );
      m_upperBoundEdit->setEnabled( on );

      emit enableChanged( m_channelNumber, on );
      if( on ) {
         emit nameChanged( m_channelNumber, m_nameEdit->text() );
         emit channelsChanged( m_channelNumber, m_channelsEdit->value() );
         emit lowerBoundChanged( m_channelNumber, m_lowerBoundEdit->value() );
         emit upperBoundChanged( m_channelNumber, m_upperBoundEdit->value() );
      }

      return;

   }

   void ChannelGui::nameChangedSlot( const QString& text ) {

      emit nameChanged( m_channelNumber, text );
      return;

   }

   void ChannelGui::channelsChangedSlot( int channels ) {

      emit channelsChanged( m_channelNumber, channels );
      return;

   }

   void ChannelGui::lowerBoundChangedSlot( double value ) {

      emit lowerBoundChanged( m_channelNumber, value );
      return;

   }

   void ChannelGui::upperBoundChangedSlot( double value ) {

      emit upperBoundChanged( m_channelNumber, value );
      return;

   }

} // namespace dt5740
