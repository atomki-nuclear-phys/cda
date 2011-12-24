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
      m_scrollWidget->setGeometry( QRect( 0, 0, WIDGET_WIDTH - 20, 4220 ) );

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
      m_connectionBox->setGeometry( QRect( 10, 245, WIDGET_WIDTH - 40, 90 ) );

      m_connModeLabel = new QLabel( tr( "Connection mode:" ),
                                    m_connectionBox);
      m_connModeLabel->setGeometry( QRect( 10, 25, 130, 25 ) );

      m_connMode = new QComboBox( m_connectionBox );
      m_connMode->setGeometry( QRect( 180, 25, 250, 25 ) );
      m_connMode->addItem( tr( "USB" ) );
      m_connMode->addItem( tr( "PCI Optical Link" ) );
      m_connMode->addItem( tr( "PCIe Optical Link" ) );
      m_connMode->addItem( tr( "PCIe Embedded Digitizer" ) );
      connect( m_connMode, SIGNAL( currentIndexChanged( int ) ),
               this, SLOT( connectionModeSlot( int ) ) ); 

      m_connLinkLabel = new QLabel( tr( "Link number:" ),
                                    m_connectionBox );
      m_connLinkLabel->setGeometry( QRect( 10, 55, 130, 25 ) );

      m_connLink = new QSpinBox( m_connectionBox );
      m_connLink->setGeometry( QRect( 180, 55, 150, 25 ) );
      m_connLink->setRange( 0, 10 );
      m_connLink->setToolTip( tr( "In USB mode this means the ID given by the OS "
                                  "to the USB connection. If this is the only such "
                                  "device connected by USB, it will be \"0\"." ) );
      connect( m_connLink, SIGNAL( valueChanged( int ) ),
               this, SLOT( connectionLinkSlot( int ) ) );

      //
      // Create the trigger settings:
      //
      m_triggerBox = new QGroupBox( tr( "Trigger settings" ),
                                    m_scrollWidget );
      m_triggerBox->setGeometry( QRect( 10, 345, WIDGET_WIDTH - 40, 180 ) );

      m_trigOvlpEnabledWidget = new QCheckBox( tr( "Enable trigger overlaps" ),
                                               m_triggerBox );
      m_trigOvlpEnabledWidget->setGeometry( QRect( 10, 25, 400, 25 ) );
      connect( m_trigOvlpEnabledWidget, SIGNAL( toggled( bool ) ),
               this, SLOT( trigOvlpEnabledSlot( bool ) ) );

      m_extTrigEnabledWidget = new QCheckBox( tr( "Enable external trigger" ),
                                              m_triggerBox );
      m_extTrigEnabledWidget->setGeometry( QRect( 10, 55, 400, 25 ) );
      connect( m_extTrigEnabledWidget, SIGNAL( toggled( bool ) ),
               this, SLOT( extTrigEnabledSlot( bool ) ) );

      m_extTrigOutEnabledWidget =
         new QCheckBox( tr( "Forward external trigger to front panel" ),
                        m_triggerBox );
      m_extTrigOutEnabledWidget->setGeometry( QRect( 10, 85, 400, 25 ) );
      connect( m_extTrigOutEnabledWidget, SIGNAL( toggled( bool ) ),
               this, SLOT( extTrigOutEnabledSlot( bool ) ) );

      m_trigModeLabel = new QLabel( tr( "Trigger mode:" ),
                                    m_triggerBox );
      m_trigModeLabel->setGeometry( QRect( 10, 115, 150, 25 ) );

      m_trigModeWidget = new QComboBox( m_triggerBox );
      m_trigModeWidget->setGeometry( QRect( 180, 115, 250, 25 ) );
      m_trigModeWidget->addItem( tr( "Trigger on input over threshold" ) );
      m_trigModeWidget->addItem( tr( "Trigger on input under threshold" ) );
      connect( m_trigModeWidget, SIGNAL( currentIndexChanged( int ) ),
               this, SLOT( trigModeSlot( int ) ) ); 

      m_postTrigPercentageLabel = new QLabel( tr( "Post trigger percentage:" ),
                                              m_triggerBox );
      m_postTrigPercentageLabel->setGeometry( QRect( 10, 145, 150, 25 ) );

      m_postTrigPercentageWidget = new QSpinBox( m_triggerBox );
      m_postTrigPercentageWidget->setGeometry( QRect( 180, 145, 150, 25 ) );
      m_postTrigPercentageWidget->setRange( 0, 100 );
      m_postTrigPercentageWidget->setValue( 100 );
      m_postTrigPercentageWidget->setToolTip( tr( "You can set here how many of "
                                                  "the collected samples should be "
                                                  "taken after the trigger "
                                                  "signal." ) );
      connect( m_postTrigPercentageWidget, SIGNAL( valueChanged( int ) ),
               this, SLOT( postTrigPercentageSlot( int ) ) ); 

      //
      // Create the data acquisition settings:
      //
      m_acquisitionBox = new QGroupBox( tr( "Data acquisition settings" ),
                                        m_scrollWidget );
      m_acquisitionBox->setGeometry( QRect( 10, 535, WIDGET_WIDTH - 40, 240 ) );

      m_patGenEnabledWidget = new QCheckBox( tr( "Enable test pattern generation" ),
                                             m_acquisitionBox );
      m_patGenEnabledWidget->setGeometry( QRect( 10, 25, 400, 25 ) );
      connect( m_patGenEnabledWidget, SIGNAL( toggled( bool ) ),
               this, SLOT( patGenEnabledSlot( bool ) ) );

      m_saveRawNtupleWidget = new QCheckBox( tr( "Save raw data to ntuple" ),
                                             m_acquisitionBox );
      m_saveRawNtupleWidget->setGeometry( QRect( 10, 55, 400, 25 ) );
      connect( m_saveRawNtupleWidget, SIGNAL( toggled( bool ) ),
               this, SLOT( saveRawNtupleSlot( bool ) ) );

      m_acqModeLabel = new QLabel( tr( "Acquisition mode:" ),
                                   m_acquisitionBox );
      m_acqModeLabel->setGeometry( QRect( 10, 85, 150, 25 ) );

      m_acqModeWidget = new QComboBox( m_acquisitionBox );
      m_acqModeWidget->setGeometry( QRect( 180, 85, 250, 25 ) );
      m_acqModeWidget->addItem( "Register controlled" );
      m_acqModeWidget->addItem( "GPI controlled" );
      connect( m_acqModeWidget, SIGNAL( currentIndexChanged( int ) ),
               this, SLOT( acqModeSlot( int ) ) );

      m_gateModeLabel = new QLabel( tr( "Gate mode:" ),
                                    m_acquisitionBox );
      m_gateModeLabel->setGeometry( QRect( 10, 115, 150, 25 ) );

      m_gateModeWidget = new QComboBox( m_acquisitionBox );
      m_gateModeWidget->setGeometry( QRect( 180, 115, 250, 25 ) );
      m_gateModeWidget->addItem( "Window" );
      m_gateModeWidget->addItem( "Single shot" );
      connect( m_gateModeWidget, SIGNAL( currentIndexChanged( int ) ),
               this, SLOT( gateModeSlot( int ) ) );

      m_bufferModeLabel = new QLabel( tr( "Buffer mode:" ),
                                      m_acquisitionBox );
      m_bufferModeLabel->setGeometry( QRect( 10, 145, 150, 25 ) );

      m_bufferModeWidget = new QComboBox( m_acquisitionBox );
      m_bufferModeWidget->setGeometry( QRect( 180, 145, 250, 25 ) );
      m_bufferModeWidget->setToolTip( tr( "You can choose how many samples should be "
                                          "collected after each trigger, using this "
                                          "property." ) );
      m_bufferModeWidget->addItem( tr( "1 buffer with 192k samples" ) );
      m_bufferModeWidget->addItem( tr( "2 buffers with 96k samples" ) );
      m_bufferModeWidget->addItem( tr( "4 buffers with 48k samples" ) );
      m_bufferModeWidget->addItem( tr( "8 buffers with 24k samples" ) );
      m_bufferModeWidget->addItem( tr( "16 buffers with 12k samples" ) );
      m_bufferModeWidget->addItem( tr( "32 buffers with 6k samples" ) );
      m_bufferModeWidget->addItem( tr( "64 buffers with 3k samples" ) );
      m_bufferModeWidget->addItem( tr( "128 buffers with 1536 samples" ) );
      m_bufferModeWidget->addItem( tr( "256 buffers with 768 samples" ) );
      m_bufferModeWidget->addItem( tr( "512 buffers with 384 samples" ) );
      m_bufferModeWidget->addItem( tr( "1024 buffers with 192 samples" ) );
      connect( m_bufferModeWidget, SIGNAL( currentIndexChanged( int ) ),
               this, SLOT( bufferModeSlot( int ) ) );

      m_clockSourceLabel = new QLabel( tr( "Clock source:" ),
                                       m_acquisitionBox );
      m_clockSourceLabel->setGeometry( QRect( 10, 175, 150, 25 ) );

      m_clockSourceWidget = new QComboBox( m_acquisitionBox );
      m_clockSourceWidget->setGeometry( QRect( 180, 175, 250, 25 ) );
      m_clockSourceWidget->setToolTip( tr( "You can choose where the device should "
                                           "get its clock for the digitization timing." ) );
      m_clockSourceWidget->addItem( tr( "Internal" ) );
      m_clockSourceWidget->addItem( tr( "External" ) );
      connect( m_clockSourceWidget, SIGNAL( currentIndexChanged( int ) ),
               this, SLOT( clockSourceSlot( int ) ) );

      m_evCountModeLabel = new QLabel( tr( "Event count mode:" ),
                                       m_acquisitionBox );
      m_evCountModeLabel->setGeometry( QRect( 10, 205, 150, 25 ) );

      m_evCountModeWidget = new QComboBox( m_acquisitionBox );
      m_evCountModeWidget->setGeometry( QRect( 180, 205, 250, 25 ) );
      m_evCountModeWidget->setToolTip( tr( "You can choose how the number of events should "
                                           "be counted by the device." ) );
      m_evCountModeWidget->addItem( tr( "Count accepted triggers" ) );
      m_evCountModeWidget->addItem( tr( "Count all triggers" ) );
      connect( m_evCountModeWidget, SIGNAL( currentIndexChanged( int ) ),
               this, SLOT( evCountModeSlot( int ) ) );

      //
      // Create the channel groups:
      //
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {

         // Create a new channel group:
         m_ggroups[ i ] = new GroupGui( m_groups[ i ], m_scrollWidget );
         m_ggroups[ i ]->setGeometry( QRect( 5, 785 + i * ( GroupGui::HEIGHT + 10 ),
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

      delete m_trigOvlpEnabledWidget;
      delete m_extTrigEnabledWidget;
      delete m_extTrigOutEnabledWidget;
      delete m_trigModeLabel;
      delete m_trigModeWidget;
      delete m_postTrigPercentageLabel;
      delete m_postTrigPercentageWidget;
      delete m_triggerBox;

      delete m_patGenEnabledWidget;
      delete m_saveRawNtupleWidget;
      delete m_acqModeLabel;
      delete m_acqModeWidget;
      delete m_gateModeLabel;
      delete m_gateModeWidget;
      delete m_bufferModeLabel;
      delete m_bufferModeWidget;
      delete m_clockSourceLabel;
      delete m_clockSourceWidget;
      delete m_evCountModeLabel;
      delete m_evCountModeWidget;
      delete m_acquisitionBox;

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

      // Tell the crate that the device's ID changed:
      emit idChanged();

      return;
   }

   void Gui::connectionLinkSlot( int link ) {

      // Remember the link number:
      m_linkNumber = link;

      // Tell the crate that the device's ID changed:
      emit idChanged();

      return;
   }

   void Gui::trigOvlpEnabledSlot( bool checked ) {

      m_trigOvlpEnabled = checked;
      return;
   }

   void Gui::extTrigEnabledSlot( bool checked ) {

      m_extTrigEnabled = checked;
      return;
   }

   void Gui::extTrigOutEnabledSlot( bool checked ) {

      m_extTrigOutEnabled = checked;
      return;
   }

   void Gui::trigModeSlot( int index ) {

      // Translate the state of the combo box into an enumeration value:
      switch( index ) {

      case 0:
         m_trigMode = TriggerOnInputOverThreshold;
         break;
      case 1:
         m_trigMode = TriggerOnInputUnderThreshold;
         break;
      default:
         REPORT_ERROR( tr( "Trigger mode not recognized" ) );
         break;
      }

      return;
   }

   void Gui::postTrigPercentageSlot( int value ) {

      m_postTrigPercentage = value;
      return;
   }

   void Gui::patGenEnabledSlot( bool checked ) {

      m_patGenEnabled = checked;
      return;
   }

   void Gui::saveRawNtupleSlot( bool checked ) {

      m_saveRawNtuple = checked;
      return;
   }

   void Gui::acqModeSlot( int index ) {

      // Translate the state of the combo box into an enumeration value:
      switch( index ) {

      case 0:
         m_acqMode = caen::Digitizer::ACQ_SW_Controlled;
         break;
      case 1:
         m_acqMode = caen::Digitizer::ACQ_SIn_Controlled;;
         break;
      default:
         REPORT_ERROR( tr( "Acquisition mode not recognized" ) );
         break;
      }

      return;
   }

   void Gui::gateModeSlot( int index ) {

      // Translate the state of the combo box into an enumeration value:
      switch( index ) {

      case 0:
         m_gateMode = WindowGate;
         break;
      case 1:
         m_gateMode = SingleShotGate;
         break;
      default:
         REPORT_ERROR( tr( "Gate mode not recognized" ) );
         break;
      }

      return;
   }

   void Gui::bufferModeSlot( int index ) {

      // Translate the state of the combo box into an enumeration value:
      switch( index ) {

      case 0:
         m_bufferMode = NBuffers1;
         break;
      case 1:
         m_bufferMode = NBuffers2;
         break;
      case 2:
         m_bufferMode = NBuffers4;
         break;
      case 3:
         m_bufferMode = NBuffers8;
         break;
      case 4:
         m_bufferMode = NBuffers16;
         break;
      case 5:
         m_bufferMode = NBuffers32;
         break;
      case 6:
         m_bufferMode = NBuffers64;
         break;
      case 7:
         m_bufferMode = NBuffers128;
         break;
      case 8:
         m_bufferMode = NBuffers256;
         break;
      case 9:
         m_bufferMode = NBuffers512;
         break;
      case 10:
         m_bufferMode = NBuffers1024;
         break;
      default:
         REPORT_ERROR( tr( "Buffer mode not recognized" ) );
         break;
      }

      return;
   }

   void Gui::clockSourceSlot( int index ) {

      switch( index ) {

      case 0:
         m_clockSource = CLK_Internal;
         break;
      case 1:
         m_clockSource = CLK_External;
         break;
      default:
         REPORT_ERROR( tr( "Clock source not recognized" ) );
         break;
      }

      return;
   }

   void Gui::evCountModeSlot( int index ) {

      switch( index ) {

      case 0:
         m_evCountMode = EV_CountAcceptedTriggers;
         break;
      case 1:
         m_evCountMode = EV_CountAllTriggers;
         break;
      default:
         REPORT_ERROR( tr( "Event counting mode not recognized" ) );
         break;
      }

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

      // Set the trigger overlap:
      m_trigOvlpEnabledWidget->setEnabled( false );
      m_trigOvlpEnabledWidget->setChecked( m_trigOvlpEnabled );
      m_trigOvlpEnabledWidget->setEnabled( true );

      // Set the external trigger:
      m_extTrigEnabledWidget->setEnabled( false );
      m_extTrigEnabledWidget->setChecked( m_extTrigEnabled );
      m_extTrigEnabledWidget->setEnabled( true );

      // Set the external trigger forwarding:
      m_extTrigOutEnabledWidget->setEnabled( false );
      m_extTrigOutEnabledWidget->setChecked( m_extTrigOutEnabled );
      m_extTrigOutEnabledWidget->setEnabled( true );

      // Set the trigger mode:
      m_trigModeWidget->setEnabled( false );
      switch( m_trigMode ) {

      case TriggerOnInputOverThreshold:
         m_trigModeWidget->setCurrentIndex( 0 );
         break;
      case TriggerOnInputUnderThreshold:
         m_trigModeWidget->setCurrentIndex( 1 );
         break;
      default:
         REPORT_ERROR( tr( "Trigger mode not recognized" ) );
         break;
      }
      m_trigModeWidget->setEnabled( true );

      // Set the percentage of post trigger samples:
      m_postTrigPercentageWidget->setEnabled( false );
      m_postTrigPercentageWidget->setValue( m_postTrigPercentage );
      m_postTrigPercentageWidget->setEnabled( true );

      // Set the state of the pattern generation:
      m_patGenEnabledWidget->setEnabled( false );
      m_patGenEnabledWidget->setChecked( m_patGenEnabled );
      m_patGenEnabledWidget->setEnabled( true );

      // Set the state of the raw ntuple writing:
      m_saveRawNtupleWidget->setEnabled( false );
      m_saveRawNtupleWidget->setChecked( m_saveRawNtuple );
      m_saveRawNtupleWidget->setEnabled( true );

      // Set the acquisition mode:
      m_acqModeWidget->setEnabled( false );
      switch( m_acqMode ) {

      case caen::Digitizer::ACQ_SW_Controlled:
         m_acqModeWidget->setCurrentIndex( 0 );
         break;
      case caen::Digitizer::ACQ_SIn_Controlled:
         m_acqModeWidget->setCurrentIndex( 1 );
         break;
      default:
         REPORT_ERROR( tr( "Acquisition mode not recognized" ) );
         break;
      }
      m_acqModeWidget->setEnabled( true );

      // Set the gate mode:
      m_gateModeWidget->setEnabled( false );
      switch( m_gateMode ) {

      case WindowGate:
         m_gateModeWidget->setCurrentIndex( 0 );
         break;
      case SingleShotGate:
         m_gateModeWidget->setCurrentIndex( 1 );
         break;
      default:
         REPORT_ERROR( tr( "Gate mode not recognized" ) );
         break;
      }
      m_gateModeWidget->setEnabled( true );

      // Set the buffer mode:
      m_bufferModeWidget->setEnabled( false );
      switch( m_bufferMode ) {

      case NBuffers1:
         m_bufferModeWidget->setCurrentIndex( 0 );
         break;
      case NBuffers2:
         m_bufferModeWidget->setCurrentIndex( 1 );
         break;
      case NBuffers4:
         m_bufferModeWidget->setCurrentIndex( 2 );
         break;
      case NBuffers8:
         m_bufferModeWidget->setCurrentIndex( 3 );
         break;
      case NBuffers16:
         m_bufferModeWidget->setCurrentIndex( 4 );
         break;
      case NBuffers32:
         m_bufferModeWidget->setCurrentIndex( 5 );
         break;
      case NBuffers64:
         m_bufferModeWidget->setCurrentIndex( 6 );
         break;
      case NBuffers128:
         m_bufferModeWidget->setCurrentIndex( 7 );
         break;
      case NBuffers256:
         m_bufferModeWidget->setCurrentIndex( 8 );
         break;
      case NBuffers512:
         m_bufferModeWidget->setCurrentIndex( 9 );
         break;
      case NBuffers1024:
         m_bufferModeWidget->setCurrentIndex( 10 );
         break;
      default:
         REPORT_ERROR( tr( "Buffer mode not recognized" ) );
         break;
      }
      m_bufferModeWidget->setEnabled( true );

      // Set the clock source:
      m_clockSourceWidget->setEnabled( false );
      switch( m_clockSource ) {

      case CLK_Internal:
         m_clockSourceWidget->setCurrentIndex( 0 );
         break;
      case CLK_External:
         m_clockSourceWidget->setCurrentIndex( 1 );
         break;
      default:
         REPORT_ERROR( tr( "Clock source not recognized" ) );
         break;
      }
      m_clockSourceWidget->setEnabled( true );

      // Set the event counting mode:
      m_evCountModeWidget->setEnabled( false );
      switch( m_evCountMode ) {

      case EV_CountAcceptedTriggers:
         m_evCountModeWidget->setCurrentIndex( 0 );
         break;
      case EV_CountAllTriggers:
         m_evCountModeWidget->setCurrentIndex( 1 );
         break;
      default:
         REPORT_ERROR( tr( "Event counting mode not recognized" ) );
         break;
      }
      m_evCountModeWidget->setEnabled( true );

      // Synchronize all the groups:
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {
         m_ggroups[ i ]->sync();
      }

      return;
   }

} // namespace dt5740
