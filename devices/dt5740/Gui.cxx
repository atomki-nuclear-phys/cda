// $Id$

// Qt include(s):
#include <QtCore/QRect>
#include <QtCore/QLine>
#include <QtGui/QFont>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QScrollArea>
#include <QtGui/QPixmap>
#include <QtGui/QGroupBox>
#include <QtGui/QComboBox>
#include <QtGui/QSpinBox>

// Local include(s):
#include "Gui.h"
#include "GroupGui.h"
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
      : dev::CaenGui( parent, flags ),
        m_logger( "dt5740::Gui" ) {

      //
      // Create the widget that will hold all the configuration widgets:
      //
      m_scrollWidget = new QWidget( 0, flags );
      m_scrollWidget->setGeometry( QRect( 0, 0, WIDGET_WIDTH - 20, 3000 ) );

      //
      // Embed the previous widget into a scroll area:
      //
      m_scrollArea = new QScrollArea( this );
      m_scrollArea->setGeometry( QRect( 0, 0, WIDGET_WIDTH, WIDGET_HEIGHT ) );
      m_scrollArea->setWidget( m_scrollWidget );

      //
      // Put a picture of the digitizer at the top:
      //
      m_image = new QLabel( m_scrollWidget );
      m_image->setGeometry( QRect( 50, 10, 400, 181 ) );
      m_image->setPixmap( QPixmap( ":/img/dt5740.png" ) );
      m_image->setScaledContents( true );

      //
      // Create a label telling us what kind of device this is:
      //
      m_topLabel = new QLabel( tr( "CAEN DT5740 32 Channel 12 bit "
                                   "62.5MS/s Digitizer" ),
                               m_scrollWidget );
      m_topLabel->setAlignment( Qt::AlignCenter );
      m_topLabel->setGeometry( QRect( 20, 200, WIDGET_WIDTH - 40, 35 ) );
      QFont titleFont( m_topLabel->font() );
      titleFont.setPointSize( titleFont.pointSize() + 2 );
      titleFont.setBold( true );
      m_topLabel->setFont( titleFont );

      //
      // Create the device connection settings:
      //
      m_connectionBox = new QGroupBox( tr( "Connection parameters" ),
                                       m_scrollWidget );
      m_connectionBox->setGeometry( QRect( 10, 245, WIDGET_WIDTH - 30, 90 ) );

      m_connModeLabel = new QLabel( tr( "Connection mode:" ),
                                    m_connectionBox);
      m_connModeLabel->setGeometry( QRect( 10, 25, 130, 25 ) );

      m_connMode = new QComboBox( m_connectionBox );
      m_connMode->setGeometry( QRect( 160, 25, 220, 25 ) );
      m_connMode->addItem( tr( "USB" ) );
      m_connMode->addItem( tr( "PCI Optical Link" ) );
      m_connMode->addItem( tr( "PCIe Optical Link" ) );
      m_connMode->addItem( tr( "PCIe Embedded Digitizer" ) );
      connect( m_connMode, SIGNAL( currentIndexChanged( int ) ),
               this, SLOT( connectionModeSlot( int ) ) ); 

      m_connLinkLabel = new QLabel( tr( "Connection link:" ),
                                    m_connectionBox );
      m_connLinkLabel->setGeometry( QRect( 10, 55, 130, 25 ) );

      m_connLink = new QSpinBox( m_connectionBox );
      m_connLink->setGeometry( QRect( 160, 55, 220, 25 ) );
      m_connLink->setRange( 0, 10 );
      connect( m_connLink, SIGNAL( valueChanged( int ) ),
               this, SLOT( connectionLinkSlot( int ) ) );

      //
      // Create the channel groups:
      //
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {

         // Create a new channel group:
         m_ggroups[ i ] = new GroupGui( i, m_scrollWidget );
         m_ggroups[ i ]->setGeometry( QRect( 5, 340 + i * ( GroupGui::HEIGHT + 10 ),
                                             GroupGui::WIDTH, GroupGui::HEIGHT ) );

         // Connect all of its signals to this object:
         connect( m_ggroups[ i ], SIGNAL( trigEnabled( int, bool ) ),
                  this, SLOT( trigEnabledSlot( int, bool ) ) );
         connect( m_ggroups[ i ], SIGNAL( trigOutEnabled( int, bool ) ),
                  this, SLOT( trigOutEnabledSlot( int, bool ) ) );
         connect( m_ggroups[ i ], SIGNAL( trigOvlpEnabled( int, bool ) ),
                  this, SLOT( trigOvlpEnabledSlot( int, bool ) ) );
         connect( m_ggroups[ i ],
                  SIGNAL( trigMode( int, dt5740::GroupConfig::TriggerMode ) ),
                  this,
                  SLOT( trigModeSlot( int, dt5740::GroupConfig::TriggerMode ) ) );
         connect( m_ggroups[ i ], SIGNAL( trigThreshold( int, int ) ),
                  this, SLOT( trigThresholdSlot( int, int ) ) );
         connect( m_ggroups[ i ], SIGNAL( trigMask( int, unsigned int ) ),
                  this, SLOT( trigMaskSlot( int, unsigned int ) ) );
         connect( m_ggroups[ i ], SIGNAL( postTrigSamples( int, int ) ),
                  this, SLOT( postTrigSamplesSlot( int, int ) ) );
         connect( m_ggroups[ i ], SIGNAL( dcOffset( int, int ) ),
                  this, SLOT( dcOffsetSlot( int, int ) ) );
         connect( m_ggroups[ i ], SIGNAL( patGenEnabled( int, bool ) ),
                  this, SLOT( patGenEnabledSlot( int, bool ) ) );
         connect( m_ggroups[ i ],
                  SIGNAL( gateMode( int, dt5740::GroupConfig::GateMode ) ),
                  this, SLOT( gateModeSlot( int, dt5740::GroupConfig::GateMode ) ) );
         connect( m_ggroups[ i ],
                  SIGNAL( bufferMode( int, dt5740::GroupConfig::BufferMode ) ),
                  this,
                  SLOT( bufferModeSlot( int, dt5740::GroupConfig::BufferMode ) ) );

         // Connect the signals of the channels belonging to the channel group
         // to this object:
         for( int j = 0; j < GroupConfig::CHANNELS_IN_GROUP; ++j ) {
            connect( m_ggroups[ i ]->getChannel( j ),
                     SIGNAL( enableChanged( int, bool ) ),
                     this, SLOT( channelEnabledSlot( int, bool ) ) );
            connect( m_ggroups[ i ]->getChannel( j ),
                     SIGNAL( nameChanged( int, const QString& ) ),
                     this, SLOT( nameChangedSlot( int, const QString& ) ) );
            connect( m_ggroups[ i ]->getChannel( j ),
                     SIGNAL( channelsChanged( int, int ) ),
                     this, SLOT( channelsChangedSlot( int, int ) ) );
            connect( m_ggroups[ i ]->getChannel( j ),
                     SIGNAL( lowerBoundChanged( int, double ) ),
                     this, SLOT( lowerBoundChangedSlot( int, double ) ) );
            connect( m_ggroups[ i ]->getChannel( j ),
                     SIGNAL( upperBoundChanged( int, double ) ),
                     this, SLOT( upperBoundChangedSlot( int, double ) ) );
         }
      }

   }

   /**
    * The destructor deletes all the objects created in the constructor.
    */
   Gui::~Gui() {

      delete m_image;
      delete m_topLabel;

      delete m_connModeLabel;
      delete m_connMode;
      delete m_connLinkLabel;
      delete m_connLink;
      delete m_connectionBox;

      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {
         delete m_ggroups[ i ];
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

   void Gui::connectionModeSlot( int index ) {

      switch( index ) {

      case 0:
         m_connType = caen::Digitizer::USB;
         break;
      case 1:
         m_connType = caen::Digitizer::PCI_OpticalLink;
         break;
      case 2:
         m_connType = caen::Digitizer::PCIE_OpticalLink;
         break;
      case 3:
         m_connType = caen::Digitizer::PCIE_EmbeddedDigitizer;
         break;
      default:
         REPORT_ERROR( tr( "Connection mode not recognized" ) );
         m_connType = caen::Digitizer::USB;
         break;
      }

      return;
   }

   void Gui::connectionLinkSlot( int link ) {

      m_linkNumber = link;
      return;
   }

   void Gui::channelEnabledSlot( int channel, bool on ) {

      if( on ) {
         m_channels[ channel ] = new ChannelConfig();
         m_channels[ channel ]->setChannelNumber( channel );
      } else {
         delete m_channels[ channel ];
         m_channels[ channel ] = 0;
      }

      return;
   }

   void Gui::nameChangedSlot( int channel, const QString& text ) {

      m_channels[ channel ]->setName( text );
      return;
   }

   void Gui::channelsChangedSlot( int channel, int channels ) {

      m_channels[ channel ]->setNumberOfChannels( channels );
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

   void Gui::trigEnabledSlot( int group, bool state ) {

      m_groups[ group ].setTrigEnabled( state );
      return;
   }

   void Gui::trigOutEnabledSlot( int group, bool state ) {

      m_groups[ group ].setTrigOutEnabled( state );
      return;
   }

   void Gui::trigOvlpEnabledSlot( int group, bool state ) {

      m_groups[ group ].setTrigOvlpEnabled( state );
      return;
   }

   void Gui::trigModeSlot( int group, GroupConfig::TriggerMode mode ) {

      m_groups[ group ].setTrigMode( mode );
      return;
   }

   void Gui::trigThresholdSlot( int group, int value ) {

      m_groups[ group ].setTrigThreshold( value );
      return;
   }

   void Gui::trigMaskSlot( int group, unsigned int value ) {

      m_groups[ group ].setTrigMask( value );
      return;
   }

   void Gui::postTrigSamplesSlot( int group, int value ) {

      m_groups[ group ].setPostTrigSamples( value );
      return;
   }

   void Gui::dcOffsetSlot( int group, int value ) {

      m_groups[ group ].setDCOffset( value );
      return;
   }

   void Gui::patGenEnabledSlot( int group, bool state ) {

      m_groups[ group ].setPatGenEnabled( state );
      return;
   }

   void Gui::gateModeSlot( int group, GroupConfig::GateMode mode ) {

      m_groups[ group ].setGateMode( mode );
      return;
   }

   void Gui::bufferModeSlot( int group, GroupConfig::BufferMode mode ) {

      m_groups[ group ].setBufferMode( mode );
      return;
   }

   /**
    * After a configuration is read from file, the graphical objects have
    * to be sync-ed to show this new configuration. This function takes
    * care of doing that.
    */
   void Gui::sync() {

      // Set the connection mode:
      m_connMode->setEnabled( false );
      switch( m_connType ) {
      case caen::Digitizer::USB:
         m_connMode->setCurrentIndex( 0 );
         break;
      case caen::Digitizer::PCI_OpticalLink:
         m_connMode->setCurrentIndex( 1 );
         break;
      case caen::Digitizer::PCIE_OpticalLink:
         m_connMode->setCurrentIndex( 2 );
         break;
      case caen::Digitizer::PCIE_EmbeddedDigitizer:
         m_connMode->setCurrentIndex( 3 );
         break;
      default:
         REPORT_ERROR( tr( "Connection mode not recognized" ) );
         break;
      }
      m_connMode->setEnabled( true ); 

      // Set the connection link:
      m_connLink->setEnabled( false );
      m_connLink->setValue( m_linkNumber );
      m_connLink->setEnabled( true );

      //
      // For each channel the adjustments to the graphical objects
      // have to be made in a tricky way. The changes have to be made
      // while the graphical channel is disabled. Otherwise the signals
      // sent from the channel as its values are changing would create
      // "strange" effects.
      //
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {
         m_ggroups[ i ]->sync( m_groups[ i ] );
         for( int j = 0; j < GroupConfig::CHANNELS_IN_GROUP; ++j ) {
            const int ch_number = i * GroupConfig::CHANNELS_IN_GROUP + j;
            if( m_channels[ ch_number ] ) {
               m_ggroups[ i ]->getChannel( j )->setEnabled( false );
               m_ggroups[ i ]->getChannel( j )->setName( m_channels[ ch_number ]->getName() );
               m_ggroups[ i ]->getChannel( j )->setChannels( m_channels[ ch_number ]->getNumberOfChannels() );
               m_ggroups[ i ]->getChannel( j )->setLowerBound( m_channels[ ch_number ]->getLowerBound() );
               m_ggroups[ i ]->getChannel( j )->setUpperBound( m_channels[ ch_number ]->getUpperBound() );
               m_ggroups[ i ]->getChannel( j )->setEnabled( true );
            } else {
               m_ggroups[ i ]->getChannel( j )->setEnabled( false );
            }
         }
      }

      return;
   }

} // namespace dt5740
