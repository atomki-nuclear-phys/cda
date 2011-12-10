// $Id$

// Qt include(s):
#include <QtCore/QRect>
#include <QtCore/QLine>
#include <QtGui/QFont>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QScrollArea>

// Local include(s):
#include "Gui.h"
#include "ChannelGui.h"

namespace dt5740 {

   /**
    * The constructor creates all member objects and arranges them
    * in the area given to the device.
    *
    * @param parent The Qt parent of the widget
    * @param flags The Qt flags given to the widget
    */
   Gui::Gui( QWidget* parent, Qt::WindowFlags flags )
      : dev::CaenGui( parent, flags ) {

      //
      // Create the widget that will hold all the configuration widgets:
      //
      m_scrollWidget = new QWidget( 0, flags );
      m_scrollWidget->setGeometry( QRect( 0, 0, WIDGET_WIDTH - 20, 900 ) );

      //
      // Embed the previous widget into a scroll area:
      //
      m_scrollArea = new QScrollArea( this );
      m_scrollArea->setGeometry( QRect( 0, 0, WIDGET_WIDTH, WIDGET_HEIGHT ) );
      m_scrollArea->setWidget( m_scrollWidget );

      //
      // Create a label telling us what kind of device this is:
      //
      m_topLabel = new QLabel( tr( "CAEN DT5740 32 Channel 12 bit 62.5MS/s Digitizer" ),
                               m_scrollWidget );
      m_topLabel->setAlignment( Qt::AlignCenter );
      m_topLabel->setGeometry( QRect( 25, 30, 430, 25 ) );

      //
      // Create a label identifying the channel name settings:
      //
      m_nameLabel = new QLabel( tr( "Name" ), m_scrollWidget );
      m_nameLabel->setAlignment( Qt::AlignCenter );
      m_nameLabel->setGeometry( QRect( 140, 75, 75, 25 ) );

      //
      // Create a label identifying the histogram channel settings:
      //
      m_channelsLabel = new QLabel( tr( "Channels" ), m_scrollWidget );
      m_channelsLabel->setAlignment( Qt::AlignCenter );
      m_channelsLabel->setGeometry( QRect( 220, 75, 75, 25 ) );

      //
      // Create a label identifying the histogram lower bound settings:
      //
      m_lowerBoundLabel = new QLabel( tr( "Lower" ), m_scrollWidget );
      m_lowerBoundLabel->setAlignment( Qt::AlignCenter );
      m_lowerBoundLabel->setGeometry( QRect( 300, 75, 75, 25 ) );

      //
      // Create a label identifying the histogram upper bound settings:
      //
      m_upperBoundLabel = new QLabel( tr( "Upper" ), m_scrollWidget );
      m_upperBoundLabel->setAlignment( Qt::AlignCenter );
      m_upperBoundLabel->setGeometry( QRect( 380, 75, 75, 25 ) );

      //
      // Create the widgets modifying the settings of the input channels:
      //
      for( int i = 0; i < NUMBER_OF_CHANNELS; ++i ) {

         m_gchannels[ i ] = new ChannelGui( i, m_scrollWidget );
         m_gchannels[ i ]->setGeometry( QRect( 25, 100 + i * 25,
                                               ChannelGui::WIDTH,
                                               ChannelGui::HEIGHT ) );
         connect( m_gchannels[ i ], SIGNAL( enableChanged( int, bool ) ),
                  this, SLOT( channelEnabledSlot( int, bool ) ) );
         connect( m_gchannels[ i ],
                  SIGNAL( nameChanged( int, const QString& ) ),
                  this, SLOT( nameChangedSlot( int, const QString& ) ) );
         connect( m_gchannels[ i ], SIGNAL( channelsChanged( int, int ) ),
                  this, SLOT( channelsChangedSlot( int, int ) ) );
         connect( m_gchannels[ i ], SIGNAL( lowerBoundChanged( int, double ) ),
                  this, SLOT( lowerBoundChangedSlot( int, double ) ) );
         connect( m_gchannels[ i ], SIGNAL( upperBoundChanged( int, double ) ),
                  this, SLOT( upperBoundChangedSlot( int, double ) ) );

      }

   }

   /**
    * The destructor deletes all the objects created in the constructor.
    */
   Gui::~Gui() {

      delete m_topLabel;

      delete m_nameLabel;
      delete m_channelsLabel;
      delete m_lowerBoundLabel;
      delete m_upperBoundLabel;

      for( int i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
         delete m_gchannels[ i ];
      }

      delete m_scrollWidget;
      delete m_scrollArea;
   }

   /**
    * The function calls the ad2249a::Device::readConfig(QIODevice*) function
    * to read the device configuration and then calls sync() to show the
    * new configuration correctly.
    */
   bool Gui::readConfig( QIODevice* dev ) {

      if( ! Device::readConfig( dev ) ) {
         return false;
      }
      sync();
      return true;
   }

   /**
    * The function calls the ad2249a::Device::readConfig(QDomElement&) function
    * to read the device configuration and then calls sync() to show the
    * new configuration correctly.
    */
   bool Gui::readConfig( const QDomElement& node ) {

      if( ! Device::readConfig( node ) ) {
         return false;
      }
      sync();
      return true;
   }

   void Gui::channelEnabledSlot( int subaddress, bool on ) {

      if( on ) {
         m_channels[ subaddress ] = new ChannelConfig();
         m_channels[ subaddress ]->setChannelNumber( subaddress );
      } else {
         delete m_channels[ subaddress ];
         m_channels[ subaddress ] = 0;
      }

      return;
   }

   void Gui::nameChangedSlot( int subaddress, const QString& text ) {

      m_channels[ subaddress ]->setName( text );
      return;
   }

   void Gui::channelsChangedSlot( int subaddress, int channels ) {

      m_channels[ subaddress ]->setNumberOfChannels( channels );
      return;
   }

   void Gui::lowerBoundChangedSlot( int subaddress, double value ) {

      m_channels[ subaddress ]->setLowerBound( value );
      return;
   }

   void Gui::upperBoundChangedSlot( int subaddress, double value ) {

      m_channels[ subaddress ]->setUpperBound( value );
      return;
   }

   /**
    * After a configuration is read from file, the graphical objects have
    * to be sync-ed to show this new configuration. This function takes
    * care of doing that.
    */
   void Gui::sync() {

      //
      // For each subaddress the adjustments to the graphical objects
      // have to be made in a tricky way. The changes have to be made
      // while the graphical channel is disabled. Otherwise the signals
      // sent from the channel as its values are changing would create
      // "strange" effects.
      //
      for( int i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
         if( m_channels[ i ] ) {
            m_gchannels[ i ]->setEnabled( false );
            m_gchannels[ i ]->setName( m_channels[ i ]->getName() );
            m_gchannels[ i ]->setChannels( m_channels[ i ]->getNumberOfChannels() );
            m_gchannels[ i ]->setLowerBound( m_channels[ i ]->getLowerBound() );
            m_gchannels[ i ]->setUpperBound( m_channels[ i ]->getUpperBound() );
            m_gchannels[ i ]->setEnabled( true );
         } else {
            m_gchannels[ i ]->setEnabled( false );
         }
      }

      return;
   }

} // namespace dt5740
