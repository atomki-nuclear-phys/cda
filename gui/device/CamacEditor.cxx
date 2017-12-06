
// Qt include(s):
#include <QRect>

// CDA include(s):
#include "device/Loader.h"
#include "device/CamacGui.h"

// Local include(s):
#include "CamacEditor.h"

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
      m_crateView.reset( new CamacCrateWidget( this ) );
      m_crateView->setLoader( Loader::instance() );
      m_crateView->setGeometry( QRect( 0, 0, CamacCrateWidget::WIDTH,
                                       CamacCrateWidget::HEIGHT ) );
      connect( m_crateView.get(), SIGNAL( doubleClicked( int ) ),
               this, SLOT( showDeviceSlot( int ) ) );

      // Create the box surrounding the device configuration:
      m_deviceBox.reset( new QGroupBox( tr( "No slot selected" ), this ) );
      m_deviceBox->setGeometry( QRect( 0, 270, 520, 370 ) );

      /// Create the widget showing the device configuration:
      m_deviceWidget.reset( new QStackedWidget( m_deviceBox.get() ) );
      m_deviceWidget->setGeometry( QRect( 10, 10, CamacGui::WIDGET_WIDTH,
                                          CamacGui::WIDGET_HEIGHT ) );
      connect( m_deviceWidget.get(), SIGNAL( widgetRemoved( int ) ),
               this, SLOT( removeDeviceSlot( int ) ) );

   }

   CamacEditor::~CamacEditor() {

      // Since QStackedWidget owns the widgets that were added to it, and
      // the dev::Crate object does so too, we need to do something explicit
      // here. Luckily QStackedWidget gets notified when one of its objects
      // get deleted, so we just need to make sure that the dev::Crate
      // object is deleted before the QStackedWidget object is.
      m_crateView.reset();
   }

   /**
    * The configuration reading is simply forwarded to the CamacCrateWidget
    * object.
    */
   StatusCode CamacEditor::readConfig( QIODevice& dev ) {

      return m_crateView->readConfig( dev );
   }

   /**
    * The configuration writing is simply forwarded to the CamacCrateWidget
    * object.
    */
   StatusCode CamacEditor::writeConfig( QIODevice& dev ) const {

      return m_crateView->writeConfig( dev );
   }

   /**
    * The configuration reading is simply forwarded to the CamacCrateWidget
    * object.
    */
   StatusCode CamacEditor::readConfig( const QDomElement& node ) {

      return m_crateView->readConfig( node );
   }

   /**
    * The configuration writing is simply forwarded to the CamacCrateWidget
    * object.
    */
   StatusCode CamacEditor::writeConfig( QDomElement& node ) const {

      return m_crateView->writeConfig( node );
   }

   /**
    * The function call is simply forvarded to the
    * CamacCrateWidget.
    */
   bool CamacEditor::canRead( QIODevice& dev ) const {

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
    * This function takes care of displaying the detailed configuration
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
