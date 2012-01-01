// $Id$

// Qt include(s):
#include <QtCore/QRect>
#include <QtCore/QLine>
#include <QtGui/QPainter>
#include <QtGui/QFont>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/errorcheck.h"
#else
#   include "common/errorcheck.h"
#endif

// Local include(s):
#include "Gui.h"
#include "ChannelGui.h"

namespace ad1000 {

   /**
    * The constructor creates all member objects and arranges them
    * in the area given to the device.
    *
    * @param parent The Qt parent of the widget
    * @param flags The Qt flags given to the widget
    */
   Gui::Gui( QWidget* parent, Qt::WindowFlags flags )
      : dev::CamacGui( parent, flags ) {

      //
      // Create a label telling us what kind of device this is:
      //
      m_topLabel = new QLabel( tr( "AD1000 Analog-to-Digital Converter" ),
                               this );
      m_topLabel->setAlignment( Qt::AlignCenter );
      m_topLabel->setGeometry( QRect( 35, 30, 430, 25 ) );


      //
      // Create a label identifying the channel name settings:
      //
      m_nameLabel = new QLabel( tr( "Name" ), this );
      m_nameLabel->setAlignment( Qt::AlignCenter );
      m_nameLabel->setGeometry( QRect( 35, 75, 75, 25 ) );

      //
      // Create a label identifying the histogram channel settings:
      //
      m_channelsLabel = new QLabel( tr( "Channels" ), this );
      m_channelsLabel->setAlignment( Qt::AlignCenter );
      m_channelsLabel->setGeometry( QRect( 115, 75, 75, 25 ) );

      //
      // Create a label identifying the histogram lower bound settings:
      //
      m_lowerBoundLabel = new QLabel( tr( "Lower" ), this );
      m_lowerBoundLabel->setAlignment( Qt::AlignCenter );
      m_lowerBoundLabel->setGeometry( QRect( 195, 75, 75, 25 ) );

      //
      // Create a label identifying the histogram upper bound settings:
      //
      m_upperBoundLabel = new QLabel( tr( "Upper" ), this );
      m_upperBoundLabel->setAlignment( Qt::AlignCenter );
      m_upperBoundLabel->setGeometry( QRect( 275, 75, 75, 25 ) );

      //
      // Create the widget modifying the settings of the input channel:
      //
      m_gchannel = new ChannelGui( this );
      m_gchannel->setGeometry( QRect( 35, 100,
                                      ChannelGui::WIDTH,
                                      ChannelGui::HEIGHT ) );

      // Before connecting all the signals, make sure that the displayed
      // and stored values for the channel are the same:
      m_channel.setName( m_gchannel->getName() );
      m_channel.setNumberOfChannels( m_gchannel->getChannels() );
      m_channel.setLowerBound( m_gchannel->getLowerBound() );
      m_channel.setUpperBound( m_gchannel->getUpperBound() );

      // Now connect the signals:
      connect( m_gchannel,
               SIGNAL( nameChanged( const QString& ) ),
               this, SLOT( nameChangedSlot( const QString& ) ) );
      connect( m_gchannel, SIGNAL( channelsChanged( int ) ),
               this, SLOT( channelsChangedSlot( int ) ) );
      connect( m_gchannel, SIGNAL( lowerBoundChanged( double ) ),
               this, SLOT( lowerBoundChangedSlot( double ) ) );
      connect( m_gchannel, SIGNAL( upperBoundChanged( double ) ),
               this, SLOT( upperBoundChangedSlot( double ) ) );

      //
      // Create the widget modifying the LAM generation setting:
      //
      m_generateLamEdit = new QCheckBox( tr( "Generate LAM" ), this );
      m_generateLamEdit->setGeometry( QRect( 35, 175, 120, 25 ) );
      connect( m_generateLamEdit, SIGNAL( toggled( bool ) ),
               this, SLOT( generateLamChangedSlot( bool ) ) );
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

      delete m_gchannel;

      delete m_generateLamEdit;
   }

   /**
    * The function calls the ad1000::Device::readConfig(QIODevice*) function
    * to read the device configuration and then calls sync() to show the
    * new configuration correctly.
    */
   bool Gui::readConfig( QIODevice* dev ) {

      CHECK( Device::readConfig( dev ) );
      sync();
      return true;
   }

   /**
    * The function calls the ad1000::Device::readConfig(QDomElement&) function
    * to read the device configuration and then calls sync() to show the
    * new configuration correctly.
    */
   bool Gui::readConfig( const QDomElement& node ) {

      CHECK( Device::readConfig( node ) );
      sync();
      return true;
   }

   /**
    * The function draws a simple representation of the CAMAC module
    * in a 20x200 pixel format. The input channels are shown in green
    * when enabled, and red when disabled. Additionally a small "LAM"
    * label is shown at the bottom if LAM generation is enabled.
    */
   void Gui::drawModule( QPainter& painter ) const {

      //
      // Draw the background color:
      //
      painter.setPen( Qt::blue );
      painter.setBrush( Qt::blue );
      painter.drawRect( QRect( 0, 0, 20, 200 ) );

      //
      // Draw a "TDC" sign on the top:
      //
      QFont font = painter.font();
#ifdef Q_OS_DARWIN
      font.setPointSize( 8 );
#else
      font.setPointSize( 6 );
#endif
      painter.setFont( font );
      painter.setPen( Qt::white );
      painter.setBrush( Qt::white );
      painter.drawText( QRect( 0, 0, 20, 15 ), Qt::AlignCenter,
                        tr( "ADC" ) );
      painter.drawLine( QLine( 3, 15, 17, 15 ) );

      //
      // Unfortunately I don't know what the module looks like, so let's just use an
      // empty space for now...
      //

      //
      // Draw a "LAM" sign on the bottom if the module is set to
      // generate LAM:
      //
      if( m_generateLam ) {
         painter.setPen( Qt::green );
         painter.setBrush( Qt::green );
         painter.drawText( QRect( 0, 185, 20, 15 ), Qt::AlignCenter,
                           tr( "LAM" ) );
      }

      return;
   }

   int Gui::deviceWidth() const {

      return 1;
   }

   void Gui::nameChangedSlot( const QString& text ) {

      m_channel.setName( text );
      return;
   }

   void Gui::channelsChangedSlot( int channels ) {

      m_channel.setNumberOfChannels( channels );
      return;
   }

   void Gui::lowerBoundChangedSlot( double value ) {

      m_channel.setLowerBound( value );
      return;
   }

   void Gui::upperBoundChangedSlot( double value ) {

      m_channel.setUpperBound( value );
      return;
   }

   void Gui::generateLamChangedSlot( bool on ) {

      m_generateLam = on;
      emit redrawModule();
      return;
   }

   /**
    * After a configuration is read from file, the graphical objects have
    * to be sync-ed to show this new configuration. This function takes
    * care of doing that.
    */
   void Gui::sync() {

      // Sync the settings of the channel:
      m_gchannel->setEnabled( false );
      m_gchannel->setName( m_channel.getName() );
      m_gchannel->setChannels( m_channel.getNumberOfChannels() );
      m_gchannel->setLowerBound( m_channel.getLowerBound() );
      m_gchannel->setUpperBound( m_channel.getUpperBound() );
      m_gchannel->setEnabled( true );

      m_generateLamEdit->setChecked( m_generateLam );

      return;
   }

} // namespace ad1000
