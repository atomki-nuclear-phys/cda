// $Id$

// Qt include(s):
#include <QtCore/QRect>
#include <QtGui/QGroupBox>
#include <QtGui/QStackedWidget>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Loader.h"
#   include "cdacore/device/Gui.h"
#else
#   include "device/Loader.h"
#   include "device/Gui.h"
#endif

// Local include(s):
#include "Editor.h"
#include "CrateWidget.h"

namespace dev {

   /**
    * This is a fixed size, 520x640 pixel size widget. The constituents
    * are created and arranged in the constructor such that they would
    * "look good"...
    */
   Editor::Editor( QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ), m_logger( "dev::Editor" ) {

      // Set the size of the widget:
      resize( CrateWidget::WIDTH, 640 );
      setMinimumSize( CrateWidget::WIDTH, 640 );
      setMaximumSize( CrateWidget::WIDTH, 640 );

      // Load all plugins:
      m_loader = new Loader();
      m_loader->loadAll();

      // Create and arrange the crate object:
      m_crateView = new CrateWidget( this );
      m_crateView->setLoader( m_loader );
      m_crateView->setGeometry( QRect( 0, 0, CrateWidget::WIDTH,
                                       CrateWidget::HEIGHT ) );
      connect( m_crateView, SIGNAL( doubleClicked( int ) ),
               this, SLOT( showDeviceSlot( int ) ) );

      // Create the box surrounding the device configuration:
      m_deviceBox = new QGroupBox( "No slot selected", this );
      m_deviceBox->setGeometry( QRect( 0, 270, 520, 370 ) );

      /// Create the widget showing the device configuration:
      m_deviceWidget = new QStackedWidget( m_deviceBox );
      m_deviceWidget->setGeometry( QRect( 10, 10, Gui::WIDGET_WIDTH,
                                          Gui::WIDGET_HEIGHT ) );
      connect( m_deviceWidget, SIGNAL( widgetRemoved( int ) ),
               this, SLOT( removeDeviceSlot( int ) ) );

   }

   /**
    * The destructor deletes all objects created in the constructor.
    */
   Editor::~Editor() {

      delete m_crateView;
      delete m_deviceWidget;
      delete m_deviceBox;
      delete m_loader;

   }

   /**
    * The configuration reading is simply forwarded to the CrateWidget
    * object.
    */
   bool Editor::readConfig( QIODevice* dev ) {

      return m_crateView->readConfig( dev );

   }

   /**
    * The configuration writing is simply forwarded to the CrateWidget
    * object.
    */
   bool Editor::writeConfig( QIODevice* dev ) const {

      return m_crateView->writeConfig( dev );

   }

   /**
    * The configuration reading is simply forwarded to the CrateWidget
    * object.
    */
   bool Editor::readConfig( const QDomElement& node ) {

      return m_crateView->readConfig( node );

   }

   /**
    * The configuration writing is simply forwarded to the CrateWidget
    * object.
    */
   bool Editor::writeConfig( QDomElement& node ) const {

      return m_crateView->writeConfig( node );

   }

   /**
    * Clearing the configuration means clearing and redrawing the CrateWidget
    * object.
    */
   void Editor::clear() {

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
   void Editor::showDeviceSlot( int slot ) {

      // Access the device:
      Gui* device = m_crateView->getDevice( slot );
      if( ! device ) {
         m_logger << msg::DEBUG << "There is no device in slot "
                  << slot << msg::endmsg;
         return;
      }

      // Do nothing if it is being shown right now:
      if( device == m_deviceWidget->currentWidget() ) {
         m_logger << msg::VERBOSE << "This device is shown right now..."
                  << msg::endmsg;
         return;
      }

      // Remove the currently shown widget. (m_deviceWidget should only
      // only contain one widget at all times...)
      QWidget* shown_widget = m_deviceWidget->currentWidget();
      if( shown_widget ) {
         m_deviceWidget->removeWidget( shown_widget );
      }

      // Show the device widget:
      m_deviceWidget->addWidget( device );
      m_deviceWidget->setCurrentWidget( device );
      m_deviceBox->setTitle( "Slot " + QString::number( slot ) );

      return;

   }

   /**
    * This is just to always show a good title on top of the QGroupBox...
    */
   void Editor::removeDeviceSlot( int /* slot */ ) {

      m_deviceBox->setTitle( "No slot selected" );
      return;

   }

} // namespace dev
