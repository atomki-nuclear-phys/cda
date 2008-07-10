// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>
#include <QtCore/QRect>
#include <QtCore/QLine>
#include <QtCore/QString>
#include <QtCore/QPoint>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QMenu>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Factory.h"
#   include "cdacore/device/Loader.h"
#else
#   include "device/Factory.h"
#   include "device/Loader.h"
#endif

// Local include(s):
#include "CrateWidget.h"
#include "CreateAction.h"
#include "ClearAction.h"

namespace dev {

   //
   // Initialise the class constants:
   //
   const int CrateWidget::SLOT_WIDTH  = 20;
   const int CrateWidget::SLOT_HEIGHT = 200;
   const int CrateWidget::BORDER_SIZE = 10;

   const int CrateWidget::WIDTH  =
      ( Crate< dev::Gui >::NUMBER_OF_SLOTS + 1 ) * CrateWidget::SLOT_WIDTH +
      2 * CrateWidget::BORDER_SIZE;
   const int CrateWidget::HEIGHT = CrateWidget::SLOT_HEIGHT +
      2 * CrateWidget::BORDER_SIZE + 30;

   /**
    * The constructor takes care of carefully constructing the dev::Crate
    * baseclass. Apart from this it basically just sets the fixed size of
    * the widget.
    *
    * @param parent Qt parent of the object
    * @param flags Qt flags for the widget
    */
   CrateWidget::CrateWidget( QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ),
        Crate< dev::Gui >( &dev::Factory::createGui ),
        m_loader( 0 ), m_logger( "dev::CrateWidget" ) {

      resize( WIDTH, HEIGHT );
      setMinimumSize( WIDTH, HEIGHT );
      setMaximumSize( WIDTH, HEIGHT );

   }

   /**
    * The devices are deleted by the dev::Crate destructor, so we don't
    * have to do anything here...
    */
   CrateWidget::~CrateWidget() {

   }

   /**
    * The configuration itself is read using the
    * dev::Crate::readConfig(QIODevice*) function. But in order to correctly
    * display the read configuration, the signals coming from the CAMAC
    * modules has to be connected to this object, and finally the crate has to
    * be redrawn.
    *
    * @param dev Device to read the binary configuration from
    */
   bool CrateWidget::readConfig( QIODevice* dev ) {

      //
      // Read the configuration using the base class:
      //
      if( ! Crate< dev::Gui >::readConfig( dev ) ) {
         return false;
      }
      //
      // Connect the signals coming from the configured modules:
      //
      for( std::map< int, Gui* >::const_iterator dev = m_devices.begin();
           dev != m_devices.end(); ++dev ) {
         connect( dev->second, SIGNAL( redrawModule() ),
                  this, SLOT( update() ) );
      }
      //
      // Redraw the widget:
      //
      update();

      return true;

   }

   /**
    * The configuration itself is read using the
    * dev::Crate::readConfig(QDomNode&) function. But in order to correctly
    * display the read configuration, the signals coming from the CAMAC
    * modules has to be connected to this object, and finally the crate has to
    * be redrawn.
    *
    * @param node The XML node holding the CAMAC crate configuration
    */
   bool CrateWidget::readConfig( const QDomNode& node ) {

      //
      // Read the configuration using the base class:
      //
      if( ! Crate< dev::Gui >::readConfig( node ) ) {
         return false;
      }
      //
      // Connect the signals coming from the configured modules:
      //
      for( std::map< int, Gui* >::const_iterator dev = m_devices.begin();
           dev != m_devices.end(); ++dev ) {
         connect( dev->second, SIGNAL( redrawModule() ),
                  this, SLOT( update() ) );
      }
      //
      // Redraw the widget:
      //
      update();

      return true;

   }

   /**
    * Both this class and its base class need to have a Loader object
    * to function. This function saves the pointer for itself first, then
    * calls the dev::Crate::setLoader function.
    *
    * @param loader Pointer to a correctly configured Loader object
    */
   void CrateWidget::setLoader( const Loader* loader ) {

      m_loader = loader;
      Crate< dev::Gui >::setLoader( loader );

      return;

   }

