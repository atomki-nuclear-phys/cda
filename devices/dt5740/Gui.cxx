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
         m_ggroups[ i ] = new GroupGui( m_groups[ i ], m_scrollWidget );
         m_ggroups[ i ]->setGeometry( QRect( 5, 340 + i * ( GroupGui::HEIGHT + 10 ),
                                             GroupGui::WIDTH, GroupGui::HEIGHT ) );
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

      // Synchronize all the groups:
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {
         m_ggroups[ i ]->sync();
      }

      return;
   }

} // namespace dt5740
