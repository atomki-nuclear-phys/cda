// $Id$

// Qt include(s):
#include <QtGui/QTabWidget>
#include <QtGui/QComboBox>
#include <QtGui/QIcon>
#include <QtGui/QMessageBox>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Loader.h"
#   include "cdacore/common/errorcheck.h"
#else
#   include "device/Loader.h"
#   include "common/errorcheck.h"
#endif

// Local include(s):
#include "CaenEditor.h"

namespace dev {

   CaenEditor::CaenEditor( QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ),
        dev::Crate< dev::CaenGui >( "CAEN", true ),
        m_logger( "dev::CaenEditor" ) {

      resize( 520, 640 );
      setMinimumSize( 520, 640 );
      setMaximumSize( 520, 640 );

      // Set up which loader the base class should use:
      setLoader( Loader::instance() );

      // Create the dropdown menu that can be used to create a device:
      m_createDevice = new QComboBox( this );
      m_createDevice->setGeometry( QRect( 110, 0, 300, 25 ) );
      m_createDevice->addItem( tr( "Add CAEN device..." ) );

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

      // Create a widget to show the devices in:
      m_deviceTab = new QTabWidget( this );
      m_deviceTab->setGeometry( QRect( 0, 50, 520,
                                       CaenGui::WIDGET_HEIGHT + 30 ) );
      m_deviceTab->setUsesScrollButtons( true );
      m_deviceTab->setTabShape( QTabWidget::Triangular );
      m_deviceTab->setTabsClosable( true );
      connect( m_deviceTab, SIGNAL( tabCloseRequested( int ) ),
               this, SLOT( deleteDeviceSlot( int ) ) );

   }

   CaenEditor::~CaenEditor() {

      // The tab widget thinks that it actually owns its widgets,
      // so it deletes them in the end. To avoid crashes because of
      // a double-delete, let's not allow the base class to try
      // to delete these objects itself.
      m_devices.clear();

      delete m_createDevice;
      delete m_deviceTab;
   }

   bool CaenEditor::readConfig( QIODevice* dev ) {

      // Read the configuration using the base class:
      if( ! dev::Crate< dev::CaenGui >::readConfig( dev ) ) {
         REPORT_ERROR( tr( "Couldn't read binary configuration" ) );
         return false;
      }

      // Now show the device(s):
      std::map< unsigned int, dev::CaenGui* >::const_iterator itr =
         m_devices.begin();
      std::map< unsigned int, dev::CaenGui* >::const_iterator end =
         m_devices.end();
      for( ; itr != end; ++itr ) {
         // Display the device:
         m_deviceTab->addTab( itr->second, itr->second->deviceName() );
         // Connect its signal(s):
         connect( itr->second, SIGNAL( idChanged() ),
                  this, SLOT( idChangedSlot() ) );
      }

      // Make sure the combo box is in the first position:
      m_createDevice->setCurrentIndex( 0 );

      // Check if the GUI is still "consistent":
      CHECK( consistent() );
      // Check if the configuration is "consistent":
      idChangedSlot();

      return true;
   }

   bool CaenEditor::readConfig( const QDomElement& node ) {

      // Read the configuration using the base class:
      if( ! dev::Crate< dev::CaenGui >::readConfig( node ) ) {
         REPORT_ERROR( tr( "Couldn't read binary configuration" ) );
         return false;
      }

      // Now show the device(s):
      std::map< unsigned int, dev::CaenGui* >::const_iterator itr =
         m_devices.begin();
      std::map< unsigned int, dev::CaenGui* >::const_iterator end =
         m_devices.end();
      for( ; itr != end; ++itr ) {
         // Display the device:
         m_deviceTab->addTab( itr->second, itr->second->deviceName() );
         // Connect its signal(s):
         connect( itr->second, SIGNAL( idChanged() ),
                  this, SLOT( idChangedSlot() ) );
      }

      // Make sure the combo box is in the first position:
      m_createDevice->setCurrentIndex( 0 );

      // Check if the GUI is still "consistent":
      CHECK( consistent() );
      // Check if the configuration is "consistent":
      idChangedSlot();

      return true;
   }

   void CaenEditor::clear() {

      m_deviceTab->clear();
      dev::Crate< dev::CaenGui >::clear();

      // Check if the GUI is still "consistent":
      if( ! consistent() ) {
         REPORT_ERROR( tr( "The GUI is not consistent after ID change "
                           "handling" ) );
         return;
      }

      return;
   }