   /**
    * This function can be used to get direct access to one of the CAMAC
    * devices in the crate. Direct access is necessary, as this object
    * doesn't show the detailed configuration of the devices. Those have
    * to be shown in an external object.
    *
    * @param slot The crate slot
    * @returns The pointer to the module, or 0 if the slot is empty
    */
   Gui* CrateWidget::getDevice( int slot ) {

      std::map< int, Gui* >::iterator dev = m_devices.find( slot );
      if( dev != m_devices.end() ) {
         return dev->second;
      } else {
         return 0;
      }

   }

   /**
    * This function can be used to set the device in a crate slot. It
    * takes care of correctly replacing a previously configured module,
    * by deleting it first before replacing its pointer with the new one.
    * It also connects the signal(s) coming from the device to this object,
    * and redraws the widget to show the new device.
    *
    * @param slot The crate slot to configure
    * @param device Pointer to the device that we now want to use
    */
   void CrateWidget::setDevice( int slot, Gui* device ) {

      //
      // Save the pointer to the new device:
      //
      std::map< int, Gui* >::iterator dev = m_devices.find( slot );
      if( dev != m_devices.end() ) {
         m_logger << msg::WARNING << "Redefining device in slot "
                  << slot << msg::endmsg;
         delete dev->second;
         dev->second = device;
      } else {
         m_devices[ slot ] = device;
      }
      //
      // Take care of correctly displaying the new device:
      //
      connect( device, SIGNAL( redrawModule() ),
               this, SLOT( update() ) );
      update();

      return;

   }

   /**
    * This function is called by Qt when the user selects to create a new
    * CAMAC device from the context-menu of a crate slot.
    * It tries to create the new device by name using the dev::Factory
    * interface of the plugin in question. When the device is created,
    * it is given to the setDevice function to "register" it.
    *
    * @param slot The crate slot to create the device in
    * @param type The type of the device
    */
   void CrateWidget::createSlot( int slot, const QString& type ) {

      m_logger << msg::VERBOSE << "\"" << type
               <<"\" creation requested in slot " << slot << msg::endmsg;

      //
      // Check that a Loader object is available:
      //
      if( ! m_loader ) {
         m_logger << msg::ERROR << "No Loader object configured yet!"
                  << msg::endmsg;
         return;
      }

      //
      // Try to access the Factory of this device type:
      //
      Factory* factory = m_loader->getFactory( type );
      if( ! factory ) {
         m_logger << msg::ERROR << "No factory found for device type \""
                  << type << "\"" << msg::endmsg;
         return;
      } else {
         m_logger << msg::VERBOSE << "Factory found for device type \""
                  << type << "\"" << msg::endmsg;
      }

      //
      // Try to create the new device:
      //
      Gui* device = factory->createGui();
      if( ! device ) {
         m_logger << msg::ERROR << "No GUI implemented by device \""
                  << type << "\"" << msg::endmsg;
         return;
      } else {
         m_logger << msg::VERBOSE << "GUI object created for device type \""
                  << type << "\"" << msg::endmsg;
      }

      //
      // Configure and register the new device:
      //
      device->setSlot( slot );
      setDevice( slot, device );
      emit doubleClicked( slot );

      return;

   }

   /**
    * This function is called by Qt when the user selects to remove a
    * CAMAC device from the context-menu of a crate slot. It tries to
    * remove the CAMAC device from the configuration and then redraws the
    * widget.
    *
    * @param slot The crate slot to clear
    */
   void CrateWidget::clearSlot( int slot ) {

      m_logger << msg::VERBOSE << "Clearing slot " << slot << msg::endmsg;

      std::map< int, Gui* >::iterator dev = m_devices.find( slot );
      if( dev != m_devices.end() ) {
         m_devices.erase( dev );
         delete dev->second;
      } else {
         m_logger << msg::DEBUG << "Slot was empty..." << msg::endmsg;
      }
      update();

      return;

   }

