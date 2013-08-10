// $Id$

// Qt include(s):
#include <QtCore/QRect>
#include <QGroupBox>
#include <QStackedWidget>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Loader.h"
#   include "cdacore/device/CamacGui.h"
#else
#   include "device/Loader.h"
#   include "device/CamacGui.h"
#endif

// Local include(s):
#include "CamacEditor.h"
#include "CamacCrateWidget.h"

namespace dev {

   /**
    * This is a fixed size, 520x640 pixel size widget. The constituents
    * are created and arranged in the constructor such that they would
    * "look good"...
    */
   CamacEditor::CamacEditor( QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ), m_logger( "dev::CamacEditor" ) {

      // Set the size of the widget:
      resize( CamacCrateWidget::WIDTH, 640 );
      setMinimumSize( CamacCrateWidget::WIDTH, 640 );
      setMaximumSize( CamacCrateWidget::WIDTH, 640 );

      // Load all plugins:
      Loader::instance()->loadAll();

      // Create and arrange the crate object:
      m_crateView = new CamacCrateWidget( this );
      m_crateView->setLoader( Loader::instance() );
      m_crateView->setGeometry( QRect( 0, 0, CamacCrateWidget::WIDTH,
                                       CamacCrateWidget::HEIGHT ) );
      connect( m_crateView, SIGNAL( doubleClicked( int ) ),
               this, SLOT( showDeviceSlot( int ) ) );

      // Create the box surrounding the device configuration:
      m_deviceBox = new QGroupBox( tr( "No slot selected" ), this );
      m_deviceBox->setGeometry( QRect( 0, 270, 520, 370 ) );

      /// Create the widget showing the device configuration:
      m_deviceWidget = new QStackedWidget( m_deviceBox );
      m_deviceWidget->setGeometry( QRect( 10, 10, CamacGui::WIDGET_WIDTH,
                                          CamacGui::WIDGET_HEIGHT ) );
      connect( m_deviceWidget, SIGNAL( widgetRemoved( int ) ),
               this, SLOT( removeDeviceSlot( int ) ) );

   }

   /**
    * The destructor deletes all objects created in the constructor.
    */
   CamacEditor::~CamacEditor() {

      delete m_crateView;
      delete m_deviceWidget;
      delete m_deviceBox;
   }

   /**
    * The configuration reading is simply forwarded to the CamacCrateWidget
    * object.
    */
   bool CamacEditor::readConfig( QIODevice* dev ) {

      return m_crateView->readConfig( dev );
   }

   /**
    * The configuration writing is simply forwarded to the CamacCrateWidget
    * object.
    */
   bool CamacEditor::writeConfig( QIODevice* dev ) const {

      return m_crateView->writeConfig( dev );
   }

   /**
    * The configuration reading is simply forwarded to the CamacCrateWidget
    * object.
    */
   bool CamacEditor::readConfig( const QDomElement& node ) {

      return m_crateView->readConfig( node );
   }

   /**
    * The configuration writing is simply forwarded to the CamacCrateWidget
    * object.
    */
   bool CamacEditor::writeConfig( QDomElement& node ) const {

      return m_crateView->writeConfig( node );
   }

   /**
    * The function call is simply forvarded to the
    * CamacCrateWidget.
    */
   bool CamacEditor::canRead( QIODevice* dev ) const {

      return m_crateView->canRead( dev );
   }

   /**
    * The function call is simply forvarded to the
    * CamacCrateWidget.
    */
   bool CamacEditor::canRead( const QDomElement& node ) const {

      return m_crateView->canRead( node );
   }

   /**
    * Clearing the configuration means clearing and redrawing the CamacCrateWidget
    * object.
    */
   void CamacEditor::clear() {

      m_crateView->clear();
      m_crateView->update();
      return;
   }

   /**
    * This function takes care of diplaying the detailed configuration
    * options of a CAMAC device if it's been selected.
    *
    * @param slot The crate slot in which the device can be found
    */
   void CamacEditor::showDeviceSlot( int slot ) {

      // Access the device:
      CamacGui* device = m_crateView->getDevice( slot );
      if( ! device ) {
         m_logger << msg::DEBUG
                  << tr( "There is no device in slot %1" ).arg( slot )
                  << msg::endmsg;
         return;
      }

      // Do nothing if it is being shown right now:
      if( device == m_deviceWidget->currentWidget() ) {
         REPORT_VERBOSE( tr( "This device is shown right now..." ) );
         return;
      }

      // Remove the currently shown widget. (m_deviceWidget should
      // only contain one widget at all times...)
      QWidget* shown_widget = m_deviceWidget->currentWidget();
      if( shown_widget ) {
         m_deviceWidget->removeWidget( shown_widget );
      }

      // Show the device widget:
      m_deviceWidget->addWidget( device );
      m_deviceWidget->setCurrentWidget( device );
      m_deviceBox->setTitle( tr( "Slot %1" ).arg( QString::number( slot ) ) );

      return;
   }

   /**
    * This is just to always show a good title on top of the QGroupBox...
    */
   void CamacEditor::removeDeviceSlot( int /* slot */ ) {

      m_deviceBox->setTitle( tr( "No slot selected" ) );
      return;
   }

} // namespace dev
