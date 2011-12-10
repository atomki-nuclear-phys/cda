// $Id$

// Qt include(s):
#include <QtGui/QStackedWidget>
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Loader.h"
#else
#   include "device/Loader.h"
#endif

// Local include(s):
#include "CaenEditor.h"
#include "CreateAction.h"

namespace dev {

   CaenEditor::CaenEditor( QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ),
        caen::Crate< dev::CaenGui >(),
        m_selfModification( false ),
        m_logger( "dev::CaenEditor" ) {

      resize( CaenGui::WIDGET_WIDTH + 20, 640 );
      setMinimumSize( CaenGui::WIDGET_WIDTH + 20, 640 );

      // Set up which loader the base class should use:
      setLoader( Loader::instance() );

      // Create the dropdown menu that can be used to create a device:
      m_createDevice = new QComboBox( this );
      m_createDevice->setGeometry( QRect( 50, 0, 250, 25 ) );
      m_createDevice->addItem( tr( "Create CAEN device..." ) );

      // Add all CAEN devices to the list:
      QStringList devices = m_loader->getDeviceNames();
      for( QStringList::const_iterator dev = devices.begin();
           dev != devices.end(); ++dev ) {

         //
         // Check if this particular device could be inserted into this
         // crate slot:
         //
         CaenGui* device =
            m_loader->getFactory( *dev )->createDevice< CaenGui >();
         // Check if this is a CAMAC device:
         if( ! device ) {
            continue;
         }
         // We don't need the device anymore:
         delete device;

         // Add this device to the list:
         m_createDevice->addItem( m_loader->getFactory( *dev )->longName(),
                                  *dev );
      }

      // Connect the dropdown menu to the slot handling the creation of
      // a CAEN device:
      connect( m_createDevice, SIGNAL( currentIndexChanged( int ) ),
               this, SLOT( createDeviceSlot( int ) ) );

      // Create the button clearing the configuration:
      m_clearDevice = new QPushButton( tr( "Clear device" ), this );
      m_clearDevice->setGeometry( QRect( 320, 0, 150, 25 ) );
      m_clearDevice->setEnabled( false );
      connect( m_clearDevice, SIGNAL( pressed() ),
               this, SLOT( clearDeviceSlot() ) );

      // Create a widget to show the devices in:
      m_deviceStack = new QStackedWidget( this );
      m_deviceStack->setGeometry( QRect( 10, 50, CaenGui::WIDGET_WIDTH,
                                         CaenGui::WIDGET_HEIGHT ) );

   }

   bool CaenEditor::readConfig( QIODevice* dev ) {

      // Read the configuration using the base class:
      if( ! caen::Crate< dev::CaenGui >::readConfig( dev ) ) {
         REPORT_ERROR( tr( "Couldn't read binary configuration" ) );
         return false;
      }

      // Now show the device:
      if( m_devices.size() ) {
         // Disable the device creation temporarily:
         m_selfModification = true;
         // Display the device:
         m_deviceStack->addWidget( m_devices.begin()->second );
         // Make the combo box display the appropriate device type:
         const int index =
            m_createDevice->findData( m_devices.begin()->second->deviceName() );
         if( index >= 0 ) {
            m_createDevice->setCurrentIndex( index );
         }
         // Re-enable the device creation:
         m_selfModification = false;
         // Disable the device creation widget:
         m_createDevice->setEnabled( false );
         // Enable the device clearing widget:
         m_clearDevice->setEnabled( true );
      }

      return true;
   }

   bool CaenEditor::readConfig( const QDomElement& node ) {

      // Read the configuration using the base class:
      if( ! caen::Crate< dev::CaenGui >::readConfig( node ) ) {
         REPORT_ERROR( tr( "Couldn't read binary configuration" ) );
         return false;
      }

      // Now show the device:
      if( m_devices.size() ) {
         // Disable the device creation temporarily:
         m_selfModification = true;
         // Display the device:
         m_deviceStack->addWidget( m_devices.begin()->second );
         // Make the combo box display the appropriate device type:
         const int index =
            m_createDevice->findData( m_devices.begin()->second->deviceName() );
         if( index >= 0 ) {
            m_createDevice->setCurrentIndex( index );
         }
         // Re-enable the device creation:
         m_selfModification = false;
         // Disable the device creation widget:
         m_createDevice->setEnabled( false );
         // Enable the device clearing widget:
         m_clearDevice->setEnabled( true );
      }

      return true;
   }

   /**
    * @param index Index of the menu item in m_createDevice
    */
   void CaenEditor::createDeviceSlot( int index ) {

      // Don't do anything if the object is modifying itself:
      if( m_selfModification ) return;

      // A security check:
      if( ! checkLoader() ) return;

      // First let's make sure that this is the only device:
      clearDeviceSlot();

      // Get the name of the library that we have to use:
      const QString type = m_createDevice->itemData( index ).toString();

      //
      // Try to access the Factory of this device type:
      //
      Factory* factory = m_loader->getFactory( type );
      if( ! factory ) {
         REPORT_ERROR( tr( "No factory found for device type \"%1\"" ).arg( type ) );
         return;
      } else {
         REPORT_VERBOSE( tr( "Factory found for device type \"%1\"" ).arg( type ) );
      }

      //
      // Try to create the new device:
      //
      CaenGui* device = factory->createDevice< CaenGui >();
      if( ! device ) {
         REPORT_ERROR( tr( "No GUI implemented by device \"%1\"" ).arg( type ) );
         return;
      } else {
         REPORT_VERBOSE( tr( "GUI object created for device type \"%1\"" ).arg( type ) );
      }

      // Store the device:
      m_devices[ 0 ] = device;

      // Configure the device and show it:
      device->setID( 0 );
      m_deviceStack->addWidget( device );

      // Disable the device creation widget:
      m_createDevice->setEnabled( false );
      // Enable the device clearing widget:
      m_clearDevice->setEnabled( true );

      return;
   }

   void CaenEditor::clearDeviceSlot() {

      // Return right away if there's no device configured at the moment:
      if( ! m_devices.size() ) return;

      // Disable the device creation temporarily:
      m_selfModification = true;

      // If there is a device configured, remove it from the stack, and
      // then remove it completely:
      m_deviceStack->removeWidget( m_devices.begin()->second );
      clear();
      m_createDevice->setCurrentIndex( 0 );

      // Enable the device creation widget:
      m_createDevice->setEnabled( true );
      // Disable the device clearing widget:
      m_clearDevice->setEnabled( false );

      // Re-enable the device creation:
      m_selfModification = false;

      return;
   }

} // namespace dev