   /**
    * This function takes care of drawing the widget. For this it uses
    * a QPainter object. The CAMAC crate is drawn in a sylized fashion
    * out of basic shapes (lines, rectangles, circles). This is the function
    * calling the dev::Gui::drawModule functions for all the CAMAC devices.
    *
    * The result should look something like this:
    *
    * @image html CrateWidget.png
    */
   void CrateWidget::paintEvent( QPaintEvent* /* event */ ) {

      //
      // Create the object used for painting:
      //
      QPainter painter( this );

      //
      // Draw the edges of the CAMAC crate:
      //
      painter.setPen( Qt::NoPen );
      painter.setBrush( Qt::darkGray );
      painter.drawRect( QRect( 0, 0, WIDTH,
                               BORDER_SIZE ) );
      painter.drawRect( QRect( 0, SLOT_HEIGHT + BORDER_SIZE,
                               WIDTH, BORDER_SIZE + 30 ) );
      painter.drawRect( QRect( 0, 0, BORDER_SIZE, HEIGHT ) );
      painter.drawRect( QRect( WIDTH - BORDER_SIZE, 0,
                               BORDER_SIZE, HEIGHT ) );

      painter.setPen( Qt::SolidLine );
      painter.setBrush( Qt::NoBrush );
      painter.drawRect( QRect( BORDER_SIZE, BORDER_SIZE,
                               WIDTH - 2 * BORDER_SIZE,
                               SLOT_HEIGHT ) );

      //
      // Draw the slot numbers:
      //
      painter.setPen( Qt::SolidLine );
      for( int i = 0; i < NUMBER_OF_SLOTS; ++i ) {
         painter.setBrush( Qt::NoBrush );
         painter.drawRect( QRect( BORDER_SIZE + i * SLOT_WIDTH,
                                  BORDER_SIZE + SLOT_HEIGHT,
                                  ( i == NUMBER_OF_SLOTS - 1 ? 2 * SLOT_WIDTH :
                                    SLOT_WIDTH ),
                                  SLOT_WIDTH + 5 ) );
         painter.setBrush( Qt::black );
         painter.drawText( QRect( BORDER_SIZE + i * SLOT_WIDTH,
                                  BORDER_SIZE + SLOT_HEIGHT + 5,
                                  SLOT_WIDTH, SLOT_WIDTH ),
                           Qt::AlignCenter, QString::number( i + 1 ) );
      }

      //
      // Draw the crate controller:
      //
      painter.setPen( Qt::SolidLine );
      painter.setBrush( Qt::lightGray );
      painter.drawRect( QRect( BORDER_SIZE + 23 * SLOT_WIDTH,
                               BORDER_SIZE, 2 * SLOT_WIDTH,
                               SLOT_HEIGHT ) );
      painter.translate( BORDER_SIZE + 23 * SLOT_WIDTH,
                         BORDER_SIZE );
      painter.setBrush( Qt::gray );
      painter.drawRect( QRect( 20, 100, 10, 50 ) );
      painter.setBrush( Qt::black );
      painter.drawEllipse( QRect( 23, 103, 4, 4 ) );
      painter.drawEllipse( QRect( 23, 143, 4, 4 ) );
      painter.drawRect( QRect( 23, 110, 4, 30 ) );
      painter.setBrush( Qt::green );
      painter.drawRect( QRect( 10, 40, 5, 10 ) );
      painter.drawRect( QRect( 10, 55, 5, 10 ) );
      painter.drawRect( QRect( 10, 70, 5, 10 ) );
      painter.drawRect( QRect( 10, 85, 5, 10 ) );
      painter.drawRect( QRect( 10, 105, 5, 15 ) );
      painter.drawRect( QRect( 10, 125, 5, 15 ) );
      painter.drawRect( QRect( 10, 145, 5, 15 ) );
      painter.setBrush( Qt::yellow );
      painter.drawRect( QRect( 25, 40, 5, 10 ) );
      painter.setBrush( Qt::blue );
      painter.drawRect( QRect( 0, 0, 2 * SLOT_WIDTH, 20 ) );
      painter.setPen( Qt::white );
      painter.setBrush( Qt::white );
      painter.drawText( QRect( 0, 0, 2 * SLOT_WIDTH, 20 ),
                        Qt::AlignCenter, "CC32" );
      painter.translate( - ( BORDER_SIZE + 23 * SLOT_WIDTH ),
                         - BORDER_SIZE );

      //
      // Draw the CAMAC devices:
      //
      for( std::map< int, Gui* >::const_iterator dev = m_devices.begin();
           dev != m_devices.end(); ++dev ) {

         painter.translate( BORDER_SIZE + ( dev->first - 1 ) * SLOT_WIDTH,
                            BORDER_SIZE );
         dev->second->drawModule( painter );
         painter.translate( - ( BORDER_SIZE + ( dev->first - 1 ) * SLOT_WIDTH ),
                            - BORDER_SIZE );

      }

      //
      // Draw the lines separating the crate slots:
      //
      painter.setPen( Qt::DotLine );
      for( int i = 0; i < NUMBER_OF_SLOTS - 1; ++i ) {
         painter.drawLine( QLine( BORDER_SIZE + SLOT_WIDTH +
                                  i * SLOT_WIDTH, BORDER_SIZE,
                                  BORDER_SIZE + SLOT_WIDTH +
                                  i * SLOT_WIDTH, BORDER_SIZE +
                                  SLOT_HEIGHT ) );
      }

      return;

   }