   /**
    * @param index Index of the menu item in m_createDevice
    */
   void CaenEditor::createDeviceSlot( int index ) {

      // A security check:
      if( ! checkLoader() ) return;

      // Get the name of the library that we have to use:
      const QString type = m_createDevice->itemData( index ).toString();

      // Ignore possible signals coming from switching the combo box
      // back to the first item:
      if( type == "" ) return;

      // Make sure the combo box is in the first position:
      m_createDevice->setEnabled( false ); // So that the function doesn't call itself
      m_createDevice->setCurrentIndex( 0 );
      m_createDevice->setEnabled( true );

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

      // Connect its signal(s):
      connect( device, SIGNAL( idChanged() ),
               this, SLOT( idChangedSlot() ) );

      // Store the device with a "random" ID first:
      m_devices[ 10000 + m_devices.size() ] = device;

      // Show the device:
      m_deviceTab->addTab( device, device->deviceName() );

      // Check if the GUI is still "consistent":
      if( ! consistent() ) {
         REPORT_ERROR( tr( "The GUI is not consistent after ID change "
                           "handling" ) );
         return;
      }

      return;
   }

   void CaenEditor::deleteDeviceSlot( int index ) {

      // Return right away if there's no device configured at the moment:
      if( ! m_devices.size() ) return;

      // Get the pointer of the currently shown device:
      CaenGui* device = dynamic_cast< CaenGui* >( m_deviceTab->widget( index ) );
      if( ! device ) {
         REPORT_ERROR( tr( "No device selected currently" ) );
         return;
      }

      // Remove the current device from view:
      m_deviceTab->removeTab( index );

      // Now remove it from the base class, and delete it. Have
      // to do it in such a complicated way, because the devices
      // are not necessarily keyed correctly at this point...
      std::map< unsigned int, dev::CaenGui* >::iterator itr =
         m_devices.begin();
      std::map< unsigned int, dev::CaenGui* >::iterator end =
         m_devices.end();
      for( ; itr != end; ++itr ) {
         if( itr->second != device ) continue;
         m_devices.erase( itr );
         break;
      }
      delete device;

      // Make sure the combo box is in the correct position:
      m_createDevice->setCurrentIndex( 0 );

      // Check if the GUI is still "consistent":
      if( ! consistent() ) {
         REPORT_ERROR( tr( "The GUI is not consistent after ID change "
                           "handling" ) );
         return;
      }

      return;
   }

   void CaenEditor::idChangedSlot() {

      // Make an "endless" loop, with a bit of added security.
      // (We don't expect more than ~20 devices, so the loop should
      // be able to stop at 100 turns...)
      for( int i = 0; i < 100; ++i ) {

         // Flag showing whether we need to continue with this
         // loop:
         bool repeat = false;

         // Loop over the devices:
         std::map< unsigned int, dev::CaenGui* >::iterator itr =
            m_devices.begin();
         std::map< unsigned int, dev::CaenGui* >::iterator end =
            m_devices.end();
         for( ; itr != end; ++itr ) {

            // If this device's ID didn't change, then leave it in peace:
            if( itr->first == itr->second->getID() ) continue;

            // Check that nobody else is using this ID:
            if( m_devices.find( itr->second->getID() ) != m_devices.end() ) {
               QMessageBox::warning( this, tr( "Clash between devices" ),
                                     tr( "The connection parameters configured for the "
                                         "device clash with those of another device. "
                                         "Please fix this before continuing." ) );
               break;
            }

            // If it now has an ID that is not used by others, then let's re-add it with
            // this ID to the base class:
            CaenGui* device = itr->second;
            m_devices.erase( itr );
            m_devices[ device->getID() ] = device;

            // Let's start the algorithm once more:
            repeat = true;
            break;
         }

         // If the loop doesn't have to be continued, break out from it:
         if( ! repeat ) break;
      }

      // Check if the GUI is still "consistent":
      if( ! consistent() ) {
         REPORT_ERROR( tr( "The GUI is not consistent after ID change "
                           "handling" ) );
         return;
      }

      return;
   }

   bool CaenEditor::consistent() const {

      return ( static_cast< int >( m_devices.size() ) == m_deviceTab->count() );
   }

} // namespace dev
