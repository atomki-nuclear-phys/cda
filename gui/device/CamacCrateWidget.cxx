
// Qt include(s):
#include <QtGlobal>
#include <QRect>
#include <QLine>
#include <QString>
#include <QPoint>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QMenu>

// CDA include(s):
#include "common/errorcheck.h"
#include "device/Factory.h"
#include "device/Loader.h"

// Local include(s):
#include "CamacCrateWidget.h"
#include "CreateAction.h"
#include "ClearAction.h"

namespace dev {

   //
   // Initialise the class constants:
   //
   const int CamacCrateWidget::NUMBER_OF_SLOTS = 24;

   const int CamacCrateWidget::SLOT_WIDTH  = 20;
   const int CamacCrateWidget::SLOT_HEIGHT = 200;
   const int CamacCrateWidget::BORDER_SIZE = 10;

   const int CamacCrateWidget::WIDTH  =
      ( CamacCrateWidget::NUMBER_OF_SLOTS + 1 ) * CamacCrateWidget::SLOT_WIDTH +
      2 * CamacCrateWidget::BORDER_SIZE;
   const int CamacCrateWidget::HEIGHT = CamacCrateWidget::SLOT_HEIGHT +
      2 * CamacCrateWidget::BORDER_SIZE + 30;

   /**
    * The constructor takes care of carefully constructing the dev::Crate
    * baseclass. Apart from this it basically just sets the fixed size of
    * the widget.
    *
    * @param parent Qt parent of the object
    * @param flags Qt flags for the widget
    */
   CamacCrateWidget::CamacCrateWidget( QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ),
        Crate< dev::CamacGui >( "CAMAC", true ),
        m_logger( "dev::CamacCrateWidget" ) {

      resize( WIDTH, HEIGHT );
      setMinimumSize( WIDTH, HEIGHT );
      setMaximumSize( WIDTH, HEIGHT );
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
   StatusCode CamacCrateWidget::readConfig( QIODevice& dev ) {

      //
      // Read the configuration using the base class:
      //
      CHECK( Crate< dev::CamacGui >::readConfig( dev ) );
      //
      // Connect the signals coming from the configured modules:
      //
      DeviceMap_t::const_iterator dev_itr = m_devices.begin();
      DeviceMap_t::const_iterator dev_end = m_devices.end();
      for( ; dev_itr != dev_end; ++dev_itr ) {
         connect( dev_itr->second.get(), SIGNAL( redrawModule() ),
                  this, SLOT( update() ) );
      }
      //
      // Redraw the widget:
      //
      update();

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   /**
    * The configuration itself is read using the
    * dev::Crate::readConfig(QDomNode&) function. But in order to correctly
    * display the read configuration, the signals coming from the CAMAC
    * modules has to be connected to this object, and finally the crate has to
    * be redrawn.
    *
    * @param element The XML node holding the CAMAC crate configuration
    */
   StatusCode CamacCrateWidget::readConfig( const QDomElement& element ) {

      //
      // Read the configuration using the base class:
      //
      CHECK( Crate< dev::CamacGui >::readConfig( element ) );
      //
      // Connect the signals coming from the configured modules:
      //
      DeviceMap_t::const_iterator dev_itr = m_devices.begin();
      DeviceMap_t::const_iterator dev_end = m_devices.end();
      for( ; dev_itr != dev_end; ++dev_itr ) {
         connect( dev_itr->second.get(), SIGNAL( redrawModule() ),
                  this, SLOT( update() ) );
      }
      //
      // Redraw the widget:
      //
      update();

      // Return gracefully:
      return StatusCode::SUCCESS;
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
   CamacGui* CamacCrateWidget::getDevice( int slot ) {

      DeviceMap_t::iterator dev = m_devices.find( slot );
      if( dev != m_devices.end() ) {
         return dev->second.get();
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
   void CamacCrateWidget::setDevice( int slot,
                                     std::unique_ptr< CamacGui > device ) {

      //
      // Save the pointer to the new device:
      //
      DeviceMap_t::iterator dev = m_devices.find( slot );
      if( dev != m_devices.end() ) {
         m_logger << msg::WARNING
                  << tr( "Redefining device in slot %1" ).arg( slot )
                  << msg::endmsg;
      }
      CamacGui* ptr = device.get(); // Remember the pointer...
      m_devices[ slot ].swap( device );
      //
      // Take care of correctly displaying the new device:
      //
      connect( ptr, SIGNAL( redrawModule() ),
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
   void CamacCrateWidget::createSlot( int slot, const QString& type ) {

      // A security check:
      if( ! checkLoader() ) return;

      REPORT_VERBOSE( tr( "\"%1\" creation requested in slot %2" )
                      .arg( type ).arg( slot ) );

      //
      // Try to access the Factory of this device type:
      //
      Factory& factory = m_loader->getFactory( type );
      REPORT_VERBOSE( tr( "Factory found for device type \"%1\"" )
                      .arg( type ) );

      //
      // Try to create the new device:
      //
      std::unique_ptr< CamacGui > device = factory.createDevice< CamacGui >();
      if( ! device.get() ) {
         REPORT_ERROR( tr( "No GUI implemented by device \"%1\"" )
                       .arg( type ) );
         return;
      } else {
         REPORT_VERBOSE( tr( "GUI object created for device type \"%1\"" )
                         .arg( type ) );
      }

      //
      // Configure and register the new device:
      //
      device->setID( slot );
      setDevice( slot, std::move( device ) );
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
   void CamacCrateWidget::clearSlot( int slot ) {

      REPORT_VERBOSE( tr( "Clearing slot %1" ).arg( slot ) );

      DeviceMap_t::iterator dev = m_devices.find( slot );
      if( dev != m_devices.end() ) {
         m_devices.erase( dev );
      } else {
         m_logger << msg::DEBUG << tr( "Slot was empty..." ) << msg::endmsg;
      }
      update();

      return;
   }

   /**
    * This function takes care of drawing the widget. For this it uses
    * a QPainter object. The CAMAC crate is drawn in a sylized fashion
    * out of basic shapes (lines, rectangles, circles). This is the function
    * calling the dev::CamacGui::drawModule functions for all the CAMAC devices.
    *
    * The result should look something like this:
    *
    * @image html CrateWidget.png
    */
   void CamacCrateWidget::paintEvent( QPaintEvent* /* event */ ) {

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
      DeviceMap_t::const_iterator dev_itr = m_devices.begin();
      DeviceMap_t::const_iterator dev_end = m_devices.end();
      for( ; dev_itr != dev_end; ++dev_itr ) {

         painter.translate( BORDER_SIZE +
                            ( static_cast< int >( dev_itr->first ) - 1 ) *
                            SLOT_WIDTH,
                            BORDER_SIZE );
         dev_itr->second->drawModule( painter );
         painter.translate( - ( BORDER_SIZE +
                                ( static_cast< int >( dev_itr->first ) - 1 ) *
                                SLOT_WIDTH ),
                            - BORDER_SIZE );

      }

      //
      // Draw the lines separating the crate slots:
      //
      painter.setPen( Qt::DotLine );
      painter.setBrush( Qt::black );
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
   void CamacCrateWidget::mousePressEvent( QMouseEvent* event ) {

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
         // Create a context-menu. The menu always lists all the available
         // module types for adding to the given slot, and if the slot is
         // already occupied, it lists an item for removing that device.
         //

         // Create the menu:
         QMenu* menu = new QMenu( this );

         // Create the menu items for creating new modules:
         QMenu* addMenu = menu->addMenu( tr( "Add device" ) );
         if( m_loader ) {
            QStringList devices = m_loader->getDeviceNames();
            for( QStringList::const_iterator dev = devices.begin();
                 dev != devices.end(); ++dev ) {

               //
               // Check if this particular device could be inserted into this
               // crate slot:
               //
               std::unique_ptr< CamacGui > device =
                  m_loader->getFactory( *dev ).createDevice< CamacGui >();
               // Check if this is a CAMAC device:
               if( ! device.get() ) {
                  continue;
               }
               bool offerDevice = true;
               // Check if it has enough space from the side of the crate:
               if( device->deviceWidth() > slot ) {
                  offerDevice = false;
               }
               // Check if there's another module in its way:
               for( int i = 0; i < device->deviceWidth(); ++i ) {
                  if( getDevice( slot - i ) ) {
                     offerDevice = false;
                     break;
                  }
               }
               // Skip this offer if the device can't be put into this slot:
               if( ! offerDevice ) {
                  continue;
               }

               // Create a menu item for the device:
               QString name = m_loader->getFactory( *dev ).longName();
               CreateAction* ac = new CreateAction( name, slot, *dev, this );
               connect( ac, SIGNAL( triggered( int, const QString& ) ),
                        this, SLOT( createSlot( int, const QString& ) ) );
               addMenu->addAction( ac );

            }
         } else {
            addMenu->addAction( tr( "No dev::Loader received..." ) );
         }

         // Create the menu item for removing the module:
         if( getDevice( slot ) ) {
            ClearAction* ac = new ClearAction( tr( "Clear slot" ), slot, this );
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
   void CamacCrateWidget::mouseDoubleClickEvent( QMouseEvent* event ) {

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