   /**
    * This function is called every time a mouse button is pressed on top
    * of the widget. If it was a left-click inside the "crate-module area",
    * then we calculate which slot was clicked and emit a clicked(int)
    * signal. If however it was a right-click inside the same area, we
    * bring up a context-menu. This menu lets the user add or remove CAMAC
    * devices to/from the system.
    *
    * @param event Object describing the mouse-click
    */
   void CrateWidget::mousePressEvent( QMouseEvent* event ) {

      //
      // Get the X and Y coordinates of the double-click:
      //
      const int x = event->x();
      const int y = event->y();

      //
      // Check whether the double-click happened inside the crate:
      //
      if( ( x < BORDER_SIZE ) || ( x > BORDER_SIZE + 23 * SLOT_WIDTH ) ||
          ( y < BORDER_SIZE ) || ( y > BORDER_SIZE + SLOT_HEIGHT ) ) {
         return;
      }

      // If we reached this point, the double-click was inside a crate slot.
      const int slot = ( x - BORDER_SIZE ) / SLOT_WIDTH + 1;

      if( event->button() == Qt::LeftButton ) {

         // Signal what happened:
         emit clicked( slot );

      } else if( event->button() == Qt::RightButton ) {

         //
         // Create a contect-menu. The menu always lists all the available
         // module types for adding to the given slot, and if the slot is
         // already occupied, it lists an item for removing that device.
         //

         // Create the menu:
         QMenu* menu = new QMenu( this );

         // Create the menu items for creating new modules:
         QMenu* addMenu = menu->addMenu( "Add module" );
         if( m_loader ) {
            QStringList devices = m_loader->getDeviceNames();
            for( QStringList::const_iterator dev = devices.begin();
                 dev != devices.end(); ++dev ) {

               CreateAction* ac = new CreateAction( *dev, slot, *dev, this );
               connect( ac, SIGNAL( triggered( int, const QString& ) ),
                        this, SLOT( createSlot( int, const QString& ) ) );
               addMenu->addAction( ac );

            }
         } else {
            addMenu->addAction( "No dev::Loader received..." );
         }

         // Create the menu item for removing the module:
         if( getDevice( slot ) ) {
            ClearAction* ac = new ClearAction( "Clear slot", slot, this );
            connect( ac, SIGNAL( triggered( int ) ),
                     this, SLOT( clearSlot( int ) ) );
            menu->addAction( ac );
         }

         // Display the menu under the cursor:
         menu->exec( mapToGlobal( QPoint( x, y ) ) );

      }

      return;

   }

   /**
    * This function is called every time a double-click happens over the
    * widget. If it was a left-button double-click inside the "crate-module
    * area", then we calculate which slot was double clicked and emit the
    * doubleClicked(int) signal.
    *
    * @param event Object describing the mouse-click
    */
   void CrateWidget::mouseDoubleClickEvent( QMouseEvent* event ) {

      //
      // Only look for left-button double-clicks:
      //
      if( event->button() != Qt::LeftButton ) return;

      //
      // Get the X and Y coordinates of the double-click:
      //
      const int x = event->x();
      const int y = event->y();

      //
      // Check whether the double-click happened inside the crate:
      //
      if( ( x < BORDER_SIZE ) || ( x > BORDER_SIZE + 23 * SLOT_WIDTH ) ||
          ( y < BORDER_SIZE ) || ( y > BORDER_SIZE + SLOT_HEIGHT ) ) {
         return;
      }

      // If we reached this point, the double-click was inside a crate slot.
      const int slot = ( x - BORDER_SIZE ) / SLOT_WIDTH + 1;

      // Signal what happened:
      emit doubleClicked( slot );

      return;

   }

} // namespace dev
